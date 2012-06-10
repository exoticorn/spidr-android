#ifndef __MATH_HPP
#define __MATH_HPP

#include <math.h>

class Vector2
{
public:
	Vector2() {}
	Vector2(float _x, float _y) { x = _x; y = _y; }
	
	Vector2 operator+(const Vector2& o) const { return Vector2(x + o.x, y + o.y); }
	Vector2 operator-(const Vector2& o) const { return Vector2(x - o.x, y - o.y); }
	void operator+=(const Vector2& o) { x += o.x; y += o.y; }
	void operator-=(const Vector2& o) { x -= o.x; y -= o.y; }
	Vector2 operator*(float factor) const { return Vector2(x * factor, y * factor); }
	float operator*(const Vector2& o) const { return x * o.x + y * o.y; }
	
	float getLength() const { return sqrtf(*this * *this); }
	Vector2 normalize() const { return *this * (1 / getLength()); }
	
	void clear() { x = y = 0; }
	
	float	x, y;
};

inline float absf(float a) { return a < 0 ? -a : a; }
inline float minf(float a, float b) { return a > b ? b : a; }

#endif // __MATH_HPP

