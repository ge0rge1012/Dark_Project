#pragma once
#include <array>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>

const int WORLD_WIDTH = 1000;
const int WORLD_HEIGHT = 130;

class Enemy;

namespace Textures
{
	enum ID { VAMPIRE, GRASS, ITEMS, GREY };
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
	std::list <Enemy> enemies;

	World();
	void create_surface();
	void create_cave(int x, int y);
	void create_mountain();
	void spawn_dungeon();
	void generate_world();
	void set_block(int x, int y, Textures::ID id);
	void delete_block(int x, int y);

	void add_enemy(sf::Vector2f position, Textures::ID id);
	// for test location
	void test_world();
};

//.............................................
class Enemy
{
private:
	sf::Sprite character;

	bool isMovingUp = false;
	bool isMovingDown = false;
	bool isMovingLeft = false;
	bool isMovingRigth = false;
	bool onGround = false;

	float gravity = 9;             // velocity of gravityAccum increasing
	float gravityAccum = 0;        // the speed of falling down because of gravitation, is increasing
	float enemy_speed = 200;      // just speed of character, stable and must not (in theory) depend on processor
	float jumpVelocity = 230;
	sf::Vector2f enemy_position;
	sf::FloatRect e_hitbox;
	Textures::ID type;

public:
	Enemy(sf::Vector2f position, Textures::ID id);

	void drawU(sf::RenderWindow& window);
	void update_statement(const sf::Time delta_time, const World& chunk);
	void screen_collision(int win_width, int win_height);
	void key_reaction(sf::Keyboard::Key key, bool isPressed);
	float getenemycoordinateX();
	float getenemycoordinateY();
};