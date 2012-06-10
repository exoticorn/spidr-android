#ifndef __PLAYER_HPP
#define __PLAYER_HPP

#include "math.hpp"

class Level;

struct Input
{
	Input() { stick.clear(); button = buttonTriggered = false; }
	Vector2 stick;
	bool button;
	bool buttonTriggered;
	bool playDead;
};

class Player
{
public:

	void initialize(Level* pLevel);

	const Vector2& getPosition() const { return m_position; }
	void update(float timeStep, const Input& input);
	void render(bool renderAiming);

private:
	void resolvePosition(float moveFactor, int depth, float timeStep);

	Vector2 m_position;
	Vector2 m_movement;
	
	Vector2 m_aimDir;
	Vector2 m_throwDir;
	Vector2 m_hookPosition;
	
	Level* m_pLevel;
	
	enum HookState { Hook_Aiming, Hook_Throwing, Hook_Attached };
	
	HookState m_hookState;
};

#endif // __PLAYER_HPP

