#include "WeatherLineRenderer.h"+


#include "imgui.h"


void WeatherLineRenderer::DrawWindSideVelocities()
{
	for (size_t c = 0; c < weather.map.cols; c++)
	{
		for (size_t r = 0; r < weather.map.rows; r++)
		{
			//if (r != c) { continue; }
			//Vec2 cellPos = Vec2(c, r) * distanceBetween2AdjacentCells;

			//lines->DrawCircle(Vec2(c, r), 0.3);

			Vec2 leftPos = Vec2(c - 0.5f, r);
			Vec2 topPos = Vec2(c, r + 0.5f);
			float left = weather.map(c, r).leftVelocity;
			float top = weather.map(c, r).upVelocity;
			Colour leftColour = ColourFromVector({ left, 0.0f });
			Colour topColour = ColourFromVector({ 0.0f, top });
			Vec2 leftLineEnd = leftPos + (Vec2(left, 0.0f) * scaleLineLengthViewBy);
			Vec2 topLineEnd = topPos + (Vec2(0.0f, top) * scaleLineLengthViewBy);

			DrawArrow(leftPos, leftLineEnd, leftColour);
			DrawArrow(topPos, topLineEnd, topColour);
		}
	}
}

void WeatherLineRenderer::DrawCentreWindVelocities()
{
	for (size_t c = 0; c < weather.map.cols; c++)
	{
		for (size_t r = 0; r < weather.map.rows; r++)
		{
			Vec2 wind = weather.getAverageWindVelocityAt(c, r);
			Vec2 pos(c, r);
			DrawArrow(pos, pos + wind, ColourFromVector(wind));
		}
	}
}

void WeatherLineRenderer::GUI()
{
	if (ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::DragFloat("Scale Line Length", &scaleLineLengthViewBy, guiDragSpeed);
		ImGui::DragFloat("Between Line Distance", &distanceBetween2AdjacentCells, guiDragSpeed);
		ImGui::Checkbox("Draw Side Lines", &showSideLines);
		ImGui::Checkbox("Draw Centre Lines", &showCentreLines);
		ImGui::Checkbox("Draw Centre Circles", &showCentreCircles);
		ImGui::Checkbox("Draw Density (Cross)", &showDensity);
		ImGui::Checkbox("Draw Pressure", &showPressure);
		ImGui::Checkbox("Update Simulation", &updatingWeather);
		ImGui::DragFloat("Line Tips Size", &lineArrowSize, guiDragSpeed);
		ImGui::DragFloat("Left click change multiplayer", &dragMultiplier, guiDragSpeed);
		ImGui::DragFloat("timeStep", &weather.timeStep);
		ImGui::DragFloat2("Wind Editor", &setAllWindTo.x, guiDragSpeed);
		if (ImGui::Button("Set Wind")) {
			weather.setAllWindTo(setAllWindTo);
		}

		//float tempColour[4];
		//ImGui::ColorPicker4("Temp colour", tempColour);
	}
	ImGui::End();
}

Vec2 WeatherLineRenderer::worldToCellPos(float x, float y)
{
	return Vec2(round(x), round(y));
}

void WeatherLineRenderer::DrawArrow(Vec2 begin, Vec2 end, Colour colour)
{
	Vec2 normal = (end - begin).GetNormalised();


	lines->DrawLineSegment(begin, end, colour);

	lines->SetColour(colour);
	lines->AddPointToLine(end - (normal * lineArrowSize) + (normal.GetRotatedBy90()) * lineArrowSize);
	lines->AddPointToLine(end);
	lines->AddPointToLine(end - (normal * lineArrowSize) - (normal.GetRotatedBy90()) * lineArrowSize);
	lines->FinishLineStrip();

	//lines->AddPointToLine(leftLineEnd - (leftWindNormal * lineArrowSize) + (leftWindNormal.GetRotatedBy90()) * lineArrowSize);
	//lines->AddPointToLine(leftLineEnd);
	//lines->AddPointToLine(leftLineEnd - (leftWindNormal * lineArrowSize) - (leftWindNormal.GetRotatedBy90()) * lineArrowSize);
}

Colour WeatherLineRenderer::ColourFromVector(Vec2 vec)
{
	return Colour(vec.x / 2.0f + 0.5f, vec.y / 2.0f + 0.5f, -vec.x / 2.0f + 0.5f);
}

WeatherLineRenderer::WeatherLineRenderer()
{
	image = new Image("test.png", STBI_grey);
	//updatingWeather = !updatingWeather;
}

void WeatherLineRenderer::Update(float delta)
{
	if (showSideLines) {
		DrawWindSideVelocities();
	}
	if (showCentreLines) {
		DrawCentreWindVelocities();
	}
	if (showCentreCircles) {
		for (size_t c = 0; c < weather.map.cols; c++)
		{
			for (size_t r = 0; r < weather.map.rows; r++)
			{
				lines->DrawCircle(Vec2(c, r), 0.3, ColourFromVector(weather.getAverageWindVelocityAt(c, r)), 4);
			}
		}
	}
	if (showDensity) {
		for (size_t c = 0; c < weather.map.cols; c++)
		{
			for (size_t r = 0; r < weather.map.rows; r++)
			{
				float density = weather.map(c, r).density;
				lines->DrawCross(Vec2(c, r), 0.3, Colour(density, density, density));
			}
		}
	}
	
	GUI();

	if (updatingWeather) {
		weather.Update();
	}

	if (rightMouseDown) {
		Vec2 cellPos = worldToCellPos(cursorPos.x, cursorPos.y);
		weather.map(cellPos.x, cellPos.y).density += 5 * weather.getDeltaTime();
	}

	if (leftMouseDown) {
		Vec2 intPos = worldToCellPos(cursorPos.x, cursorPos.y);
		Weather::Cell& atCell = weather.map(intPos.x, intPos.y);
		Weather::Cell& rightCell = weather.map(intPos.x + 1, intPos.y);
		Weather::Cell& downCell = weather.map(intPos.x, intPos.y - 1);
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
	weather.Update();
}

void WeatherLineRenderer::OnMiddleClick()
{
	weather.Projection();
	//if (image->data == nullptr) { return; }

	//for (size_t c = 0; c < weather.map.cols; c++)
	//{
	//	for (size_t r = 0; r < weather.map.rows; r++)
	//	{
	//		
	//		weather.map(c, r).density = image->data[((weather.map.rows - 1 - r) * weather.map.rows + c) + 0] / 256.0f;


	//	}
	//}


	//weather.Update();
	//updatingWeather = !updatingWeather;
}

void WeatherLineRenderer::OnMiddleRelease()
{

}
