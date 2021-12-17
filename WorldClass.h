#pragma once
#include <array>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <map>
#include <cmath>
#include <vector>
#include <string>
#include <memory>
#include "settings.h"

const int WORLD_WIDTH = 1000;
const int WORLD_HEIGHT = 150;


class Enemy;
class GroundItem;

namespace Textures
{
	enum ID { NUL = 0, MENU, VAMPIRE, VAMPIRE1, VAMPIRE2, VAMPIRE3, VAMPIRE4, VAMPIRE5, VAMPIRE6, VAMPIRE7,
		ORANGE, ITEMS, GREY, ROCK, DIRT, IRON, WOOD, LEAVES, LADDER_LEFT,
		LADDER_RIGHT, LADDER, GRASS, IRON_ING, ORICHALCUM, ORICHALCUM_ING,
		WORKBENCH, BOX, BAKE, INVENTORY, SLOT, EARTHBACKGROUND, GUIBACK, LINE, 
		STICK, PICK_TR, PICK_ST, PICK_IR, PICK_OR, SWORD_TR, SWORD_ST, 
		SWORD_IR, SWORD_OR, ITEM_OPTIONS, ARROW };
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

//struct IDHolder {
//	std::unique_ptr<sf::Texture> texture_pointer;
//	int type;
//};

class TextureHolder {

private:
	std::map<Textures::ID, std::unique_ptr<sf::Texture>> gTextureMap;
	std::map<Textures::ID, int> type_map;

public:
	TextureHolder();
	void load(Textures::ID id, const std::string& filename, int type);  //type: 0 - NON-item;
	sf::Texture& get(Textures::ID id);									// 1 - just ITEM without any functional; 
	const sf::Texture& get(Textures::ID id) const;						//2 - ITEM-BLOCK; 3 - ITEM_TOOL; 4-ITEM_FOOD;
	void set_type(Textures::ID id, int type);
	const int get_type (Textures::ID id) const;
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
	bool functional();

private:
	Textures::ID id = Textures::ID::NUL;
	bool isPassable = false;
	bool isBreakable = false;
	bool isFunctional = false;

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
	void create_cave_right(int x, int y);
	void create_cave_left(int x, int y);
	void create_mountain();
	void spawn_dungeon();
	void spawn_resources();
	void spawn_tree(int x, int y);
	void generate_world();
	void set_block(int x, int y, Textures::ID id);
	void delete_block(int x, int y);
	void destroy_block(sf::Vector2i m_pos, sf::Vector2f p_pos);
	bool place_block(sf::Vector2i m_pos, Textures::ID id, sf::Vector2f p_pos);
	void add_ground_item(Textures::ID id, sf::Vector2f coord, int kolvo);
	void add_ground_item(Textures::ID id, sf::Vector2f coord);
	static std::string get_id_name(Textures::ID id);
	bool isWorkbenchClose(int playerX, int playerY);

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
	GroundItem(Textures::ID id, sf::Vector2f coord, int kolvo);
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
