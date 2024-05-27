#include "WeatherLineRenderer.h"+

#include "LineRenderer.h"

#include "imgui.h"

void WeatherLinerRenderer::DrawWindVelocities()
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
			Colour leftColour = Colour(left.x / 2.0f + 0.5f, left.y / 2.0f + 0.5f, -left.x / 2.0f + 0.5f);
			Colour topColour = Colour(top.x / 2.0f + 0.5f, top.y / 2.0f + 0.5f, -top.x / 2.0f + 0.5f);
			Vec2 leftLineEnd = leftPos + (left * scaleLineLengthViewBy);
			Vec2 topLineEnd = topPos + (top * scaleLineLengthViewBy);
			Vec2 leftWindNormal = (leftLineEnd - leftPos).GetNormalised();
			Vec2 topWindNormal = (topLineEnd - topPos).GetNormalised();
			lines->DrawLineSegment(leftPos, leftLineEnd, leftColour);
			lines->DrawLineSegment(topPos, topLineEnd, topColour);

			if (showLineArrows) {
				//lines->DrawCross(lineEnd, lineArrowSize, colour);
				lines->SetColour(leftColour);
				lines->AddPointToLine(leftLineEnd - (leftWindNormal * lineArrowSize) + (leftWindNormal.GetRotatedBy90()) * lineArrowSize);
				lines->AddPointToLine(leftLineEnd);
				lines->AddPointToLine(leftLineEnd - (leftWindNormal * lineArrowSize) - (leftWindNormal.GetRotatedBy90()) * lineArrowSize);
				lines->FinishLineStrip();

				lines->SetColour(topColour);
				lines->AddPointToLine(topLineEnd - (topWindNormal * lineArrowSize) + (topWindNormal.GetRotatedBy90()) * lineArrowSize);
				lines->AddPointToLine(topLineEnd);
				lines->AddPointToLine(topLineEnd - (topWindNormal * lineArrowSize) - (topWindNormal.GetRotatedBy90()) * lineArrowSize);
				lines->FinishLineStrip();
			}
		}
	}
}

void WeatherLinerRenderer::GUI()
{
	if (ImGui::Begin("Draw Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::DragFloat("Scale Line Length", &scaleLineLengthViewBy, guiDragSpeed);
		ImGui::DragFloat("Between Line Distance", &distanceBetween2AdjacentCells, guiDragSpeed);
		ImGui::Checkbox("Draw Line Tips", &showLineArrows);
		ImGui::DragFloat("Line Tips Size", &lineArrowSize);
	}
	ImGui::End();
}

WeatherLinerRenderer::WeatherLinerRenderer()
{

}

void WeatherLinerRenderer::Update(float delta)
{
	if (drawWindVelocity) {
		DrawWindVelocities();
	}
	
	GUI();

	if (leftMouseDown) {
		weather.Update();
	}
}

void WeatherLinerRenderer::OnLeftClick()
{
	//weather.Update();
}
