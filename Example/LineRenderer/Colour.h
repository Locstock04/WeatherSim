#pragma once

struct Colour
{
	float r, g, b;
	Colour(float red, float green, float blue) : r(red), g(green), b(blue) {}

	Colour Multiply(float factor) const;
	Colour Desaturate(float factor) const;
	Colour Invert() const;

	Colour Dark() const { return Multiply(0.5f); }
	Colour Light() const { return Desaturate(0.5f); }

	static const Colour RED;
	static const Colour GREEN;
	static const Colour BLUE;

	static const Colour CYAN;
	static const Colour MAGENTA;
	static const Colour YELLOW;

	static const Colour WHITE;
	static const Colour BLACK;
	static const Colour GREY;
};