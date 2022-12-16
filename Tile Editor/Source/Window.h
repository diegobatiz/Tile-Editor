#include <SFML/Graphics.hpp>

class Window
{
public:
	virtual ~Window();
	static Window* Get();
	sf::RenderWindow* window;
	static int height, width;

private:
	Window();
	static Window* instance;
};