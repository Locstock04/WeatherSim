#include "WeatherLineRenderer.h"+

#include "imgui.h"

#define STB_IMAGE_IMPLEMENTATION  
//#include "stb_image.h"
#include "Image.h"

#include <iostream>

using LocWeather::Weather;

void WeatherLineRenderer::DrawWindSideVelocities() const
{
	for (int c = 0; c < weather.map.cols; c++)
	{
		for (int r = 0; r < weather.map.rows; r++)
		{
			//if (r != c) { continue; }
			//Vec2 cellPos = Vec2(c, r) * distanceBetween2AdjacentCells;

			//lines->DrawCircle(Vec2(c, r), 0.3);

			Vec2 leftPos = Vec2((float)c - 0.5f, (float)r);
			Vec2 topPos = Vec2((float)c, (float)r + 0.5f);
			float left = weather.map.getConst(c, r).leftVelocity;
			float top = weather.map.getConst(c, r).upVelocity;
			Colour leftColour = ColourFromVector({ left, 0.0f });
			Colour topColour = ColourFromVector({ 0.0f, top });
			Vec2 leftLineEnd = leftPos + (Vec2(left, 0.0f) * scaleLineLengthViewBy);
			Vec2 topLineEnd = topPos + (Vec2(0.0f, top) * scaleLineLengthViewBy);

			DrawArrow(leftPos, leftLineEnd, leftColour);
			DrawArrow(topPos, topLineEnd, topColour);
		}
	}
}

void WeatherLineRenderer::DrawCentreWindVelocities() const
{
	for (int c = 0; c < weather.map.cols; c++)
	{
		for (int r = 0; r < weather.map.rows; r++)
		{
			Vec2 wind = LocVecToVec(weather.getVelocityAt(c, r));
			Vec2 pos((float)c, (float)r);
			DrawArrow(pos, pos + (wind * scaleLineLengthViewBy), ColourFromVector(wind));
		}
	}
}

void WeatherLineRenderer::GUI()
{
	if (ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::DragFloat("Scale Line Length", &scaleLineLengthViewBy, guiDragSpeed);
		ImGui::Checkbox("Draw Side Lines", &showSideLines);
		ImGui::Checkbox("Draw Centre Lines", &showCentreLines);
		ImGui::Checkbox("Draw Centre Circles", &showCentreCircles);
		ImGui::Checkbox("Draw Density (Cross)", &showDensity);
		ImGui::Checkbox("Draw Water", &showWater);
		ImGui::Checkbox("Draw Rain", &showRain);
		ImGui::Checkbox("Draw Temperature", &showTemperature);
		ImGui::Checkbox("Draw Streamlines", &showStreamlines);
		ImGui::DragInt("Streamlines Depth", &streamlineDepth, 0.5f, 0);
		ImGui::Checkbox("Update Simulation", &updatingWeather);
		//ImGui::Checkbox("Jacobi (non Gauss-Seidel)", &weather.jacobi);
		ImGui::Checkbox("Right click draw solid", &drawingSolid);
		ImGui::DragFloat("Line Tips Size", &lineArrowSize, guiDragSpeed);
		ImGui::DragFloat("Left click change multiplayer", &dragMultiplier, guiDragSpeed);
		ImGui::DragFloat("timeStep", &weather.timeStep);
		ImGui::DragFloat2("Wind Editor", &setAllWindTo.x, guiDragSpeed);
		if (ImGui::Button("Set Wind")) {
			weather.setAllWindTo(LocWeather::Vec2(setAllWindTo.x, setAllWindTo.y));
		}
	}
	ImGui::End();
}

Vec2 WeatherLineRenderer::worldToCellPos(float x, float y)
{
	return Vec2(round(x), round(y));
}

Colour WeatherLineRenderer::hslToRgb(float h, float s, float l)
{
	Colour rgb{ 0, 0, 0 };

	if (0 == s) {
		rgb.r = rgb.g = rgb.b = l; // achromatic
	}
	else {
		float q = l < 0.5 ? l * (1 + s) : l + s - l * s;
		float p = 2 * l - q;
		rgb.r = hueToRgb(p, q, h + 1.0f / 3.0f);
		rgb.g = hueToRgb(p, q, h);
		rgb.b = hueToRgb(p, q, h - 1.0f / 3.0f);
	}

	return rgb;
}

float WeatherLineRenderer::hueToRgb(float p, float q, float t)
{
	if (t < 0) {
		t += 1;
	}
	if (t > 1) {
		t -= 1;
	}
	if (t < 1.0f / 6.0f) {
		return p + (q - p) * 6 * t;
	}
	else if (t < 1.0f / 2.0f) {
		return q;
	}
	else if (t < 2.0 / 3.0f) {
		return p + (q - p) * (2.0f / 3.0f - t) * 6.0f;
	}
	else {
		return p;
	}

}

