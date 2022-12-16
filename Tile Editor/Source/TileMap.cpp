#include "TileMap.h"
#include <map>
#include <fstream>



void TileMap::ChangeTile(Tile* tile)
{
	tile->SetRect(tileChoices[currentTile], currentTile);
}

void TileMap::Save()
{
	if (hasBeenSaved == false)
	{
		std::ofstream file(data.filePath);

		file << data.tileMapWidth << "\n";
		file << data.tileMapHeight << "\n";
		for (int i = 0; i < tiles.size(); i++)
		{
			file << tiles[i]->GetTileNum() << " ";
			if (i % data.tileMapWidth == 0)
			{
				file << "\n";
			}
		}
	}

	else if (hasBeenSaved)
	{
		std::ofstream file;
		file.open(data.filePath);

		file.seekp(0, std::ios::beg);
		file.setf(std::ios::trunc);

		file << data.tileMapWidth << "\n";
		file << data.tileMapHeight << "\n";
		for (int i = 0; i < tiles.size(); i++)
		{
			file << tiles[i]->GetTileNum() << " ";
			if (i % data.tileMapWidth == 0)
			{
				file << "\n";
			}
		}

		file.close();
	}

	hasBeenSaved = true;
}


TileMap::TileMap(TileMapData tileSet) : data(tileSet)
{
	if (data.mode == OpenMode::New)
		hasBeenSaved = false;
	else
		hasBeenSaved = true;

	int numTiles = 0;
	float height = 0.f;

	for (int i = 0; i < data.imageHeight; i++)
	{
		float width = 0.f;
		for (int j = 0; j < data.imageWidth; j++)
		{
			tileChoices.insert(std::pair<int, sf::IntRect>(numTiles, sf::IntRect(width, height, data.tileSideLength, data.tileSideLength)));
			width += data.tileSideLength;
			numTiles++;
		}
		height += data.tileSideLength;
	}

	totalTiles = numTiles;


	if (data.mode == OpenMode::New)
	{
		sf::Vector2f scale(1.8, 1.8);
		float height = 0.f;
		for (int i = 0; i < data.tileMapHeight; i++)
		{
			float width = 0.f;
			for (int j = 0; j < data.tileMapWidth; j++)
			{
				TileData tileData;
				tileData.rect = tileChoices[0];
				tileData.tileNumber = 0;
				tileData.imagePath = data.imagePath;
				tileData.position = sf::Vector2f(width, height);
				Tile* tile = new Tile(tileData, scale);
				tiles.push_back(tile);
				width += data.tileSideLength * scale.x;
			}
			height += data.tileSideLength * scale.y;
		}
	}
	else
	{
		sf::Vector2f scale(1.8, 1.8);
		int height;
		int width;
		std::fstream inputStream;
		inputStream.open(data.filePath);

		inputStream >> width;
		inputStream >> height;

		tiles.clear();

		int tileType;

		while (inputStream >> tileType)
		{
			TileData tileData;


			tileData.rect = tileChoices[tileType];
			tileData.tileNumber = tileType;
			tileData.imagePath = data.imagePath;

			int index = (int)tiles.size();
			int col = index % width;
			int row = index / width;
			tileData.position = (sf::Vector2f((float)col * data.tileSideLength * scale.x, (float)row * data.tileSideLength * scale.y));
			Tile* tile = new Tile(tileData, scale);
			tiles.push_back(tile);
		}

		inputStream.close();
	}

	int windowTileSize = 500 / data.imageWidth;
	int sWindowW = windowTileSize * data.imageWidth;
	int sWindowH = windowTileSize * data.imageHeight;
	sf::Vector2f scaleFactor = sf::Vector2f((float)windowTileSize / (float)data.tileSideLength, (float)windowTileSize / (float)data.tileSideLength);

	sideWindow = new sf::RenderWindow(sf::VideoMode(sWindowW, sWindowH, 32), "Tiles");

	for (int i = 0; i < (int)data.imageHeight * data.imageWidth; i++)
	{
		TileData tileData;
		tileData.rect = tileChoices[i];
		tileData.tileNumber = i;
		tileData.imagePath = data.imagePath;
		int col = i % data.imageWidth;
		int row = i / data.imageWidth;
		tileData.position = (sf::Vector2f((float)col * windowTileSize, (float)row * windowTileSize));
		Tile* tile = new Tile(tileData, scaleFactor);
		tilesSecondWindow.push_back(tile);
	}

	for (int i = 0; i < (int)tilesSecondWindow.size(); i++)
	{
		sideWindow->draw(tilesSecondWindow[i]->GetSprite());
	}
	sideWindow->display();
}

TileMap::~TileMap()
{
	for (int i = 0; i < (int)tiles.size(); i++)
	{
		delete tiles[i];
	}
	tiles.clear();

	delete sideWindow;
}

void TileMap::Update()
{
	for (int i = 0; i < tiles.size(); i++)
	{
		if (tiles[i]->ContainsMouse() && sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			ChangeTile(tiles[i]);
		}
	}

	for (int i = 0; i < tilesSecondWindow.size(); i++)
	{
		if (tilesSecondWindow[i]->GetSprite().getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*sideWindow))) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			currentTile = tilesSecondWindow[i]->GetTileNum();
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && keyPressed == false)
	{
		if (currentTile == 0)
		{
			currentTile = totalTiles;
		}
		else
		{
			currentTile--;
		}
		keyPressed = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && keyPressed == false)
	{
		if (currentTile == totalTiles)
		{
			currentTile = 0;
		}
		else
		{
			currentTile++;
		}
		keyPressed = true;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && keyPressed == false)
	{
		if (currentTile < data.imageWidth)
		{
			currentTile -= data.imageWidth;
			currentTile = totalTiles + data.imageWidth;
		}
		else
		{
			currentTile -= data.imageWidth;
		}
		keyPressed = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && keyPressed == false)
	{
		if (currentTile > totalTiles - data.imageWidth)
		{
			currentTile += data.imageWidth;
			currentTile -= totalTiles;
		}
		else
		{
			currentTile += data.imageWidth;
		}
		keyPressed = true;
	}

	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !sf::Keyboard::isKeyPressed(sf::Keyboard::S) && !sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		keyPressed = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && keyPressed == false)
	{
		Save();
		keyPressed = true;
	}
}

void TileMap::Render()
{
	for (int i = 0; i < tiles.size(); i++)
	{
		tiles[i]->Render();
	}
}
