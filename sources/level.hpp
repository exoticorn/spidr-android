#ifndef __LEVEL_HPP
#define __LEVEL_HPP

#include "math.hpp"

struct Line;
struct Circle;

struct LevelData
{
	Vector2 startPosition;
	int numGfxVertices;
	const float* pVertices;
	int numCollisionLines;
	const Line* pCollisionLines;
	int numOrbs;
	const Vector2* pOrbs;
};

class Level
{
public:
	Level();
	~Level();
	
	void initialize(const LevelData* pLevelData);
	
	const Vector2& getStartPosition() { return m_pLevelData->startPosition; }
	bool testLineCollision(const Line& line, Vector2* pIntersection) const;
	bool testCircleCollision(const Circle& circle, const Vector2& movement, Vector2* pEndPosition, Vector2* pHitNormal, float* pHitDistance) const;
	void collideSoftCircle(Circle& circle) const;
	void collectOrb(const Vector2& position);
	int numOrbsLeft() const { return m_numOrbsLeft; }
	
	void render() const;
	
private:

	const LevelData* m_pLevelData;
	bool* m_pActiveOrbs;
	int m_numOrbsLeft;
};

extern const int numLevels;
extern const LevelData* pLevels[];
extern const LevelData level99;

#endif // __LEVEL_HPP

