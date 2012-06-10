#ifndef EXO_APPLICATIONBASE_HPP
#define EXO_APPLICATIONBASE_HPP

namespace exo
{
	class GameFramework;

	class ApplicationBase {
	public:
		ApplicationBase(GameFramework& gameFramework) : m_gameFramework(gameFramework) {}
		virtual ~ApplicationBase() {};

		virtual void	update(float timeStep) = 0;
		virtual void	render() = 0;

		GameFramework&	m_gameFramework;
	};
}

#endif
