#include "Weather.h"

#include <iostream>

// TODO: Will this cause an issue if happens in multiple spots, could potentially cause some issues
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void Weather::Projection()
{
	for (size_t i = 0; i < projectionIterations; i++)
	{
		for (size_t col = 0; col < map.cols; col++)
		{
			for (size_t row = 0; row < map.rows; row++)
			{
				ForceIncompressibilityAt(col, row);
				//total += getAverageWindVelocityAt(col, row);

			}
		}
	}
}

void Weather::Advection()
{

#pragma region old

	// Save old density


	//Array2D<float, MAP_SIZE, MAP_SIZE> oldDensities;
	//
	//for (size_t col = 0; col < map.cols; col++)
	//{
	//	for (size_t row = 0; row < map.rows; row++)
	//	{
	//		oldDensities(col, row) = map(col, row).density;
	//	}
	//}


	//for (size_t col = 0; col < map.cols; col++)
	//{
	//	for (size_t row = 0; row < map.rows; row++)
	//	{
	//		Cell& atCell = map(col, row);

	//		float density = oldDensities(col, row);

	//		if (density == 0.0f) { continue; }

	//		Cell& rightCell = map(col + 1, row);
	//		Cell& downCell = map(col, row - 1);

	//		Cell& leftCell = map(col - 1, row);
	//		Cell& upCell = map(col, row + 1);

	//		// TODO: Doesn't consider the other forces as each velocity is a vec 2
	//		float outLeft = fmax(-atCell.leftVelocity, 0.0f);
	//		float outRight = fmax(rightCell.leftVelocity, 0.0f);
	//		float outUp = fmax(atCell.upVelocity, 0.0f);
	//		float outDown = fmax(-downCell.upVelocity, 0.0f);

	//		// Divergence
	//		float totalOut = outLeft + outRight + outUp + outDown;
	//		if (totalOut == 0.0f) { continue; }

	//		// TODO: 

	//		leftCell.density += outLeft / totalOut * density;
	//		rightCell.density += outRight / totalOut * density;
	//		upCell.density += outUp / totalOut * density;
	//		downCell.density += outDown / totalOut * density;

	//		atCell.density -= ((outLeft / totalOut) + (outRight / totalOut) + (outUp / totalOut) + (outDown / totalOut)) * density;

	//	}
	//}
#pragma endregion

	for (int c = 0; c < map.cols; c++)
	{
		for (int r = 0; r < map.rows; r++)
		{
			// Work out velocity at spot (velocity spot)
			// Figure out what the prevoius position would have been
			//


			//Vec2 wind = getAverageWindVelocityAt(col, row);
			Cell& at = map(c, r);
			const Cell& leftCell = map(c - 1, r);
			const Cell& rightCell = map(c + 1, r);
			const Cell& downCell = map(c, r - 1);
			const Cell& downLeftCell = map(c - 1, r - 1);
			const Cell& upCell = map(c, r + 1);
			const Cell& upRightCell = map(c + 1, r + 1);


			
			// x, leftVelocity
			{
				Vec2 pos = { c - (gridSpacing / 2.0f), (float)r };
				float yVel = (at.upVelocity + leftCell.upVelocity + downLeftCell.upVelocity + downCell.upVelocity) / 4;
				Vec2 vel = { at.leftVelocity, yVel };

				Vec2 previousPos = pos - vel;

				Vec2 previousCellPos = { round(previousPos.x), round(previousPos.y + (gridSpacing / 2)) };

				const float downLeft = map(previousCellPos.x, previousCellPos.y).leftVelocity;
				const float downRight = map(previousCellPos.x + 1, previousCellPos.y).leftVelocity;
				const float upLeft = map(previousCellPos.x, previousCellPos.y + 1).leftVelocity;
				const float upRight = map(previousCellPos.x + 1, previousCellPos.y + 1).leftVelocity;

				// Would be positive
				float yDistanceFromDown = previousPos.y - (previousCellPos.y - (gridSpacing / 2));
				float xDistanceFromLeft = previousPos.x - (previousCellPos.x - (gridSpacing / 2));

				float rightWeight = (xDistanceFromLeft / gridSpacing);
				float leftWeight = 1 - (xDistanceFromLeft / gridSpacing);
				float upWeight = (yDistanceFromDown / gridSpacing);
				float downWeight = 1 - (yDistanceFromDown / gridSpacing);

				float previousPosVelX = 
					upWeight * leftWeight * upLeft +
					upWeight * rightWeight * upRight +
					downWeight * leftWeight * downLeft +
					downWeight * rightWeight * downRight;

				at.leftVelocity = previousPosVelX;
			}
			// y, upVelocity
			{
				Vec2 pos = { (float)c, r + (gridSpacing / 2.0f) };
				float xVel = (at.leftVelocity + rightCell.leftVelocity + upCell.leftVelocity + upRightCell.leftVelocity) / 4;
				Vec2 vel = { xVel, at.upVelocity };

				Vec2 previousPos = pos - vel;

				// Add half a grid to ensure we use the correct numbers for the weighted average
				Vec2 previousCellPos = { round(previousPos.x + (gridSpacing / 2)), round(previousPos.y) };

				const float upRight = map(previousPos.x, previousPos.y).upVelocity;
				const float downRight = map(previousPos.x, previousPos.y - 1).upVelocity;
				const float upLeft = map(previousPos.x - 1, previousPos.y).upVelocity;
				const float downLeft = map(previousPos.x - 1, previousPos.y - 1).upVelocity;
					
				float yDistanceFromDown = previousPos.y - (previousCellPos.y - (gridSpacing / 2));
				float xDistanceFromLeft = previousPos.x - (previousCellPos.x - (gridSpacing / 2));

				float rightWeight = (xDistanceFromLeft / gridSpacing);
				float leftWeight = 1 - (xDistanceFromLeft / gridSpacing);
				float upWeight = (yDistanceFromDown / gridSpacing);
				float downWeight = 1 - (yDistanceFromDown / gridSpacing);

				float previousPosVelY = 
					upWeight * leftWeight * upLeft +
					upWeight * rightWeight * upRight +
					downWeight * leftWeight * downLeft +
					downWeight * rightWeight * downRight;

				at.upVelocity = previousPosVelY;
			}
		}
	}
}

