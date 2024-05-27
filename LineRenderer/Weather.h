#pragma once

#include "Array2D.h"

#include "Maths.h"

class Weather
{
private:

	float timeStep = 0.1f;
	

public:

	class Cell {
	public:
		//Vec2 windVelocity;
		Vec2 leftVelocity;
		Vec2 topVelocity;

		float pressure = 0.0f;
		// Cloud
		float density = 0.0f;
	};

	Weather();
	void Update();

	// Should be greater than 1 and less than 2
	float overRelaxation = 1.9f;

	bool calculatingPressure = true;
	void ForceIncompressibilityAt(int col, int row);
	Vec2 getAverageWindVelocityAt(int col, int row);
	float getPressureAt(int col, int row);
	Array2D<Cell, 10, 10> map;
};