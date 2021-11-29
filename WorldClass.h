#pragma once
#include <array>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>
#include <string>
#include <stdlib.h>
#include <time.h>

const int WORLD_WIDTH = 1000;
const int WORLD_HEIGHT = 150;


class Enemy;
class GroundItem;

namespace Textures
{
	enum ID { NUL = 0, MENU, VAMPIRE, VAMPIREL, ORANGE, ITEMS, GREY, ROCK, DIRT, IRON, WOOD, LEAVES };
}

class Randomizer {

public:
	int N[1000];
	int counter = 0;
	Randomizer();
	void initialize();
	int get_random(int min, int max);
	int get_random();
};

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
	Textures::ID get_id();
	void set_id(Textures::ID id);

private:
	Textures::ID id = Textures::ID::NUL;
	bool isPassable = false;
	bool isBreakable = false;

	sf::Vector2f coordinates;
	sf::Sprite block;
};

class World {
public:
	std::array<std::array<Block*, WORLD_WIDTH>, WORLD_HEIGHT> tilemap;
	std::list <Enemy> enemies;
	std::list<GroundItem> gitems;
	int line_of_horizon = 49;

	World();
	void create_surface();
	void create_cave(int x, int y);
	void create_mountain();
	void spawn_dungeon();
	void spawn_resources();
	void spawn_tree(int x, int y);
	void generate_world();
	void set_block(int x, int y, Textures::ID id);
	void delete_block(int x, int y);
	void destroy_block(sf::Vector2i pos, sf::Vector2f p_pos);
	bool place_block(sf::Vector2i pos, Textures::ID id, sf::Vector2f p_pos);
	void add_ground_item(Textures::ID id, sf::Vector2f coord);
	static std::string get_id_name(Textures::ID id);

	void add_enemy(sf::Vector2f position, Textures::ID id);
	// for test location
	void test_world();
	void drawU(sf::RenderWindow& window, sf::Vector2f p_coordinates);
};

//____________________________________________________________________

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
	void update_statement(const sf::Time delta_time, const World& chunk, sf::Vector2f p_coor);
	static bool may_jump_left(const World& chunk, sf::Vector2f p_coor);
	static bool may_jump_right(const World& chunk, sf::Vector2f p_coor);
	void screen_collision(int win_width, int win_height);
	void key_reaction(sf::Keyboard::Key key, bool isPressed);
	float getenemycoordinateX();
	float getenemycoordinateY();
};

//____________________________________________________________________

class GroundItem
{
public:
	Textures::ID get_id();
	int get_amount();
	GroundItem(Textures::ID id, sf::Vector2f coord);
	void add_plenty(int num);
	sf::Vector2f get_position();
	void set_position(sf::Vector2f pos);
	void drawU(sf::RenderWindow& window);
	void update_statement(const sf::Time delta_time, const World& chunk);
	sf::FloatRect getGlobalBounds();

private:
	Textures::ID id;
	sf::Sprite sprite;
	int amount;

	int gravity = 8;
	int gravityAccum = 0;
	bool onGround = false;
};