#pragma once
#include "Application.h"

#include "LineRenderer.h"

#include "Weather.h"

class WeatherLineRenderer : public Application
{
private:
	Weather weather;

	float scaleLineLengthViewBy = 0.4f;
	float distanceBetween2AdjacentCells = 1.0f;
	
	float dragMultiplier = 1.0f;

	float lineArrowSize = 0.1f;
	bool showSideLines = true;
	bool showCentreLines = true;
	bool showCentreCircles = true;

	float guiDragSpeed = 0.1f;

	bool drawPressure = true;
	
	Vec2 previousCursorPos;

	void DrawWindSideVelocities();
	void DrawCentreWindVelocities();

	void DrawArrow(Vec2 begin, Vec2 end, Colour colour);
	static Colour ColourFromVector(Vec2 vec);

	void GUI();
public:


	WeatherLineRenderer();

	void Update(float delta) override;

	void OnLeftClick() override;

	void OnRightClick() override;

};

