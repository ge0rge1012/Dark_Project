#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <array>
#include <map>
#include <memory>

// a little bit of shitcode (let it be) for fixed time steps realisation - my proud
// it gives about 60 fps, but work (may be) more stable, then standart function
const sf::Time TimePerFrame = sf::seconds(1.f / 60.f);
const int CHUNK_WIDTH = 20;
const int CHUNK_HEIGHT = 15;

namespace Textures
{
	enum ID { VAMPIRE, BLOCKS, ITEMS };
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
	void drawU(sf::RenderWindow& window);
	void set_coordinates(const sf::Vector2f& coord);
	sf::FloatRect getGlobalBound();
	bool passable();


private:
	bool isPassable = false;
	sf::Vector2f coordinates;
	sf::Sprite block;
};

class Chunk
{
public:
	Chunk();

	std::array<std::array<Block*, 20>, 15> tilemap;
};

class Player
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
	float player_speed = 300;      // just speed of character, stable and must not (in theory) depend on processor
	float jumpVelocity = 230;
	sf::Vector2f player_position;

public:
	Player();

	void drawU(sf::RenderWindow& window);
	void key_reaction(sf::Keyboard::Key key, bool isPressed);
	void update_statement(const sf::Time delta_time, const Chunk& chunk);
	void screen_collision(int win_width, int win_height);
};

class Game
{

public:
	~Game();
	static void start_game();              // calling in main to configurate and start game. Doesn't need an object

	static Game* get_game_object();        // for singleton
	Game(Game& other) = delete;            // to prevent copies for singleton
	void operator=(const Game&) = delete;  // for singleton

private:
	void run();                  // starting game loop (main loop)
	void boot_screen();
	std::vector<std::vector<std::string>> tiles;


	void process_events();                                 // observing for players action (pressing keys)
	void update(const sf::Time delta_time);               // making changes in objects (if smth happens or keys are pressed)
	void render();                                         // main rendering function, drawing everything
	void draw_objects();

	void handle_events(sf::Keyboard::Key key, bool isPressed);


private:
	Game();                     // private constructor for the easiest realisation of singleton pattern
	static Game* game_ptr;
	sf::RenderWindow g_window;
	const int WINDOW_WIDTH = 640;
	const int WINDOW_HEIGHT = 480;

	// can be vectors of different objects
	Player* player;
	Chunk chunk;
};