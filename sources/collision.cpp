#include "collision.hpp"
#include <stdio.h>

#define EPSILON 0.0001f

bool testHitLineLevel(const Line& line, const Line* pLevel, int numLevelLines, Vector2* pIntersection)
{
	Vector2 lineDir = line.end - line.start;
	Vector2 normal = Vector2(lineDir.y, -lineDir.x).normalize();
	
	for(int i = 0; i < numLevelLines; i++)
	{
		const Line& levelLine = pLevel[i];
		float side1 = (levelLine.start - line.start) * normal;
		float side2 = (levelLine.end - line.start) * normal;
		if(side1 * side2 <= 0)
		{
			Vector2 levelNormal = Vector2(levelLine.end.y - levelLine.start.y, levelLine.start.x - levelLine.end.x).normalize();
			side1 = (line.start - levelLine.start) * levelNormal;
			side2 = (line.end - levelLine.start) * levelNormal;
			if(side1 * side2 <= 0)
			{
				*pIntersection = line.start + lineDir * (-side1 / (side2 - side1));
				return true;
			}
		}
	}
	
	return false;
}

bool testHitLineCircle(const Line& line, const Circle& circle, Vector2* pIntersection)
{
	Vector2 toCircle = circle.position - line.start;
	if(toCircle.getLength() <= circle.radius)
	{
		*pIntersection = line.start;
		return true;
	}
	
	Vector2 lineDir = line.end - line.start;
	if(lineDir * toCircle <= 0)
	{
		return false;
	}
	
	Vector2 normalizedLineDir = lineDir.normalize();
	Vector2 normal(normalizedLineDir.y, -normalizedLineDir.x);
	
	float sideOffset = normal * toCircle;
	if(absf(sideOffset) > circle.radius)
	{
		return false;
	}
	
	float circleDistance = normalizedLineDir * toCircle - sqrtf(circle.radius * circle.radius - sideOffset * sideOffset);
	
	if(circleDistance <= lineDir.getLength())
	{
		*pIntersection = line.start + normalizedLineDir * circleDistance;
		return true;
	}
	
	return false;
}

static void findClosestPointOnLine(const Vector2& position, const Line& line, Vector2* pClosestPoint)
{
	Vector2 lineDir = line.end - line.start;
	Vector2 toPosition = position -  line.start;
	float posOnLine = (toPosition * lineDir) / (lineDir * lineDir);
	if(posOnLine <= 0)
	{
		*pClosestPoint = line.start;
	}
	else if(posOnLine >= 1)
	{
		*pClosestPoint = line.end;
	}
	else
	{
		*pClosestPoint = line.start + lineDir * posOnLine;
	}
}

bool testHitCircleLine(const Circle& circle, const Line& line, Vector2* pClosestPoint)
{
	Vector2 closestPoint;
	findClosestPointOnLine(circle.position, line, &closestPoint);
	
	if((closestPoint - circle.position).getLength() <= circle.radius)
	{
		*pClosestPoint = closestPoint;
		return true;
	}
	
	return false;
}

void collideSoftCircleLevel(Circle& circle, const Line* pLevel, int numLevelLines)
{
	for(int i = 0; i < numLevelLines; i++)
	{
		Vector2 closestPoint;
		if(testHitCircleLine(circle, pLevel[i], &closestPoint))
		{
			Vector2 toCircle = circle.position - closestPoint;
			float moveBy = (circle.radius - toCircle.getLength()) / 2;
			circle.radius -= moveBy;
			circle.position += toCircle.normalize() * moveBy;
		}
	}
}

bool testHitSweptCircleLevel(const Circle& _circle, const Vector2& movement, const Line* pLevel, int numLevelLines, Vector2* pEndPosition, Vector2* pHitNormal, float* pHitDistance)
{
	Circle circle = _circle;
	circle.radius += EPSILON;
	collideSoftCircleLevel(circle, pLevel, numLevelLines);
	circle.radius -= EPSILON;
	
	float moveDistance = movement.getLength();
	float hitDistance = moveDistance;
	
	Vector2 normalizedMovement = movement.normalize();
	
	for(int i = 0; i < numLevelLines; i++)
	{
		const Line& line = pLevel[i];
		Vector2 lineNormal = Vector2(line.end.y - line.start.y, line.start.x - line.end.x).normalize();
		Vector2 circleToLine = line.start - circle.position;
		
		float distance = hitDistance;
		Vector2 hit;
		
		bool testEndPoints = absf(lineNormal * circleToLine) <= circle.radius;
		if(!testEndPoints)
		{
			Vector2 sphereEdge;
			if(circleToLine * lineNormal < 0)
			{
				sphereEdge = circle.position - lineNormal * circle.radius;
			}
			else
			{
				sphereEdge = circle.position + lineNormal * circle.radius;
			}
			Vector2 intersection;
			if(testHitLineLevel(Line(sphereEdge, sphereEdge + movement), &line, 1, &intersection))
			{
				distance = (intersection - sphereEdge).getLength();
				hit = intersection;
			}
			else
			{
				testEndPoints = true;
			}
		}
		
		if(testEndPoints)
		{
			Vector2 closestPoint;
			findClosestPointOnLine(circle.position, line, &closestPoint);
			Vector2 circleHitPoint;
			if(testHitLineCircle(Line(closestPoint, closestPoint - movement), circle, &circleHitPoint))
			{
				distance = (circleHitPoint - closestPoint).getLength();
				hit = closestPoint;
			}
		}
		
		if(distance < hitDistance)
		{
			hitDistance = distance;
			*pHitNormal = (circle.position + normalizedMovement * distance - hit).normalize();
		}
	}

	*pEndPosition = circle.position + normalizedMovement * hitDistance;
	*pHitDistance = hitDistance;
	
	return hitDistance < moveDistance;
}
