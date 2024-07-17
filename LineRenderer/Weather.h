#pragma once

#include "Array2D.h"

#include "Maths.h"

constexpr auto MAP_SIZE = 50;

class Weather
{
private:


public: // TODO: remove public
	float timeStep = 1.0f / 60.0f;
	
	void ForceIncompressibilityAt(int col, int row);
	void Projection();
	void AdvectionOfField();
	void AdvectionOfClouds();
	int projectionIterations = 50;

	float const gridSpacing = 1.0f;

	float CalculateDivergence(float right, float left, float up, float down);

public:

	class Cell {
	public:
		//Vec2 windVelocity;
		float leftVelocity; // TODO: Should be right instead, as when this is positive it is 'right'
		float upVelocity;

		//// TODO: this is a temporary attempt at a solution, remove
		//float maxVelocityInSingleDirection = 2.0f;
		
		float pressure = 0.0f;
		
		// Cloud
		float density = 0.0f;

		bool solid = false;
	};

	Array2D<Cell, MAP_SIZE, MAP_SIZE> map;
	
	Weather();
	void Update();

	float getDeltaTime() const;

	void setAllWindTo(Vec2 wind);
	void setBordersTo(Vec2 wind);

	// Should be greater than 1 and less than 2
	float overRelaxation = 1.9f;

	bool calculatingPressure = true;
	Vec2 getAverageWindVelocityAt(int col, int row);
	float getPressureAt(int col, int row);

};