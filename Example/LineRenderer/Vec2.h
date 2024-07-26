#pragma once

class Vec2
{
public:
	float x = 0.0f;
	float y = 0.0f;

	Vec2() = default;
	Vec2(float xInit, float yInit) :x(xInit), y(yInit) {}
	float GetMagnitude() const;
	float GetMagnitudeSquared() const;
	Vec2 GetNormalised() const;
	void Normalise();

	void RotateBy90();
	void RotateBy270();
	void RotateBy(float angle);
	void RotateBy(float cosAngle, float sinAngle);

	Vec2 GetRotatedBy90() const;
	Vec2 GetRotatedBy270() const;
	Vec2 GetRotatedBy(float angle) const;
	Vec2 GetRotatedBy(float cosAngle, float sinAngle) const;

	void SetMagnitude(float mag);

	Vec2& operator+=(Vec2 v);
	Vec2& operator-=(Vec2 v);
	Vec2& operator*=(float s);
	Vec2& operator/=(float s);
};

//The following standalone functions work with vectors.
//Under some circumstances we'd make them friend 
//functions, but the Vec2 class has no private members
//so this isn't needed.
Vec2 operator+(Vec2 a, Vec2 b);
Vec2 operator-(Vec2 a, Vec2 b);
Vec2 operator*(Vec2 v, float s);
Vec2 operator*(float s, Vec2 v);
Vec2 operator/(Vec2 v, float s);
Vec2 operator-(Vec2 v);

float Dot(Vec2 a, Vec2 b);
float PseudoCross(Vec2 a, Vec2 b);
Vec2 ProjectOnto(Vec2 projector, Vec2 projectee);