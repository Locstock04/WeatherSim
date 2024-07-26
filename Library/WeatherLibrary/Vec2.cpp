#include "Vec2.h"


LocWeather::Vec2::Vec2(float _x, float _y) :
	x(_x),
	y(_y)
{
}

LocWeather::Vec2& LocWeather::Vec2::operator+=(Vec2 v)
{
	x += v.x;
	y += v.x;
	return *this;
}

LocWeather::Vec2& LocWeather::Vec2::operator-=(Vec2 v)
{
	x -= v.x;
	y -= v.x;
	return *this;
}

LocWeather::Vec2& LocWeather::Vec2::operator*=(float s)
{
	x *= s;
	y *= s;
	return *this;
}

LocWeather::Vec2& LocWeather::Vec2::operator/=(float s)
{
	x /= s;
	y /= s;
	return *this;
}

LocWeather::Vec2 LocWeather::Vec2::operator+(Vec2 v)
{
	return LocWeather::Vec2(x + v.x, y + v.y);
}

LocWeather::Vec2 LocWeather::Vec2::operator-(Vec2 v)
{
	return LocWeather::Vec2(x - v.x, y - v.y);
}
LocWeather::Vec2 LocWeather::Vec2::operator*(float s)
{
	return LocWeather::Vec2(x * s, y * s);
}
LocWeather::Vec2 LocWeather::Vec2::operator/(float s)
{
	return LocWeather::Vec2(x / s, y / s);
}
LocWeather::Vec2 LocWeather::Vec2::operator-()
{
	return LocWeather::Vec2(-x, -y);
}
