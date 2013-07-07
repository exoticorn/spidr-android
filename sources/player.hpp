#ifndef __PLAYER_HPP
#define __PLAYER_HPP

#include "math.hpp"
#include "exo/base/serializer.hpp"

class Level;
namespace exo
{
	class Renderer;
}

struct Input
{
	Input() { stick.clear(); button = buttonTriggered = touch = touchTriggered = playDead = false; buttons = 0; }
	void consumeInput() { button = buttonTriggered = touch = touchTriggered = false; }
	Vector2 stick;
	Vector2	pos;
	bool button;
	bool buttonTriggered;
	bool touch;
	bool touchTriggered;
	exo::uint32 buttons;
	bool playDead;
};

class Player
{
public:

	void initialize(Level* pLevel);

	const Vector2& getPosition() const { return m_position; }
	void update(float timeStep, const Input& input);
	void render(exo::Renderer& renderer, bool renderHook, bool renderAiming);
	void serialize(exo::Serializer& serializer);

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

