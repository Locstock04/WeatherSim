#include "WeatherLineRenderer.h"+


#include "imgui.h"

void WeatherLineRenderer::DrawWindSideVelocities()
{
	for (size_t c = 0; c < weather.map.getColCount(); c++)
	{
		for (size_t r = 0; r < weather.map.getRowCount(); r++)
		{
			//if (r != c) { continue; }
			//Vec2 cellPos = Vec2(c, r) * distanceBetween2AdjacentCells;

			//lines->DrawCircle(Vec2(c, r), 0.3);

			Vec2 leftPos = Vec2(c - 0.5f, r);
			Vec2 topPos = Vec2(c, r + 0.5f);
			Vec2 left = weather.map(c, r).leftVelocity;
			Vec2 top = weather.map(c, r).topVelocity;
			Colour leftColour = ColourFromVector(left);
			Colour topColour = ColourFromVector(top);
			Vec2 leftLineEnd = leftPos + (left * scaleLineLengthViewBy);
			Vec2 topLineEnd = topPos + (top * scaleLineLengthViewBy);

			DrawArrow(leftPos, leftLineEnd, leftColour);
			DrawArrow(topPos, topLineEnd, topColour);
		}
	}
}

void WeatherLineRenderer::DrawCentreWindVelocities()
{
	for (size_t c = 0; c < weather.map.getColCount(); c++)
	{
		for (size_t r = 0; r < weather.map.getRowCount(); r++)
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
		ImGui::DragFloat("Line Tips Size", &lineArrowSize, guiDragSpeed);
		ImGui::DragFloat("Left click change multiplayer", &dragMultiplier, guiDragSpeed);
	}
	ImGui::End();
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
		for (size_t c = 0; c < weather.map.getColCount(); c++)
		{
			for (size_t r = 0; r < weather.map.getRowCount(); r++)
			{
				lines->DrawCircle(Vec2(c, r), 0.3, ColourFromVector(weather.getAverageWindVelocityAt(c, r)));
			}
		}
	}
	
	GUI();

	if (rightMouseDown) {
		weather.Update();
	}

	if (leftMouseDown) {
		Vec2 intPos = Vec2((int)(cursorPos.x + 0.5f), (int)(cursorPos.y + 0.5f));
		Weather::Cell& atCell = weather.map(intPos.x, intPos.y);
		Weather::Cell& rightCell = weather.map(intPos.x + 1, intPos.y);
		Weather::Cell& downCell = weather.map(intPos.x, intPos.y - 1);
		Vec2 newVelocity = (cursorPos - previousCursorPos) * dragMultiplier;
		atCell.leftVelocity += newVelocity;
		atCell.topVelocity += newVelocity;
		rightCell.leftVelocity += newVelocity;
		downCell.topVelocity += newVelocity;

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

}
