#include "Tile.h"
#include <SFML/Graphics.hpp>
#include <vector>


enum class OpenMode
{
	Load,
	New
};

struct TileMapData
{
	std::string imagePath;
	std::string filePath;
	int imageWidth;
	int imageHeight;
	float tileSideLength;
	OpenMode mode;
	int tileMapWidth;
	int tileMapHeight;
};

class TileMap
{
private:
	TileMapData data;
	std::map<int, sf::IntRect> tileChoices;

	int totalTiles;
	int currentTile;
	bool keyPressed;
	bool hasBeenSaved;

	std::vector<Tile*> tiles;
	std::vector<Tile*> tilesSecondWindow;

	sf::RenderWindow* sideWindow;

	void ChangeTile(Tile* tile);
	void Save();

public:
	TileMap(TileMapData tileSet);
	virtual ~TileMap();
	void Update();
	void Render();
};