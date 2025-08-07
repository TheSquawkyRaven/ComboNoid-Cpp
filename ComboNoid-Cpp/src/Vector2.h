#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>

class Vector2
{
public:

	float x = 0;
	float y = 0;

	Vector2() {}
	Vector2(float x, float y) : x(x), y(y) {}

	float Magnitude() const;
	float MagnitudeSqr() const;
	void Normalize();
	Vector2 Normalized();

	float Dot(const Vector2& rhs) const;

	Vector2 Reflect(const Vector2& normal) const;

	Vector2 operator+(Vector2 const& other) const;
	Vector2 operator-(Vector2 const& other) const;
	Vector2 operator*(float const& other) const;
	Vector2 operator/(float const& other) const;

	bool operator==(Vector2 const& other) const;
	bool operator!=(Vector2 const& other) const;

};

