#pragma once

#include "WorldClass.h"
#include "NickAndFonts.h"


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