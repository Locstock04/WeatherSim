#pragma once

#include "Array2D.h"

#include "Maths.h"

constexpr auto MAP_SIZE = 10;

class Weather
{
private:

	float timeStep = 1.0f / 60.0f;
	
	void ForceIncompressibilityAt(int col, int row);
	void Projection();
	void Advection();
	int projectionIterations = 10;

	float const gridSpacing = 1.0f;

	Vec2 CalculateDivergence(Vec2 right, Vec2 left, Vec2 up, Vec2 down);

public:

	class Cell {
	public:
		//Vec2 windVelocity;
		Vec2 leftVelocity;
		Vec2 upVelocity;

		float pressure = 0.0f;
		// Cloud
		float density = 0.0f;
	};

	Array2D<Cell, MAP_SIZE, MAP_SIZE> map;
	
	Weather();
	void Update();

	float getDeltaTime() const;


	// Should be greater than 1 and less than 2
	float overRelaxation = 1.9f;

	bool calculatingPressure = true;
	Vec2 getAverageWindVelocityAt(int col, int row);
	float getPressureAt(int col, int row);

	void RandomiseMapVelocities();


};