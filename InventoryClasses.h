#pragma once

#include "WorldClass.h"
#include "NickAndFonts.h"


class InvItem
{
public:
	Textures::ID get_id();
	void set_item_id(Textures::ID id);
	void set_amount(int amount);
	int get_amount();
	InvItem();
	InvItem(Textures::ID id);
	InvItem(Textures::ID id, int kolvo);
	void add_one();
	void add_plenty(int num);
	void set_scale(sf::Vector2f scale);
	sf::Vector2f get_position();
	void substract_one();
	void set_position(sf::Vector2f pos);
	void drawU(sf::RenderWindow& window);
	int get_item_type();
	sf::Sprite get_sprite();
	void set_sprite(sf::Sprite new_sprite);

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

	void drawGUI(int type, sf::RenderWindow& window);
	void drawBack(sf::RenderWindow& window);
	void drawWorkbenchGUI(sf::RenderWindow& window);
	void drawChestGUI(sf::RenderWindow& window);
	void drawBakeGUI(sf::RenderWindow& window);

	void turnGUI(bool on);
	void turn_in_hand(bool on);
	bool get_invent_on();
	bool is_in_hand();
	void save_slot(int slot);
	int get_save_slot();
	void change_slots(int new_slot, int old_slot);
	bool is_slot_empty(int slot);

	void add_invent_item(Textures::ID id, int kolvo = 1);

	int get_pos_now(sf::Vector2i m_position);
	void get_in_hand(sf::Vector2i m_position);
private:
	std::list<InvItem> items;
	std::array<sf::RectangleShape, 8> cubes;
	sf::RectangleShape inv_line;
	int current_item = 0;

	std::array<InvItem, 20> inv_items;
	sf::Sprite inventory_sprite;
	std::array<sf::Sprite, 30> slots;
	bool in_hand = false;
	bool inventory_on = false;
	int saved_slot;

};
