#include "button.hpp"
#include "renderer.hpp"
#include "player.hpp"
#include "exo/base/functions.hpp"
#include <string.h>

namespace exo
{
	Button::Button(const char* pText)
	{
		m_pText = pText;
		m_width = 20 * strlen(pText);
		m_fade = m_maxFade = max<int>(6, strlen(pText));
		m_fadingOut = true;
		m_pressed = false;
		m_margin = 10;
		m_gamepadButtons = 0;
		m_posX = m_posY = 0;
	}

	void Button::update(float timeStep, float posX, float posY)
	{
		m_fade += timeStep * (m_fadingOut ? 30 : -30);
		m_fade = clamp(0.0f, m_maxFade, m_fade);
		m_posX = posX;
		m_posY = posY;

		m_margin += ((m_pressed ? 15 : 10) - m_margin) * min(1.0f, timeStep * 30);
	}

	bool Button::handleInput(Input& input)
	{
		bool padPressed = (input.buttons & m_gamepadButtons) != 0;

		if(input.touch || padPressed)
		{
			if(padPressed || (
				input.pos.x >= m_posX - 10 && input.pos.x < m_posX + m_width + 10 &&
				input.pos.y >= m_posY - 10 && input.pos.y < m_posY + 30))
			{
				input.consumeInput();
				m_pressed = true;
			}
			else
			{
				m_pressed = false;
			}
		}
		else
		{
			if(m_pressed)
			{
				m_pressed = false;
				return true;
			}
		}
		return false;
	}

	static void drawLine(Renderer& renderer, float fade, const Vector2& a, const Vector2& b)
	{
		if(fade > 1)
		{
			return;
		}
		Vector2 line[2];
		line[0] = a;
		if(fade <= 0)
		{
			line[1] = b;
		}
		else
		{
			line[1] = a + (b - a) * (1 - fade);
		}
		renderer.drawLines(&line[0].x, 2);
	}

	void Button::render(Renderer& renderer)
	{
		print(renderer, Vector2(m_posX, m_posY), m_fade, "%s", m_pText);
		float margin = m_margin;
		Vector2 base(m_posX - margin, m_posY - margin);
		drawLine(renderer, m_fade / 3 - 1, base, Vector2(base.x + m_width + margin * 2, base.y));
		drawLine(renderer, m_fade / 3 - 1, base, Vector2(base.x, base.y + 20 + margin * 2));
		drawLine(renderer, m_fade / 3, Vector2(base.x + m_width + margin * 2, base.y), Vector2(base.x + m_width + margin * 2, base.y + 20 + margin * 2));
		drawLine(renderer, m_fade / 3, Vector2(base.x, base.y + 20 + margin * 2), Vector2(base.x + m_width + margin * 2, base.y + 20 + margin * 2));
	}
}
