#include <SFML/Graphics.hpp>



struct TileData
{
	std::string imagePath;
	sf::IntRect rect;
	sf::Vector2f position { 0.f, 0.f };
	int tileNumber;
};


class Tile
{
private:
	sf::Sprite sprite;
	int tileNum;
	sf::Texture texture;


public:
	Tile(TileData data);
	Tile(TileData data, sf::Vector2f scale);
	virtual ~Tile();
	void Render();
	bool ContainsMouse();
	void SetRect(sf::IntRect rect, int tileNum);
	sf::Sprite GetSprite();
	int GetTileNum();
};