#include "level.hpp"
#include "objects.hpp"
#include "collision.hpp"
#include "sfx.hpp"
#include "renderer.hpp"
#include "exo/base/serializer.hpp"
#include "exo/base/vector.hpp"
#include "exo/base/functions.hpp"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern int score;

Level::Level()
{
	m_pActiveOrbs = NULL;
	m_pLevelLines = NULL;
}

Level::~Level()
{
	delete [] m_pActiveOrbs;
	delete [] m_pLevelLines;
}

struct LevelSortVertex
{
	Vector2* pVertex;
	float sortValue;
};

static int compareFunc(const void* pA, const void* pB)
{
	float d = static_cast<const LevelSortVertex*>(pA)->sortValue - static_cast<const LevelSortVertex*>(pB)->sortValue;
	return d < 0 ? -1 : (d > 0 ? 1 : -1);
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

	delete [] m_pLevelLines;

	exo::Vector<float> buffer;
	for(int i = 0; i < pLevelData->numGfxVertices * 2; ++i)
	{
		buffer.pushBack(pLevelData->pVertices[i]);
	}

	{
		const Vector2* pVertices = reinterpret_cast<const Vector2*>(pLevelData->pFillVertices);
		Vector2 normal = Vector2(1, 2).normalize();
		Vector2 tangent(normal.y, -normal.x);
		float dotMin = 100000;
		float dotMax = -dotMin;
		for(int i = 0; i < pLevelData->numFillVertices; ++i)
		{
			float dot = pVertices[i] * normal;
			dotMin = exo::min(dot, dotMin);
			dotMax = exo::max(dot, dotMax);
		}

		const float spacing = 0.08f;
		for(float d = dotMin + spacing * 0.34f; d < dotMax; d += spacing)
		{
			exo::Vector<Vector2> points;
			for(int i = 0; i < pLevelData->numFillVertices; i += 2)
			{
				float dot1 = pVertices[i] * normal;
				bool side1 = dot1 > d;
				float dot2 = pVertices[i+1] * normal;
				bool side2 = dot2 > d;
				if(side1 != side2)
				{
					points.pushBack(pVertices[i] + (pVertices[i+1] - pVertices[i]) * ((d - dot1) / (dot2 - dot1)));
				}
			}

			if((points.getSize() & 1) == 0)
			{
				LevelSortVertex* pSortVertices = new LevelSortVertex[points.getSize()];

				for(uint i = 0; i < points.getSize(); ++i)
				{
					pSortVertices[i].pVertex = &points[i];
					pSortVertices[i].sortValue = points[i] * tangent;
				}

				qsort(pSortVertices, points.getSize(), sizeof(LevelSortVertex), compareFunc);

				for(uint i = 0; i < points.getSize(); ++i)
				{
					buffer.pushBack(pSortVertices[i].pVertex->x);
					buffer.pushBack(pSortVertices[i].pVertex->y);
				}

				delete [] pSortVertices;
			}
		}
	}

	m_numLines = buffer.getSize() / 4;
	m_pLevelLines = new exo::Renderer::Line[m_numLines];
	exo::Renderer::prepareLines(m_pLevelLines, buffer.getData(), m_numLines);
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
			FxSynth::playSfx(sfx_collect, 1, true);
		}
	}
}

void Level::render(exo::Renderer& renderer) const
{
	renderer.drawLines(m_pLevelLines, m_numLines);
	
	for(int i = 0; i < m_pLevelData->numOrbs; i++)
	{
		if(m_pActiveOrbs[i])
		{
			const Vector2& pos = m_pLevelData->pOrbs[i];
			renderer.push();
			renderer.translate(pos.x - 0.5f, pos.y - 0.5f);
			renderer.drawLines(obj_orb + 1, (exo::uint)*obj_orb);
			renderer.pop();
		}
	}
}

void Level::serialize(exo::Serializer& serializer)
{
	serializer.serialize(&m_numOrbsLeft);
	for(int i = 0; i < m_pLevelData->numOrbs; ++i)
	{
		serializer.serialize(&m_pActiveOrbs[i]);
	}
}
