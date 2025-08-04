#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>

class Color
{
public:

	Uint8 r = 0;
	Uint8 g = 0;
	Uint8 b = 0;
	Uint8 a = 255;

	Color() {}
	inline Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 1.0f) 
		: r(r), g(g), b(b), a(a)
	{}
	inline Color(float r, float g, float b, float a = 1.0f)
		: r(static_cast<Uint8>(r * 255)), g(static_cast<Uint8>(g * 255)), b(static_cast<Uint8>(b * 255)), a(static_cast<Uint8>(a * 255))
	{}

	inline static const Color White() { return Color(1.0f, 1.0f, 1.0f); }
	inline static const Color Black() { return Color(0.0f, 0.0f, 0.0f); }
	inline static const Color Transparent() { return Color(0.0f, 0.0f, 0.0f, 0.0f); }

};

