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
	Inventory();
	void update_statement();
	bool add_item_fast(Textures::ID id, int kolvo = 1); // adding items  to line. if isnt added - returns false, so we need to add item in inventory
	Textures::ID get_current();
	void key_reaction(sf::Keyboard::Key key);
	int set_current(int num);
	void decrease_item();

	void drawGUI(sf::RenderWindow& window);
	void drawInventoryBack(sf::RenderWindow& window);
	void drawGUIBack(sf::RenderWindow& window);
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
	bool inventoryContains(Textures::ID id, int numb);

	bool add_invent_item(Textures::ID id, int count = 1);
	void remove_invent_item(Textures::ID id, int count = 1);

	int getInvSlotNow(sf::Vector2i m_position);
	int getCraftSlotNow(sf::Vector2i m_position);

	void updateCrafts();
	void craftItem(int slot);
	bool isCraftable(Textures::ID id);


private:
	sf::Sprite invLine;
	int current_item = 0;

	std::array<InvItem, 30> inv_items;
	sf::Sprite inventory_sprite;
	std::array<sf::Sprite, 30> slots;
	bool in_hand = false;
	bool inventory_on = false;
	int saved_slot;

	sf::Sprite BackgroundGUI;
	std::array<sf::RectangleShape, 10> craftSlots;
	std::array<sf::Sprite, 10> craftItems;
	bool workbenchOn = false;

	std::array<InvItem, 4> armor_items; //i think we dont have time to realize armor
	std::array<sf::Sprite, 4> armor_slots; //anyway i will leave it here for better times

};
