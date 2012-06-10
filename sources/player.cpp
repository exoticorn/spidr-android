#include "player.hpp"
#include "objects.hpp"
#include "level.hpp"
#include "collision.hpp"
#include "sfx.hpp"
#include "GL/gl.h"
#include <stdio.h>

void render_object(const float*);

const float playerRadius = 0.125f;
const float hookRange = 2.2f;
const float normalHookDistance = 0.5f;
const float hookSpeed = 8;
const float gravitation = 1.5f;
const float stopFactor = 20;
const float stopConstant = 4.0f;
const float accelFactor = 3;

void Player::initialize(Level* pLevel)
{
	m_pLevel = pLevel;
	m_position = pLevel->getStartPosition();
	m_movement.clear();
	m_hookState = Hook_Aiming;
}

void Player::update(float timeStep, const Input& input)
{
	if(input.stick.getLength() > 0)
	{
		m_aimDir = input.stick.normalize();
	}
	
	Vector2 oldPosition = m_position;
	m_movement.y += gravitation * timeStep;
	m_position += m_movement * timeStep;
	
	if(!input.playDead)
	{
		switch(m_hookState)
		{
		case Hook_Aiming:
		
			if(input.buttonTriggered)
			{
				m_hookPosition = m_position + m_aimDir * playerRadius;
				m_throwDir = m_aimDir;
				m_hookState = Hook_Throwing;
				FxSynth::playSfx(sfx_shoot);
			}
			break;
		
		case Hook_Throwing:
			{
				Vector2 oldHookPosition = m_hookPosition;
				m_hookPosition += m_throwDir * (hookSpeed * timeStep);
				bool rangeReached = false;
				if((m_hookPosition - m_position).getLength() > hookRange)
				{
					rangeReached = true;
					m_hookPosition = m_position + (m_hookPosition - m_position).normalize() * hookRange;
				}
			
				if(m_pLevel->testLineCollision(Line(oldHookPosition, m_hookPosition), &m_hookPosition))
				{
					m_hookState = Hook_Attached;
					FxSynth::playSfx(sfx_hit);
				}
				else
				{
					if(rangeReached || !input.button)
					{
						m_hookState = Hook_Aiming;
						break;
					}
				}
			}
			break;
		
		case Hook_Attached:
			if(!input.button)
			{
				m_hookState = Hook_Aiming;
				break;
			}
		
			Vector2 toHook = m_hookPosition - m_position;
			float distance = toHook.getLength();
			toHook = toHook.normalize();
		
			float movementAway = -(toHook * m_movement);
			float force = 0;
			if(movementAway > 0 && distance > normalHookDistance)
			{
				force = movementAway * stopFactor + stopConstant;
			}
			if(distance > normalHookDistance)
			{
				force += (distance - normalHookDistance) * accelFactor;
			}
		
			m_movement += toHook * (force * timeStep);		
			break;
		}
	}
	
	m_position = oldPosition;
	resolvePosition(1, 4, timeStep);
	
	if(!input.playDead)
	{
		m_pLevel->collectOrb(m_position);
	}
}

void Player::resolvePosition(float moveFactor, int depth, float timeStep)
{
	Vector2 endPosition;
	Vector2 hitNormal;
	Vector2 move = m_movement * timeStep;
	float hitDistance;
	if(m_pLevel->testCircleCollision(Circle(m_position, playerRadius), move * moveFactor, &endPosition, &hitNormal, &hitDistance))
	{
		m_position = endPosition;
		float hitForce = absf(m_movement * hitNormal);
		if(hitForce > 0.1)
		{
			FxSynth::playSfx(sfx_collide, minf(hitForce / 4, 1));
		}
		m_movement = m_movement - hitNormal * (hitNormal * m_movement * 1.5f);
		
		if(depth > 0)
		{
			float fraction = hitDistance / move.getLength();
			resolvePosition(moveFactor - fraction, depth - 1, timeStep);
		}
	}
	else
	{
		m_position += move * moveFactor;
	}
}

void Player::render(bool renderAiming)
{
	glPushMatrix();
	Vector2 pos = m_position;
	glTranslatef(pos.x - 4 * playerRadius, pos.y - 4 * playerRadius, 0);
	glScalef(playerRadius * 8, playerRadius * 8, 1);
	render_object(obj_spidr);
	glPopMatrix();

	if(renderAiming)
	{
		glLineStipple(4, 0x4444);
		glEnable(GL_LINE_STIPPLE);
		Vector2 vtx[2];
		vtx[0] = (m_position + m_aimDir * playerRadius);
		vtx[1] = vtx[0] + m_aimDir * (hookRange - playerRadius);
		glVertexPointer(2, GL_FLOAT, 0, vtx);
		glDrawArrays(GL_LINES, 0, 2);
		glDisable(GL_LINE_STIPPLE);

		if(m_hookState != Hook_Aiming)
		{
			vtx[0] = (m_position + (m_hookPosition - m_position).normalize() * playerRadius);
			vtx[1] = m_hookPosition;
			glDrawArrays(GL_LINES, 0, 2);
		}
	}	
}
