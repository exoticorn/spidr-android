#include "level.hpp"
#include "objects.hpp"
#include "collision.hpp"
#include "sfx.hpp"
#include "GL/gl.h"
#include <stdio.h>
#include <string.h>

void render_object(const float*);
extern int score;

Level::Level()
{
	m_pActiveOrbs = NULL;
}

Level::~Level()
{
	delete [] m_pActiveOrbs;
}

void Level::initialize(const LevelData* pLevelData)
{
	m_pLevelData = pLevelData;
	delete [] m_pActiveOrbs;
	m_pActiveOrbs = new bool[pLevelData->numOrbs];
	for(int i = 0; i < pLevelData->numOrbs; ++i)
	{
		m_pActiveOrbs[i] = true;
	}
	m_numOrbsLeft = pLevelData->numOrbs;
}

bool Level::testLineCollision(const Line& line, Vector2* pIntersection) const
{
	return testHitLineLevel(line, m_pLevelData->pCollisionLines, m_pLevelData->numCollisionLines, pIntersection);
}

bool Level::testCircleCollision(const Circle& circle, const Vector2& movement, Vector2* pEndPosition, Vector2* pHitNormal, float* pHitDistance) const
{
	return testHitSweptCircleLevel(circle, movement, m_pLevelData->pCollisionLines, m_pLevelData->numCollisionLines, pEndPosition, pHitNormal, pHitDistance);
}

void Level::collideSoftCircle(Circle& circle) const
{
	collideSoftCircleLevel(circle, m_pLevelData->pCollisionLines, m_pLevelData->numCollisionLines);
}

void Level::collectOrb(const Vector2& position)
{
	for(int i = 0; i < m_pLevelData->numOrbs; i++)
	{
		float distance = (m_pLevelData->pOrbs[i] - position).getLength();
		if(m_pActiveOrbs[i] && distance < 0.3f)
		{
			m_pActiveOrbs[i] = false;
			m_numOrbsLeft--;
			score += 100;
			FxSynth::playSfx(sfx_collect);
		}
	}
}

void Level::render() const
{
	glVertexPointer(2, GL_FLOAT, 0, m_pLevelData->pVertices);
	glDrawArrays(GL_LINES, 0, m_pLevelData->numGfxVertices);
	
	for(int i = 0; i < m_pLevelData->numOrbs; i++)
	{
		if(m_pActiveOrbs[i])
		{
			const Vector2& pos = m_pLevelData->pOrbs[i];
			glPushMatrix();
			glTranslatef(pos.x - 0.5f, pos.y - 0.5f, 0);
			render_object(obj_orb);
			glPopMatrix();
		}
	}
}

