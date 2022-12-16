#include <SFML/Graphics.hpp>
#include <iostream>
#include <filesystem>
#include <string>
#include "TileMap.h"
#include "Window.h"
#include <fstream>


TileMapData StartSettings()
{
	std::string dirPathImage = "Files\\Images";
	std::string dirPathFile = "Files\\TileMaps";
	int fileNum{};
	int choice{};
	TileMapData data;
	sf::Texture texture;

	//get file path
	std::cout << "Select the tile set you want to use\n";

	for (const auto& entry : std::filesystem::directory_iterator(dirPathImage))
	{
		fileNum++;
		std::cout << "[" << fileNum << "] - " << entry.path() << std::endl;
	}

	//std::cin >> choice;
	choice = 1;
	fileNum = 0;
	system("cls");

	for (const auto& entry : std::filesystem::directory_iterator(dirPathImage))
	{
		fileNum++;
		if (fileNum == choice)
		{
			data.imagePath = entry.path().u8string();
			break;
		}
	}
	
	//get image width in tiles
	std::cout << "How many tiles wide is the image?\n";
	//std::cin >> data.imageWidth;
	data.imageWidth = 11;
	system("cls");
	
	//get image height in tiles
	std::cout << "How many tiles high is the image?\n";
	//std::cin >> data.imageHeight;
	data.imageHeight = 11;
	system("cls");
	
	//get size of tile
	std::cout << "What is the tile side length in pixels?\n";
	//std::cin >> data.tileSideLength;
	data.tileSideLength = 17;
	system("cls");


	//get mode (load or new)
	std::cout << "Load from saved file or start new tilemap?\n[1] - Load\n[2] - New\n";
	//std::cin >> choice;
	choice = 1;
	if (choice == 1)
	{
		data.mode = OpenMode::Load;
	}
	else
	{
		data.mode = OpenMode::New;
	}
	system("cls");


	//get the file path
	if (data.mode == OpenMode::Load)
	{
		fileNum = 0;
		choice = 0;
		std::cout << "Select the file to load\n";

		for (const auto& entry : std::filesystem::directory_iterator(dirPathFile))
		{
			fileNum++;
			std::cout << "[" << fileNum << "] - " << entry.path() << std::endl;
		}

		std::cin >> choice;
		fileNum = 0;
		system("cls");

		for (const auto& entry : std::filesystem::directory_iterator(dirPathFile))
		{
			fileNum++;
			if (fileNum == choice)
			{
				data.filePath = entry.path().u8string();
				break;
			}
		}

		std::fstream inputStream;
		inputStream.open(data.filePath);

		inputStream >> data.tileMapWidth;
		inputStream >> data.tileMapHeight;
		inputStream.close();
	}

	if (data.mode == OpenMode::New)
	{
		//get tile map width
		std::cout << "What is the width of the level? (In tiles)\n";
		//std::cin >> data.tileMapWidth;
		data.tileMapWidth = 15;
		system("cls");


		//get tile map height
		std::cout << "What is the height of the level? (In tiles)\n";
		//std::cin >> data.tileMapHeight;
		data.tileMapHeight = 15;
		system("cls");

		std::string newFileName;
		std::cout << "What do you want the file name to be?";
		std::cin >> newFileName;
		data.filePath = "Files\\TileMaps\\" + newFileName + ".lvl";
	}

	std::cout << "Press LEFT and RIGHT arrows or A and D to change tiles\nPress ENTER to save the level\n\n\nPress any key to continue";
	std::cin.get();

	return data;
}


int main()
{
	TileMapData data = StartSettings();

	//get the initial window dimensions
	Window::width = data.tileMapWidth * data.tileSideLength * 1.8;
	Window::height = data.tileMapHeight * data.tileSideLength * 1.8;


	//construct the tile map
	TileMap myMap(data);


	while (Window::Get()->window->isOpen())
	{
		sf::Event event;
		while (Window::Get()->window->pollEvent(event))
		{
			if ((event.type == sf::Event::Closed) || ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)))
			{
				Window::Get()->window->close();
				break;
			}
		}

		myMap.Update();
		myMap.Render();
		Window::Get()->window->display();

	}
	

	return 0;
}