#pragma once

#include <stdint.h>

class Color
{
public:
	Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) { this->r = r; this->g = g; this->b = b; this->a = a; }
	uint8_t r, g, b, a;

	static Color DarkGray;
	static Color LightGray;
	static Color White;
	static Color Black;
	static Color Red;
	static Color Green;
	static Color LightGreen;
	static Color Blue;
	static Color Cyan;
	static Color Brown;
	static Color Navy;
	static Color WineRed;
};