#pragma once

#include "Maths.h"
#include <vector>
#include "Graphics.h"
#include "Colour.h"
#include <string>

class LineRenderer
{
private:
	std::vector<Vec2> positions;
	std::vector<Colour> colours;
	bool initialised = false;

	Colour currentColour = { 1.0f,1.0f,1.0f };

	Vec2 firstPos;
	Vec2 lastPos;
	Colour firstColour = { 0.0f,0.0f,0.0f };
	Colour lastColour = { 0.0f,0.0f,0.0f };
	bool lineActive = false;

	GLuint positionBufferID = 0;
	GLuint colourBufferID = 0;

public:
	LineRenderer() = default;
	~LineRenderer();
	LineRenderer(const LineRenderer&) = delete;
	LineRenderer(const LineRenderer&&) = delete;
	const LineRenderer& operator=(const LineRenderer&) = delete;
	const LineRenderer& operator=(LineRenderer&&) = delete;

	void Initialise();

	void SetColour(Colour colour);

	void DrawLineSegment(Vec2 start, Vec2 end);
	void DrawLineSegment(Vec2 start, Vec2 end, Colour colour);

	void FinishLineStrip();
	void FinishLineLoop();

	void AddPointToLine(Vec2 point);
	void AddPointToLine(Vec2 point, Colour colour);

	void DrawCross(Vec2 centre, float size);
	void DrawCross(Vec2 centre, float size, Colour colour);

	void DrawCircle(Vec2 centre, float size, int segmentCount = 24);
	void DrawCircle(Vec2 centre, float size, Colour colour, int segmentCount = 24);

	float DrawText(std::string text, Vec2 pos, float size);
	float DrawText(std::string text, Vec2 pos, float size, Colour colour);
	float DrawChar(char character, Vec2 pos, float size);
	float DrawChar(char character, Vec2 pos, float size, Colour colour);

	void UpdateFrame();

	void Clear();
	void Compile();
	void Draw();


private:
	static std::vector<Vec2> GetGlyph(char character);
};