#pragma once

#include "Array2D.h"

#include "Maths.h"

#include <string>

#include "Image.h"

constexpr int MAP_SIZE = 50;

class Weather
{
private:
	
	void ForceIncompressibilityAt(int col, int row);
	void Projection();
	void AdvectionOfField();

	void WaterCycle();


	//float VelocityAt()
	float SampleField(int c, int r, float distanceFromDown, float distanceFromLeft, size_t variableOffset) const;
	
	float GetWeightedValue(float upLeft, float upRight, float downLeft, float downRight, float distanceFromDown, float distanceFromLeft) const;

	float getXVelocityAt(float x, float y) const;
	float getYVelocityAt(float x, float y) const;

	void AdvectionOfClouds();
	int projectionIterations = 50;
public:

	//TODO: Could be more performant to store all the cell contents indivdually together, as in store all the left velocitys in a single object, and the rest their own.
	class Cell {
	public:
		// These are the horizontal and vertical velocties at the left and up side of the cell respectively
		float leftVelocity = 0.0f; // TODO: Should be right instead, as when this is positive it is 'right'
		float upVelocity = 0.0f;
				
		// Cloud
		float density = 0.0f;

		float water = 0.0f;

		float raining = 0.0f;


		bool nonSolid = true;

		//TODO: Pressure

		float averageTemperature = 10.0f;
		float currentTemperature = 10.0f;

	};

	Array2D<Cell, MAP_SIZE, MAP_SIZE> map;
	
	Weather();
	void Update();


	void setAllWindTo(Vec2 wind);
	void setBordersTo(bool solid = true);

	float stopRainPercent = 0.1f;

	float timeStep = 1.0f / 60.0f;

	float evaporationRate = 1.0f / 20.0f;
	float percipitationRate = 1.0f / 2.0f;

	// Should be greater than 1 and less than 2
	float overRelaxation = 1.9f;

	float maxCloudDensity = 1.0f;
	float startRainDensity = 0.9f;
	float stopRainDensity = 0.01f;

	float minTemperature = -30.0f;
	float maxTemperature = 50.0f;

	float temperatureStickToAveragePercent = 0.5f;

	float TemperatureAsPercent(float temperature) const;

	bool calculatingPressure = true;
	Vec2 getVelocityAt(int col, int row) const;
	Vec2 getVelocityAt(float x, float y) const;

	float getPressureAt(int col, int row) const;

};