#include "Vector2.h"

float Vector2::Magnitude()
{
	return sqrtf(x * x + y * y);
}

void Vector2::Normalize()
{
	float magnitude = Magnitude();
	x /= magnitude;
	y /= magnitude;
}

Vector2 Vector2::Normalized()
{
	// Create a copy of the current vector, normalize it, and return
	Vector2 current = *this;
	current.Normalize();
	return current;
}

float Vector2::Dot(const Vector2& rhs)
{
	return x * rhs.x + y * rhs.y;
}

Vector2 Vector2::Reflect(const Vector2& normal)
{
	return *this - normal * (2.0f * this->Dot(normal));
}

Vector2 Vector2::operator+(Vector2 const& other) const
{
	return Vector2(x + other.x, y + other.y);
}

Vector2 Vector2::operator-(Vector2 const& other) const
{
	return Vector2(x - other.x, y - other.y);
}

Vector2 Vector2::operator*(float const& other) const
{
	return Vector2(x * other, y * other);
}

Vector2 Vector2::operator/(float const& other) const
{
	return Vector2(x / other, y / other);
}

bool Vector2::operator==(Vector2 const& other) const
{
	return x == other.x && y == other.y;
}

bool Vector2::operator!=(Vector2 const& other) const
{
	return x != other.x && y != other.y;
}
