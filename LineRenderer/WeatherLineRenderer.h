#pragma once
#include "Application.h"

#include "Weather.h"

class WeatherLinerRenderer : public Application
{
private:
	Weather weather;

	float scaleLineLengthViewBy = 0.4f;
	float distanceBetween2AdjacentCells = 1.0f;
	
	float dragMultiplier = 1.0f;

	float lineArrowSize = 0.1f;
	bool showLineArrows = true;

	float guiDragSpeed = 0.1f;

	bool drawWindVelocity = true;
	bool drawPressure = true;
	
	Vec2 previousCursorPos;

	void DrawWindVelocities();
	void GUI();
public:
	WeatherLinerRenderer();

	void Update(float delta) override;

	void OnLeftClick() override;

	void OnRightClick() override;

};

