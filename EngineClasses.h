/*
Class Game is made by using a stupid realisation of sigleton pattern
This class provides only main logic of the game, all actions are determinated
Program can be started by using static function Game::start_game()
It seems strange and illogical to use so significant abstraction, but i find it an appropriate base for future
(in my sick mind)

project is needed to be code restyled!!!! Especially names of vars:D

little note for me about game stages:
1) loading all textures to TextureHolder, player can see progress bar
Also we'll need AudioHolder, I think
use sf::Image for big picture on the background of main menu
Moreover, sf::Texture/sf::Image can work with .gif!

2) Main Menu, configuraion of the game, information, may be training (why not)
here should be abilities to save and load progress

3) game process
when game is paused, process of game is stopped, but not closed (of course)



TextureHolder modifiing:
1) make it look not so palliative (may be make an hierarchy)
2) textures loading should be a separate step - not at the beginnig of Game::start_game()

*/

#pragma once
#include <cmath>
#include <vector>
#include <string>
#include <memory>
#include "settings.h"
#include "WorldClass.h"


// a little bit of shitcode (let it be, it's a joke) for fixed time steps realisation - my proud
// it gives about 60 fps, but work (may be) more stable, then standart function
const sf::Time TimePerFrame = sf::seconds(1.f / 60.f);

void set_view(float x, float y);

class NickName
{
private:
	sf::Text text;
	int nick_length = 0;

public:
	NickName();
	void set_coordinates(float x, float y);
	void set_string(std::string nick);
	void drawU(sf::RenderWindow& window);

};

class UserInput
{
private:
	std::string inp;
	sf::Text text;

public:
	UserInput();
	void set_size(int size);
	void set_coordinates(float x, float y);
	std::string inputting(sf::RenderWindow& window);
	std::string get_input();

};

namespace Fonts
{
	enum ID { OLD };
}

class FontHolder
{
private:
	std::map<Fonts::ID, std::unique_ptr<sf::Font>> gFontMap;

public:
	FontHolder();
	void load(Fonts::ID id, const std::string& filename);
	sf::Font& get(Fonts::ID id);
	const sf::Font& get(Fonts::ID id) const;
};

class InvItem
{
public:
	Textures::ID get_id();
	int get_amount();
	InvItem(Textures::ID id);
	InvItem(Textures::ID id, int kolvo);
	void add_one();
	void add_plenty(int num);
	void set_scale(sf::Vector2f scale);
    sf::Vector2f get_position();
	void substract_one();
	void set_position(sf::Vector2f pos);
	void drawU(sf::RenderWindow& window);
	int set_item_type(Textures::ID id);

private:
	int item_type;
	Textures::ID id;
	sf::Sprite sprite;
	int amount;

};

class Inventory
{
public:
	void drawU(sf::RenderWindow& window);
	Inventory();
	void update_statement();
	void add_item(Textures::ID id, int kolvo = 1);
	Textures::ID get_current();
	void key_reaction(sf::Keyboard::Key key);
	int set_current(int num);
	void decrease_item();

private:
	std::list<InvItem> items;
	std::array<sf::RectangleShape, 8> cubes;
	sf::RectangleShape inv_line;
	int current_item = 0;

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
	float player_speed = 200;      // just speed of character, stable and must not (in theory) depend on processor
	float jumpVelocity = 230;
	sf::Vector2f player_position;
	sf::FloatRect p_hitbox;
	sf::Texture& plR;
	sf::Texture& plL;

public:
	Player();

	void drawU(sf::RenderWindow& window);
	void key_reaction(sf::Keyboard::Key key, bool isPressed);
	void update_statement(const sf::Time delta_time, const World& chunk);
	void screen_collision(int win_width, int win_height);
	float getplayercoordinateX();
	float getplayercoordinateY();
	sf::Vector2f get_position();
	sf::FloatRect getGlobalBounds();
};

class Game
{

public:
	~Game();
	static void start_game();              // calling in main to configurate and start game. Doesn't need an object

	static Game* get_game_object();        // for singleton
	Game(Game& other) = delete;            // to prevent copies for singleton
	void operator=(const Game&) = delete;  // for singleton
	void raising_items();
	void merging_ground_items();

private:
	void run();                  // starting game loop (main loop)
	void boot_screen();
	std::vector<std::vector<std::string>> tiles;
	Inventory inventory;


	void process_events();                                 // observing for players action (pressing keys)
	void update(const sf::Time delta_time);                // making changes in objects (if smth happens or keys are pressed)
	void render();                                         // main rendering function, drawing everything
	void draw_objects();

	void handle_events(sf::Keyboard::Key key, bool isPressed);
	void mouse_processor();


private:
	Game();                     // private constructor for the easiest realisation of singleton pattern
	static Game* game_ptr;
	sf::RenderWindow g_window;
	std::string nick = "";
	NickName nick_under_head;
	sf::Sprite menu;

	// can be vectors of different objects
	Player* player;
	World chunk;
};