#include "Weather.h"

#include <iostream>

void Weather::Projection()
{
	for (size_t i = 0; i < projectionIterations; i++)
	{
		for (size_t col = 0; col < map.cols; col++)
		{
			for (size_t row = 0; row < map.rows; row++)
			{

				ForceIncompressibilityAt(col, row);
				//total += getAverageWindVelocityAt(col, row);

			}
		}
	}
}

void Weather::Advection()
{
	// Save old density

	Array2D<float, MAP_SIZE, MAP_SIZE> oldDensities;
	
	for (size_t col = 0; col < map.cols; col++)
	{
		for (size_t row = 0; row < map.rows; row++)
		{
			oldDensities(col, row) = map(col, row).density;
		}
	}


	for (size_t col = 0; col < map.cols; col++)
	{
		for (size_t row = 0; row < map.rows; row++)
		{
			Cell& atCell = map(col, row);

			float density = oldDensities(col, row);

			if (density == 0.0f) { continue; }

			Cell& rightCell = map(col + 1, row);
			Cell& downCell = map(col, row - 1);

			Cell& leftCell = map(col - 1, row);
			Cell& upCell = map(col, row + 1);

			// TODO: Doesn't consider the other forces as each velocity is a vec 2
			float outLeft = fmax(-atCell.leftVelocity.x, 0.0f);
			float outRight = fmax(rightCell.leftVelocity.x, 0.0f);
			float outUp = fmax(atCell.upVelocity.y, 0.0f);
			float outDown = fmax(-downCell.upVelocity.y, 0.0f);

			// Divergence
			float totalOut = outLeft + outRight + outUp + outDown;
			if (totalOut == 0.0f) { continue; }



			// 

			// TODO: 

			leftCell.density += outLeft / totalOut * density;
			rightCell.density += outRight / totalOut * density;
			upCell.density += outUp / totalOut * density;
			downCell.density += outDown / totalOut * density;

			atCell.density -= ((outLeft / totalOut) + (outRight / totalOut) + (outUp / totalOut) + (outDown / totalOut)) * density;

		}
	}

}

Vec2 Weather::CalculateDivergence(Vec2 right, Vec2 left, Vec2 up, Vec2 down)
{
	return right - left + up - down;
}

Weather::Weather()
{
	//RandomiseMapVelocities();
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

	Advection();

	//total /= map.getSize();
	//std::cout << total.x << ", " << total.y << "\n";

	//std::cout << (total, Vec2(0, 0)).GetMagnitude() << "\n";
}

float Weather::getDeltaTime() const
{
	return timeStep;
}

void Weather::ForceIncompressibilityAt(int col, int row)
{
	// TODO: Wall suppport, more general solution

	Cell& atCell = map(col, row);
	Cell& belowCell = map(col, row - 1);
	Cell& rightCell = map(col + 1, row);

	
	Vec2& up = atCell.upVelocity;
	Vec2& down = belowCell.upVelocity;
	Vec2& left = atCell.leftVelocity;
	Vec2& right = rightCell.leftVelocity;

	Vec2 divergence = overRelaxation * CalculateDivergence(right, left, up, down);
	Vec2 balance = divergence / 4;

	left += balance;
	right -= balance;
	down += balance;
	up -= balance;
	
	atCell.pressure += balance.GetMagnitude() * ((atCell.density * gridSpacing) / timeStep);
}

Vec2 Weather::getAverageWindVelocityAt(int col, int row)
{
	const Cell& at = map(col, row);
	const Cell& below = map(col, row - 1);
	const Cell& right = map(col + 1, row);

	return (at.leftVelocity + at.upVelocity + below.upVelocity + right.leftVelocity) / 4.0f;
}

float Weather::getPressureAt(int col, int row)
{
	// TODO:
	return 0.0f;
}

void Weather::RandomiseMapVelocities()
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

		i->upVelocity = (Vec2(rand() % 201 - 100.f,
			rand() % 201 - 100.f)).GetNormalised();
		if (isnan(i->upVelocity.x)) {
			i->upVelocity = { 0.4, 0.4 };
		}
	}
}
