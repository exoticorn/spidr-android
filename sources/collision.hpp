#ifndef __COLLISION_HPP
#define __COLLISION_HPP

#include "math.hpp"

struct Line
{
	Line(const Vector2& s, const Vector2& e) { start = s; end = e; }
	Vector2 start;
	Vector2 end;
};

struct Circle
{
	Circle(const Vector2& p, float r) { position = p; radius = r; }
	Vector2 position;
	float radius;
};

bool testHitLineLevel(const Line& line, const Line* pLevel, int numLevelLines, Vector2* pIntersection);
bool testHitLineCircle(const Line& line, const Circle& circle, Vector2* pIntersection);
bool testHitCircleLine(const Circle& circle, const Line& line, Vector2* pClosestPoint);
void collideSoftCircleLevel(Circle& circle, const Line* pLevel, int numLevelLines);
bool testHitSweptCircleLevel(const Circle& circle, const Vector2& movement, const Line* pLevel, int numLevelLines, Vector2* pEndPos, Vector2* pHitNormal, float* hitDistance);

#endif // __COLLISION_HPP

