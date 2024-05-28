#include "Weather.h"

#include <iostream>

void Weather::Projection()
{
	for (size_t i = 0; i < projectionIterations; i++)
	{
		for (size_t col = 0; col < map.getColCount(); col++)
		{
			for (size_t row = 0; row < map.getRowCount(); row++)
			{

				ForceIncompressibilityAt(col, row);
				//total += getAverageWindVelocityAt(col, row);

			}
		}
	}
}

Weather::Weather()
{
	std::srand(1337);
	//std::srand(700);

	for (auto i = map.Begin(); i < map.End(); i++)
	{

		i->leftVelocity = (Vec2(rand() % 201 - 100.f,
			rand() % 201 - 100.f)).GetNormalised();
		if (isnan(i->leftVelocity.x)) {
			i->leftVelocity = { 0.4, 0.4 };
		}

		i->topVelocity = (Vec2(rand() % 201 - 100.f,
			rand() % 201 - 100.f)).GetNormalised();
		if (isnan(i->topVelocity.x)) {
			i->topVelocity = { 0.4, 0.4 };
		}
	}
	//map(0, 0).windVelocity = { -1, -1 };
	//map(2, 2).windVelocity = { -1, -1 };
	//map(4, 4).windVelocity = { -1, -1 };
	//map(6, 6).windVelocity = { -1, -1 };
	//map(4, 6).windVelocity = { -1, -1 };
	//map(4, 8).windVelocity = { -1, -1 };
}

void Weather::Update()
{
	//Vec2 total(0, 0);

	Projection();

	//total /= map.getSize();
	//std::cout << total.x << ", " << total.y << "\n";

	//std::cout << (total, Vec2(0, 0)).GetMagnitude() << "\n";
}

void Weather::ForceIncompressibilityAt(int col, int row)
{
	// TODO: Wall suppport, more general solution

	Cell& atCell = map(col, row);
	Cell& belowCell = map(col, row - 1);
	Cell& rightCell = map(col + 1, row);

	
	Vec2& up = atCell.topVelocity;
	Vec2& down = belowCell.topVelocity;
	Vec2& left = atCell.leftVelocity;
	Vec2& right = rightCell.leftVelocity;

	Vec2 divergence = overRelaxation * (right - left + up - down);
	Vec2 balance = divergence / 4;

	left += balance;
	right -= balance;
	down += balance;
	up -= balance;
	
	atCell.pressure += balance.GetMagnitude() * ((atCell.density * gridSpacing) / timeStep);
}

Vec2 Weather::getAverageWindVelocityAt(int col, int row)
{
	Cell& at = map(col, row);
	Cell& below = map(col, row - 1);
	Cell& right = map(col + 1, row);

	return (at.leftVelocity + at.topVelocity + below.topVelocity + right.leftVelocity) / 4.0f;
}

float Weather::getPressureAt(int col, int row)
{
	// TODO:
	return 0.0f;
}
