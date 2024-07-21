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
			}
		}
	}
}

void Weather::AdvectionOfField()
{
	for (int c = 0; c < map.cols; c++)
	{
		for (int r = 0; r < map.rows; r++)
		{
			Cell& at = map(c, r);
			if (!at.nonSolid) { continue; }

			const Cell& leftCell = map(c - 1, r);
			const Cell& rightCell = map(c + 1, r);
			const Cell& downCell = map(c, r - 1);
			const Cell& downLeftCell = map(c - 1, r - 1);
			const Cell& upCell = map(c, r + 1);
			const Cell& upRightCell = map(c + 1, r + 1);

			Vec2 xPos = { c - 0.5f, (float)r };
			float xPosYVel = (at.upVelocity + leftCell.upVelocity + downLeftCell.upVelocity + downCell.upVelocity) / 4;
			Vec2 xPosVel = { at.leftVelocity, xPosYVel };
			Vec2 xPreviousPos = xPos - (xPosVel * timeStep);

			Vec2 yPos = { (float)c, r + 0.5f };
			float yPosXVel = (at.leftVelocity + rightCell.leftVelocity + upCell.leftVelocity + upRightCell.leftVelocity) / 4;
			Vec2 yPosVel = { yPosXVel, at.upVelocity };
			Vec2 yPreviousPos = yPos - (yPosVel * timeStep);

			at.leftVelocity = getXVelocityAt(xPreviousPos.x, xPreviousPos.y);
			at.upVelocity = getYVelocityAt(yPreviousPos.x, yPreviousPos.y);
		}
	}
}

// TODO: This honestly looks worse then the previously used macro
float Weather::SampleField(int c, int r, float distanceFromDown, float distanceFromLeft, size_t variableOffset)
{
	const float upLeft =    *((float*)( (char*)&(map(c, r))         + variableOffset));
	const float upRight =   *((float*)( (char*)&(map(c + 1, r))     + variableOffset));
	const float downLeft =  *((float*)( (char*)&(map(c, r - 1))     + variableOffset));
	const float downRight = *((float*)( (char*)&(map(c + 1, r - 1)) + variableOffset));
	                                                                                 
	return GetWeightedValue(upLeft, upRight, downLeft, downRight, distanceFromDown, distanceFromLeft);
}

float Weather::GetWeightedValue(float upLeft, float upRight, float downLeft, float downRight, float distanceFromDown, float distanceFromLeft)
{
	float leftWeight = 1 - distanceFromLeft;
	float rightWeight = distanceFromLeft;
	float upWeight = distanceFromDown;
	float downWeight = 1 - distanceFromDown;

	return upWeight *   (leftWeight * upLeft   + rightWeight * upRight) +
		   downWeight * (leftWeight * downLeft + rightWeight * downRight);
}

float Weather::getXVelocityAt(float x, float y)
{
	int cellC = round(x);
	int cellR = ceil(y);
	float distanceFromDown = y - (cellR - 1.0f);
	float distanceFromLeft = x - (cellC - 0.5f);

	return SampleField(cellC, cellR, distanceFromDown, distanceFromLeft, offsetof(Cell, leftVelocity));
}

float Weather::getYVelocityAt(float x, float y)
{
	int cellC = floor(x);
	int cellR = round(y);

	float distanceFromDown = y - (cellR - 0.5f);
	float distanceFromLeft = x - (cellC);

	return SampleField(cellC, cellR, distanceFromDown, distanceFromLeft, offsetof(Cell, upVelocity));
}

Vec2 Weather::getVelocityAt(float x, float y)
{
	return { getXVelocityAt(x, y), getYVelocityAt(x, y) };
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