#include "Tile.h"
#include "Window.h"

Tile::Tile(TileData data)
{
	texture.loadFromFile(data.imagePath);
	sprite.setTexture(texture);
	sprite.setTextureRect(data.rect);
	sprite.setPosition(data.position);
	tileNum = data.tileNumber;
}

Tile::Tile(TileData data, sf::Vector2f scale)
{
	texture.loadFromFile(data.imagePath);
	sprite.setTexture(texture);
	sprite.setTextureRect(data.rect);
	sprite.setPosition(data.position);
	tileNum = data.tileNumber;
	sprite.scale(scale);
}

Tile::~Tile()
{
}

void Tile::Render()
{
	Window::Get()->window->draw(sprite);
}

bool Tile::ContainsMouse()
{
	return sprite.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*Window::Get()->window)));
}

void Tile::SetRect(sf::IntRect rect, int tileNumber)
{
	sprite.setTextureRect(rect);
	tileNum = tileNumber;
}

sf::Sprite Tile::GetSprite()
{
	return sprite;
}

int Tile::GetTileNum()
{
	return tileNum;
}


