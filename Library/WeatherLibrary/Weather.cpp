#include "Weather.h"

#include <iostream>

#include <math.h>

using LocWeather::Weather;

void Weather::Projection()
{
	for (int i = 0; i < projectionIterations; i++)
	{
		for (int col = 0; col < map.cols; col++)
		{
			for (int row = 0; row < map.rows; row++)
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

void Weather::WaterCycle()
{
	float evaporationChange = evaporationRate * timeStep;
	float percipitationChange = percipitationRate * timeStep;
	for (Cell& cell : map.data)
	{
		// Evaporation
		if (cell.water > 0.0f) {
			cell.density = std::min(cell.density + evaporationChange * cell.water, maxCloudDensity);
		}
		
		// Percipitation
		if (cell.raining && cell.density < stopRainDensity) {
			cell.raining = std::max(0.0f, cell.raining - percipitationChange);
		}
		else if (cell.density > startRainDensity) {
			cell.raining = 1.0f;
		}

		if (cell.raining <= stopRainPercent) {
			cell.raining = 0.0f;
		}
		
		if (cell.raining) {
			cell.density = std::max(cell.density - (percipitationChange * cell.raining), 0.0f);
		}
	}
}


// TODO: This honestly looks worse then the previously used macro
float Weather::SampleField(int c, int r, float distanceFromDown, float distanceFromLeft, size_t variableOffset) const
{
	const float upLeft =    *((float*)( (char*)&(map.getConst(c, r))         + variableOffset));
	const float upRight =   *((float*)( (char*)&(map.getConst(c + 1, r))     + variableOffset));
	const float downLeft =  *((float*)( (char*)&(map.getConst(c, r - 1))     + variableOffset));
	const float downRight = *((float*)( (char*)&(map.getConst(c + 1, r - 1)) + variableOffset));
	                                                                                 
	return GetWeightedValue(upLeft, upRight, downLeft, downRight, distanceFromDown, distanceFromLeft);
}

float Weather::GetWeightedValue(float upLeft, float upRight, float downLeft, float downRight, float distanceFromDown, float distanceFromLeft) const
{
	float leftWeight = 1 - distanceFromLeft;
	float rightWeight = distanceFromLeft;
	float upWeight = distanceFromDown;
	float downWeight = 1 - distanceFromDown;

	return upWeight *   (leftWeight * upLeft   + rightWeight * upRight) +
		   downWeight * (leftWeight * downLeft + rightWeight * downRight);
}

LocWeather::Vec2 Weather::getVelocityAt(float x, float y) const
{
	return { getXVelocityAt(x, y), getYVelocityAt(x, y) };
}

float Weather::getXVelocityAt(float x, float y) const
{
	int cellC = (int)round(x);
	int cellR = (int)ceil(y);
	float distanceFromDown = y - (cellR - 1.0f);
	float distanceFromLeft = x - (cellC - 0.5f);

	return SampleField(cellC, cellR, distanceFromDown, distanceFromLeft, offsetof(Cell, leftVelocity));
}

float Weather::getYVelocityAt(float x, float y) const
{
	int cellC = (int)floor(x);
	int cellR = (int)round(y);

	float distanceFromDown = y - (cellR - 0.5f);
	float distanceFromLeft = x - (cellC);

	return SampleField(cellC, cellR, distanceFromDown, distanceFromLeft, offsetof(Cell, upVelocity));
}


void Weather::AdvectionOfClouds()
{
	for (int c = 0; c < map.cols; c++)
	{
		for (int r = 0; r < map.rows; r++)
		{
			Cell& at = map(c, r);
			if (!at.nonSolid) {
				continue;
			}
			Vec2 pos = { (float)c, (float)r };
			Vec2 vel = getVelocityAt(c, r);

			Vec2 prevPos = pos - (vel * timeStep);
			int prevC = (int)floor(prevPos.x);
			int prevR = (int)ceil(prevPos.y);

			float distanceFromDown = prevPos.y - (prevR - 1.0f);
			float distanceFromLeft = prevPos.x - (prevC);



			at.density = SampleField(prevC, prevR, distanceFromDown, distanceFromLeft, offsetof(Cell, density));
			
			at.raining = SampleField(prevC, prevR, distanceFromDown, distanceFromLeft, offsetof(Cell, raining));

			at.currentTemperature = 
				(SampleField(prevC, prevR, distanceFromDown, distanceFromLeft, offsetof(Cell, currentTemperature)) * (1 - temperatureStickToAveragePercent))
			  + (SampleField(prevC, prevR, distanceFromDown, distanceFromLeft, offsetof(Cell, averageTemperature)) * (    temperatureStickToAveragePercent));
		}	
	}
}


Weather::Weather()
{
}

void Weather::Update()
{
	WaterCycle();

	Projection();

	AdvectionOfField();
	AdvectionOfClouds();
}

void Weather::setAllWindTo(Vec2 wind)
{
	for (auto i = map.Begin(); i != map.End(); i++)
	{
		i->leftVelocity = wind.x;
		i->upVelocity = wind.y;
	}
}

void Weather::setBordersTo(bool solid)
{
	for (int c = 0; c < map.cols; c++)
	{
		for (int r = 0; r < map.rows; r++)
		{
			if (c != 0 && c != map.cols - 1 && r != 0 && r != map.rows - 1) { continue; }

			Cell& cell = map(c, r);
			cell.nonSolid = !solid;
		}
	}

}

void Weather::ForceIncompressibilityAt(int col, int row)
{
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
	//atCell.pressure += abs(balance) * ((atCell.density * 1) / timeStep);
}

float Weather::TemperatureAsPercent(float temperature) const
{
	return (temperature - minTemperature) / (maxTemperature - minTemperature);
}

LocWeather::Vec2 Weather::getVelocityAt(int col, int row) const
{
	const Cell& at = map.getConst(col, row);
	const Cell& below = map.getConst(col, row - 1);
	const Cell& right = map.getConst(col + 1, row);

	return (Vec2(at.leftVelocity + right.leftVelocity, at.upVelocity + below.upVelocity)) / 4.0f;
}

float Weather::getPressureAt(int col, int row) const
{
	// TODO:
	return 0.0f;
}