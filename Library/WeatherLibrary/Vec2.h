#pragma once

namespace LocWeather {


	class Vec2 {
	public:
		float x = 0.0f;
		float y = 0.0f;

		Vec2(float _x = 0.0f, float _y = 0.0f);


		Vec2& operator+=(Vec2 v);
		Vec2& operator-=(Vec2 v);
		Vec2& operator*=(float s);
		Vec2& operator/=(float s);

		Vec2 operator+(Vec2 v);
		Vec2 operator-(Vec2 v);
		Vec2 operator*(float s);
		Vec2 operator/(float s);
		Vec2 operator-();
	};

	

}