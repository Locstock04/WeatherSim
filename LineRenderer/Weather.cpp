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

//#define AdvectSingleVelocity(MAIN_VEL, OTHER_VEL, POSX, POSY)                                               \
//{                                                                                                           \
//	Vec2 pos = { POSX, POSY };                                                                              \
//	float otherVel = (at.OTHER_VEL + leftCell.OTHER_VEL + downLeftCell.OTHER_VEL + downCell.OTHER_VEL) / 4; \
//                                                                                                            \
//	Vec2 vel = { at.MAIN_VEL, otherVel };                                                                   \
//	Vec2 previousPos = pos - (vel * timeStep);                                                              \
//	Vec2 previousCellPos = { round(previousPos.x), round(previousPos.y) };                                  \
//                                                                                                            \
//	const float upLeft = map(previousCellPos.x, previousCellPos.y + 1).MAIN_VEL;                            \
//	const float upRight = map(previousCellPos.x + 1, previousCellPos.y + 1).MAIN_VEL;                       \
//	const float downLeft = map(previousCellPos.x, previousCellPos.y).MAIN_VEL;                              \
//	const float downRight = map(previousCellPos.x + 1, previousCellPos.y).MAIN_VEL;                         \
//                                                                                                            \
//	float yDistanceFromDown = previousPos.y - (previousCellPos.y - (gridSpacing / 2));                      \
//	float xDistanceFromLeft = previousPos.x - (previousCellPos.x - (gridSpacing / 2));                      \
//                                                                                                            \
//	float leftWeight = 1 - (xDistanceFromLeft / gridSpacing);                                               \
//	float rightWeight = (xDistanceFromLeft / gridSpacing);                                                  \
//	float upWeight = (yDistanceFromDown / gridSpacing);                                                     \
//	float downWeight = 1 - (yDistanceFromDown / gridSpacing);                                               \
//                                                                                                            \
//	float previousPosVel =                                                                                  \
//	upWeight * (leftWeight * upLeft + rightWeight * upRight) +                                              \
//	downWeight * (leftWeight * downLeft + rightWeight * downRight);                                         \
//                                                                                                            \
//	at.MAIN_VEL = previousPosVel;                                                                           \
//}

#define AdjectSingleVelocity() \
{ \
	 \
}


void Weather::AdvectionOfField()
{
	// TODO: don't remove this comment till function cleaned up

	Array2D<Cell, MAP_SIZE, MAP_SIZE> newMap = map;

	for (int c = 0; c < map.cols; c++)
	{
		for (int r = 0; r < map.rows; r++)
		{

			// Work out velocity at spot (velocity spot)
			// Figure out what the prevoius position would have been
			//


			//Vec2 wind = getAverageWindVelocityAt(col, row);
			Cell& at = map(c, r);
			if (!at.nonSolid) {
				continue;
			}

			const Cell& leftCell = map(c - 1, r);
			const Cell& rightCell = map(c + 1, r);
			const Cell& downCell = map(c, r - 1);
			const Cell& downLeftCell = map(c - 1, r - 1);
			const Cell& upCell = map(c, r + 1);
			const Cell& upRightCell = map(c + 1, r + 1);

			float previousPosVelX;
			float previousPosVelY;
			// TODO: please put these into function
			//AdvectSingleVelocity(leftVelocity, upVelocity, c - (gridSpacing / 2.0f), (float)r);
			//AdvectSingleVelocity(upVelocity, leftVelocity, (float)c, r + (gridSpacing / 2.0f));


			// x, leftVelocity
			{
				// Pos should be the left velocity spot of the grid
				Vec2 pos = { c - 0.5f, (float)r };
				float yVel = (at.upVelocity + leftCell.upVelocity + downLeftCell.upVelocity + downCell.upVelocity) / 4;
				Vec2 vel = { at.leftVelocity, yVel };

				Vec2 previousPos = pos - (vel * timeStep);

				int previousCellC = round(previousPos.x);
				int previousCellR = ceil(previousPos.y);

				const float upLeft = map(previousCellC, previousCellR).leftVelocity;
				const float upRight = map(previousCellC + 1, previousCellR).leftVelocity;
				const float downLeft = map(previousCellC, previousCellR - 1).leftVelocity;
				const float downRight = map(previousCellC + 1, previousCellR - 1).leftVelocity;

				// Would be positive
				float distanceFromDown = previousPos.y - (previousCellR - 1.0f);
				float distanceFromLeft = previousPos.x - (previousCellC - 0.5f);

				float leftWeight = 1 - distanceFromLeft;
				float rightWeight = distanceFromLeft;
				float upWeight = distanceFromDown;
				float downWeight = 1 - distanceFromDown;

				previousPosVelX =
					upWeight * (leftWeight * upLeft + rightWeight * upRight) +
					downWeight * (leftWeight * downLeft + rightWeight * downRight);

				if (r == c && (r == 1 || r == 0)) {
					//std::cout << "\n";
					std::cout <<
						"\n     CellPos: " << c << ", " << r <<
						"\n         pos: " << pos.x << ", " << pos.y <<
						"\nPrevious pos: " << previousPos.x << ", " << previousPos.y <<
						"\nPreviousCpos: " << previousCellC << ", " << previousCellR <<
						"\nDis From D  : " << distanceFromDown <<
						"\nDis From L  : " << distanceFromLeft <<
						"\nUp Weight   : " << upWeight <<
						"\nDown Weight : " << downWeight <<
						"\nLeft Weight : " << leftWeight <<
						"\nRight Weight: " << rightWeight <<
						"\nPrevPos VelX: " << previousPosVelX <<

						"\n\n";
				}
			}


			// y, upVelocity
			
			{
				Vec2 pos = { (float)c, r + 0.5f };
				float xVel = (at.leftVelocity + rightCell.leftVelocity + upCell.leftVelocity + upRightCell.leftVelocity) / 4;
				Vec2 vel = { xVel, at.upVelocity };

				Vec2 previousPos = pos - (vel * timeStep);

				int previousCellC = floor(previousPos.x);
				int previousCellR = round(previousPos.y);

				const float upLeft = map(previousCellC, previousCellR).upVelocity;
				const float upRight = map(previousCellC + 1, previousCellR).upVelocity;
				const float downLeft = map(previousCellC, previousCellR - 1).upVelocity;
				const float downRight = map(previousCellC + 1, previousCellR - 1).upVelocity;

				float distanceFromDown = previousPos.y - (previousCellR - 0.5f);
				float distanceFromLeft = previousPos.x - (previousCellC);

				float leftWeight = 1 - distanceFromLeft;
				float rightWeight = distanceFromLeft;
				float upWeight = distanceFromDown;
				float downWeight = 1 - distanceFromDown;

				previousPosVelY =
					upWeight * (leftWeight * upLeft + rightWeight * upRight) +
					downWeight * (leftWeight * downLeft + rightWeight * downRight);


				if (r == c && r == 1) {
					//std::cout << "\n";
					std::cout <<
						"\n     CellPos: " << c << ", " << r <<
						"\n         pos: " << pos.x << ", " << pos.y <<
						"\nPrevious pos: " << previousPos.x << ", " << previousPos.y <<
						"\nPreviousCpos: " << previousCellC << ", " << previousCellR <<
						"\nDis From D  : " << distanceFromDown <<
						"\nDis From L  : " << distanceFromLeft <<
						"\nUp Weight   : " << upWeight <<
						"\nDown Weight : " << downWeight <<
						"\nLeft Weight : " << leftWeight <<
						"\nRight Weight: " << rightWeight <<
						"\nPrevPos VelY: " << previousPosVelY <<

						"\n\n";
				}
			}

			if (jacobi) {
				at = newMap(c, r);
			}
			at.leftVelocity = previousPosVelX;
			at.upVelocity = previousPosVelY;
		}
	}
	if (jacobi) {
		map = newMap;
	}
}

