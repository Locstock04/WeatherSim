#pragma once
#include "Application.h"

#include "LineRenderer.h"

#include "Weather.h"

#include "Image.h"


class WeatherLineRenderer : public Application
{
private:
	Weather weather;

	float scaleLineLengthViewBy = 0.4f;
	float distanceBetween2AdjacentCells = 1.0f;
	
	float dragMultiplier = 1.0f;

	float lineArrowSize = 0.1f;
	bool showSideLines = false;
	bool showCentreLines = true;
	bool showCentreCircles = true;
	bool showDensity = false;
	bool showWater = false;

	Vec2 setAllWindTo = { 0, 0 };

	float guiDragSpeed = 0.1f;

	bool updatingWeather = false;
	
	bool drawingSolid = true;

	Vec2 previousCursorPos;

	Image* image;

	void DrawWindSideVelocities() const;
	void DrawCentreWindVelocities() const;

	void DrawArrow(Vec2 begin, Vec2 end, Colour colour) const;
	static Colour ColourFromVector(Vec2 vec);

	void GUI();

	static Vec2 worldToCellPos(float x, float y);

	static Colour hslToRgb(float h, float s, float l);
	static float hueToRgb(float p, float q, float t);

	void Draw() const;

public:


	WeatherLineRenderer();

	void Update(float delta) override;

	void OnLeftClick() override;

	void OnRightClick() override;

	void OnMiddleClick() override;
	void OnMiddleRelease() override;
};

