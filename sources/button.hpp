#ifndef EXO_BUTTON_HPP
#define EXO_BUTTON_HPP

class Input;

namespace exo
{
	class Renderer;

	class Button
	{
	public:
		Button(const char* pText);

		void	setText(const char* pText) { m_pText = pText; }

		void	fadeIn() { m_fadingOut = false; }
		void	fadeOut() { m_fadingOut = true; }

		void	update(float timeStep, float posX, float posY);
		bool	handleInput(Input& input);
		void	render(Renderer& renderer);

		float	getWidth() const { return m_width; }
		float	getHeight() const { return 20; }

	private:
		const char*	m_pText;
		float		m_posX;
		float		m_posY;
		float		m_width;
		float		m_fade;
		float		m_maxFade;
		bool		m_fadingOut;
		bool		m_pressed;
		float		m_margin;
	};
}

#endif
