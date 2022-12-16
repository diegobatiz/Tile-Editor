#include "Window.h"

Window* Window::instance = nullptr;
int Window::height = 0;
int Window::width = 0;

Window::Window()
{
	window = new sf::RenderWindow(sf::VideoMode(width, height, 32), "Tile Editor");
}

Window::~Window()
{
	delete window;
}

Window* Window::Get()
{
	if (instance == nullptr)
	{
		instance = new Window();
	}

	return instance;
}

