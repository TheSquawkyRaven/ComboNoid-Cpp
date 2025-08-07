#include "Vector2.h"

float Vector2::Magnitude() const
{
	return sqrtf(x * x + y * y);
}

float Vector2::MagnitudeSqr() const
{
	return x * x + y * y;
}

void Vector2::Normalize()
{
	float magnitude = Magnitude();
	x /= magnitude;
	y /= magnitude;
}

Vector2 Vector2::Normalized()
{
	Vector2 current{ x, y };
	current.Normalize();
	return current;
}

float Vector2::Dot(const Vector2& rhs) const
{
	return x * rhs.x + y * rhs.y;
}

Vector2 Vector2::Reflect(const Vector2& normal) const
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