void Weather::AdvectionOfClouds()
{
}


Weather::Weather()
{
	setBordersTo();
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

	//Projection();
	//setBordersTo({ 0.0f, 0.0f });

	Projection();
//
	AdvectionOfField();

	//setBordersTo({ 0.0f, 0.0f });
	
	//Projection();

	//setBordersTo({ 0.0f, 0.0f });
	//total /= map.getSize();
	//std::cout << total.x << ", " << total.y << "\n";

	//std::cout << (total, Vec2(0, 0)).GetMagnitude() << "\n";
}

float Weather::getDeltaTime() const
{
	return timeStep;
}

void Weather::setAllWindTo(Vec2 wind)
{
	for (auto i = map.Begin(); i != map.End(); i++)
	{
		i->leftVelocity = wind.x;
		i->upVelocity = wind.y;
	}
}

void Weather::setBordersTo()
{
	for (int c = 0; c < map.cols; c++)
	{
		for (int r = 0; r < map.rows; r++)
		{
			if (c != 0 && c != map.cols - 1 && r != 0 && r != map.rows - 1) { continue; }

			Cell& cell = map(c, r);
			cell.density = 1.0f;
			cell.nonSolid = false;
		}
	}

}

void Weather::ForceIncompressibilityAt(int col, int row)
{
	// TODO: Wall suppport, more general solution

	Cell& atCell = map(col, row);
	if (!atCell.nonSolid) { return; }
	Cell& downCell = map(col, row - 1);
	Cell& rightCell = map(col + 1, row);
	const Cell& upCell = map(col, row + 1);
	const Cell& leftCell = map(col - 1, row);
	
	int sides = upCell.nonSolid + downCell.nonSolid + leftCell.nonSolid + rightCell.nonSolid;
	if (sides == 0) {
		return;
	}

	float& up = atCell.upVelocity;
	float& down = downCell.upVelocity;
	float& left = atCell.leftVelocity;
	float& right = rightCell.leftVelocity;

	float divergence = overRelaxation * (right - left + up - down);
	float balance = divergence / sides;

	left += balance * leftCell.nonSolid;
	right -= balance * rightCell.nonSolid;
	down += balance * downCell.nonSolid;
	up -= balance * upCell.nonSolid;
	
	// TODO: below might be wrong
	atCell.pressure += abs(balance) * ((atCell.density * 1) / timeStep);
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