void WeatherLineRenderer::Draw() const
{
	if (showSideLines) {
		DrawWindSideVelocities();
	}
	if (showCentreLines) {
		DrawCentreWindVelocities();
	}	
	for (int c = 0; c < weather.map.cols; c++)
	{
		for (int r = 0; r < weather.map.rows; r++)
		{
			if (showCentreCircles) {
				lines->DrawCircle(Vec2((float)c, (float)r), 0.3f, ColourFromVector(LocVecToVec(weather.getVelocityAt(c, r))), 4);
			}
			if (showDensity) {
				float density = std::fminf(weather.map.getConst(c, r).density, 1.0f);
				lines->DrawCross(Vec2((float)c, (float)r), 0.3f, Colour(density, density, density));
			}
			if (showWater) {
				float water = weather.map.getConst(c, r).water;
				if (water > 0.0f) {
					lines->DrawCircle(Vec2((float)c, (float)r), 0.15f, { 0.0f, 0.0f, water }, 3);
				}
			}
			if (showRain) {
				float raining = weather.map.getConst(c, r).raining;
				if (raining) {
					lines->DrawCircle(Vec2((float)c, (float)r), 0.3f, { raining * 0.2f, raining * 0.2f, raining * 0.7f }, 3);
				}
			}
			if (showTemperature) {
				float hue = weather.TemperatureAsPercent(weather.map.getConst(c, r).currentTemperature) * -0.75f;
				lines->DrawCircle(Vec2((float)c, (float)r), 0.3f, hslToRgb(hue, 1.0f, 0.5f), 2);
			}
			if (showStreamlines) {
				Vec2 pos = Vec2((float)c, (float)r);
				Vec2 vel = LocVecToVec(weather.getVelocityAt(pos.x, pos.y));
				lines->AddPointToLine(pos, ColourFromVector(vel));
				for (size_t i = 0; i < streamlineDepth; i++)
				{
					pos += vel;
					lines->AddPointToLine(pos, ColourFromVector(vel));
					vel = LocVecToVec(weather.getVelocityAt(pos.x, pos.y));
				}
				lines->FinishLineStrip();
			}
		}
	}

}

void WeatherLineRenderer::DrawArrow(Vec2 begin, Vec2 end, Colour colour) const
{
	Vec2 normal = (end - begin).GetNormalised();


	lines->DrawLineSegment(begin, end, colour);

	lines->SetColour(colour);
	lines->AddPointToLine(end - (normal * lineArrowSize) + (normal.GetRotatedBy90()) * lineArrowSize);
	lines->AddPointToLine(end);
	lines->AddPointToLine(end - (normal * lineArrowSize) - (normal.GetRotatedBy90()) * lineArrowSize);
	lines->FinishLineStrip();
}

Colour WeatherLineRenderer::ColourFromVector(Vec2 vec)
{
	float hue = (-(atan2f(vec.x, -vec.y) / PI) + 1) / 2;
	float l = std::min(vec.GetMagnitude(), 0.5f);
	return hslToRgb(hue, 1.0f, l);
}

WeatherLineRenderer::WeatherLineRenderer()
{
	SetWater("Water.png");
	SetCloud("Cloud.png");
	SetAverageTemperature("Average Temperature");
}

void WeatherLineRenderer::Update(float delta)
{
	Draw();

	GUI();

	if (updatingWeather) {
		weather.Update();
	}

	if (rightMouseDown && leftMouseDown) {
		Vec2 cellPos = worldToCellPos(cursorPos.x, cursorPos.y);
		weather.map((int)cellPos.x, (int)cellPos.y).density += 1 * weather.timeStep;
	}

	else if (rightMouseDown) {
		Vec2 cellPos = worldToCellPos(cursorPos.x, cursorPos.y);
		weather.map((int)cellPos.x, (int)cellPos.y).nonSolid = !drawingSolid;
	}

	else if (leftMouseDown) {
		Vec2 pos = worldToCellPos(cursorPos.x, cursorPos.y);
		int x = (int)pos.x;
		int y = (int)pos.y;
		Weather::Cell& atCell = weather.map(x, y);
		Weather::Cell& rightCell = weather.map(x + 1, y);
		Weather::Cell& downCell = weather.map(x, y - 1);
		Vec2 newVelocity = (cursorPos - previousCursorPos) * dragMultiplier;
		atCell.leftVelocity += newVelocity.x;
		atCell.upVelocity += newVelocity.y;
		rightCell.leftVelocity += newVelocity.x;
		downCell.upVelocity += newVelocity.y;

		previousCursorPos = cursorPos;
	}
}

void WeatherLineRenderer::OnLeftClick()
{
	//weather.Update();

	previousCursorPos = cursorPos;
}

void WeatherLineRenderer::OnRightClick()
{
	//updatingWeather = !updatingWeather;
}

void WeatherLineRenderer::OnMiddleClick()
{


	//weather.Update();
	//updatingWeather = !updatingWeather;
}

void WeatherLineRenderer::OnMiddleRelease()
{

}

void WeatherLineRenderer::SetWater(std::string path)
{
	Image waterMap = Image(path, STBI_grey);
	if (waterMap.data == nullptr) { return; }

	if (waterMap.width != weather.map.cols || waterMap.height != weather.map.rows) {
		std::cout << "Warning: Mismatched image size with map\n";
		return;
	}

	for (int c = 0; c < weather.map.cols; c++)
	{
		for (int r = 0; r < weather.map.rows; r++)
		{
			weather.map(c, r).water = (waterMap.data[((weather.map.rows - 1 - r) * weather.map.rows + c) + 0] / 256.0f);
		}
	}
}

void WeatherLineRenderer::SetCloud(std::string path)
{
	Image cloudMap = Image(path, STBI_grey);
	if (cloudMap.data == nullptr) { return; }

	if (cloudMap.width != weather.map.cols || cloudMap.height != weather.map.rows) {
		std::cout << "Warning: Mismatched image size with map\n";
		return;
	}

	for (int c = 0; c < weather.map.cols; c++)
	{
		for (int r = 0; r < weather.map.rows; r++)
		{
			weather.map(c, r).density = cloudMap.data[((weather.map.rows - 1 - r) * weather.map.rows + c) + 0] / 256.0f;
		}
	}
}

void WeatherLineRenderer::SetSolid(std::string path)
{

}

void WeatherLineRenderer::SetAverageTemperature(std::string path)
{

}

Vec2 WeatherLineRenderer::LocVecToVec(LocWeather::Vec2 v) const
{
	return Vec2(v.x, v.y);
}