float Weather::CalculateDivergence(float right, float left, float up, float down)
{
	return right - left + up - down;
}

Weather::Weather()
{
	//RandomiseMapVelocities();
	//map(0, 0).windVelocity = { -1, -1 };
	//map(2, 2).windVelocity = { -1, -1 };
	//map(4, 4).windVelocity = { -1, -1 };
	//map(6, 6).windVelocity = { -1, -1 };
	//map(4, 6).windVelocity = { -1, -1 };
	//map(4, 8).windVelocity = { -1, -1 };
}

void Weather::Update()
{
	//Vec2 total(0, 0);

	Projection();

	Advection();

	//total /= map.getSize();
	//std::cout << total.x << ", " << total.y << "\n";

	//std::cout << (total, Vec2(0, 0)).GetMagnitude() << "\n";
}

float Weather::getDeltaTime() const
{
	return timeStep;
}

void Weather::ForceIncompressibilityAt(int col, int row)
{
	// TODO: Wall suppport, more general solution

	Cell& atCell = map(col, row);
	Cell& belowCell = map(col, row - 1);
	Cell& rightCell = map(col + 1, row);

	
	float& up = atCell.upVelocity;
	float& down = belowCell.upVelocity;
	float& left = atCell.leftVelocity;
	float& right = rightCell.leftVelocity;

	float divergence = overRelaxation * CalculateDivergence(right, left, up, down);
	float balance = divergence / 4;

	left += balance;
	right -= balance;
	down += balance;
	up -= balance;
	
	// TODO: below might be wrong
	atCell.pressure += abs(balance) * ((atCell.density * gridSpacing) / timeStep);
}

Vec2 Weather::getAverageWindVelocityAt(int col, int row)
{
	const Cell& at = map(col, row);
	const Cell& below = map(col, row - 1);
	const Cell& right = map(col + 1, row);

	return (Vec2(at.leftVelocity + right.leftVelocity, at.upVelocity + below.upVelocity)) / 4.0f;
}

float Weather::getPressureAt(int col, int row)
{
	// TODO:
	return 0.0f;
}