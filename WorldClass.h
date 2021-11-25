#pragma once
#include <array>
#include <SFML/Graphics.hpp>
#include <iostream>

const int WORLD_WIDTH = 1000;
const int WORLD_HEIGHT = 100;

namespace Textures
{
	enum ID { VAMPIRE, GRASS, ITEMS };
}

class TextureHolder
{
private:
	std::map<Textures::ID, std::unique_ptr<sf::Texture>> gTextureMap;

public:
	TextureHolder();
	void load(Textures::ID id, const std::string& filename);
	sf::Texture& get(Textures::ID id);
	const sf::Texture& get(Textures::ID id) const;
};

class Block
{
public:
	Block();
	Block(Textures::ID id);
	void drawU(sf::RenderWindow& window);
	void set_coordinates(const sf::Vector2f& coord);
	sf::FloatRect getGlobalBound();
	bool passable();
	bool breakable();


private:
	int id = 0;
	bool isPassable = false;
	bool isBreakable = false;

	sf::Vector2f coordinates;
	sf::Sprite block;
};

class World {
public:
	std::array<std::array<Block*, WORLD_WIDTH>, WORLD_HEIGHT> tilemap;

	World();
	void create_surface();
	void create_cave(int x, int y);
	void create_dungeon();
	void generate_world();
	void set_block(int x, int y, Textures::ID id);
	void delete_block(int x, int y);


	// for test location
	void test_world();
};