#pragma once
#include "Application.h"

#include "LineRenderer.h"

#include "Weather.h"

using LocWeather::Weather;

class WeatherLineRenderer : public Application
{
private:
	Weather weather = Weather(50, 50);

	float scaleLineLengthViewBy = 0.4f;
	
	float dragMultiplier = 10.0f;

	float lineArrowSize = 0.1f;
	bool showSideLines = false;
	bool showCentreLines = false;
	bool showCentreCircles = false;
	bool showDensity = false;
	bool showWater = false;
	bool showRain = false;
	bool showTemperature = false;
	int streamlineDepth = 2;
	bool showStreamlines = false;

	Vec2 setAllWindTo = { 0, 0 };

	float guiDragSpeed = 0.1f;

	bool updatingWeather = true;
	
	bool drawingSolid = true;

	Vec2 previousCursorPos;

	void DrawWindSideVelocities() const;
	void DrawCentreWindVelocities() const;

	void DrawArrow(Vec2 begin, Vec2 end, Colour colour) const;
	static Colour ColourFromVector(Vec2 vec);

	void GUI();

	static Vec2 worldToCellPos(float x, float y);

	static Colour hslToRgb(float h, float s, float l);
	static float hueToRgb(float p, float q, float t);

	void Draw() const;

	void SetWater(std::string path);
	void SetCloud(std::string path);
	void SetSolid(std::string path);
	void SetAverageTemperature(std::string path);

	Vec2 LocVecToVec(LocWeather::Vec2 v) const;

public:


	WeatherLineRenderer();

	void Update(float delta) override;

	void OnLeftClick() override;

	void OnRightClick() override;

	void OnMiddleClick() override;
	void OnMiddleRelease() override;
};

