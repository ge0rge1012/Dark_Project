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

	InvItem& operator=(const InvItem& item);

private:
	int item_type;
	Textures::ID id;
	sf::Sprite sprite;
	int amount;
};

struct Coordinate {
	int x; //same as in tilemap realization
	int y;
};

struct BoxItems {
	std::array<InvItem, 10> items;
};

struct BakeItems {
	std::array<InvItem, 2> items;
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

	void drawGUI(sf::RenderWindow& window, sf::Vector2f m_position);
	void drawInventoryBack(sf::RenderWindow& window);
	void drawGUIBack(sf::RenderWindow& window);
	void drawWorkbenchGUI(sf::RenderWindow& window);
	void drawChestGUI(sf::RenderWindow& window);
	void drawBakeGUI(sf::RenderWindow& window);
	void drawItemOptions(sf::RenderWindow& window);

	void turnGUI(bool on);
	void turn_in_hand(bool on);
	bool get_invent_on();
	bool is_in_hand();


	bool is_slot_empty(int slot);
	bool inventoryContains(Textures::ID id, int numb);

	bool add_invent_item(Textures::ID id, int count = 1);
	void remove_invent_item(Textures::ID id, int count = 1);

	int getInvSlotNow(sf::Vector2i m_position);
	int getCraftSlotNow(sf::Vector2i m_position);
	int getBakeSlotNow(sf::Vector2i m_position);
	int getBoxSlotNow(sf::Vector2i m_position);
	
	void updateCrafts();
	void craftItem(int slot);
	bool isCraftable(Textures::ID id);

	bool isItemOptionsOn();
	void turnItemOptions(bool on);
	int getOptionsSlot();
	void setOptionsSlot(int slot);
	int getChoose(sf::Vector2i m_position);
	void deleteSlotItems(int slot);

	bool isWorkbenchOn();
	void turnWorkbenchOn(bool on);

	bool isBakeOn();
	void turnBakeOn(bool on);

	bool isBoxOn();
	void turnBoxOn(bool on);

	void setTempItem(std::string arr, int slot);
	void setFromTemp(std::string arr, int slot);

	void addBoxCoords(sf::Vector2i m_position);
	void deleteBox();
	InvItem getBoxItem(int iterator);

	void setOpenedBoxID(sf::Vector2i m_position);
	void loadInOpenedBox();
	void loadFromOpenedBox();

	void addBakeCoords(sf::Vector2i m_position);
	void deleteBake();
	InvItem getBakeItem(int iterator);

	void setOpenedBakeID(sf::Vector2i m_position);
	void loadInOpenedBake();
	void loadFromOpenedBake();

private:
	std::vector<Coordinate> boxesCoords;
	std::vector<BoxItems> boxesItems;
	int openedBoxID = 0;

	std::vector<Coordinate> bakesCoords;
	std::vector<BakeItems> bakesItems;
	int openedBakeID = 0;
	int bakeTimer = 0;

	sf::Sprite invLine;
	int current_item = 0;

	std::array<InvItem, 30> inv_items;
	sf::Sprite inventory_sprite;
	std::array<sf::Sprite, 30> slots;
	sf::Sprite itemOptionsSprite;
	sf::Sprite BackgroundGUI;

	bool itemOptionsOn = false;
	bool inventoryOn = false;
	bool workbenchOn = false;
	bool bakeOn = false;
	bool boxOn = false;
	bool in_hand = false;

	int optionsSlot;

	InvItem tempItem;

	std::array<InvItem, 10> boxItems;
	std::array<sf::RectangleShape, 10> boxSlots;

	std::array<InvItem, 2> bakeItems;
	std::array<sf::RectangleShape, 2> bakeSlots;
	sf::Sprite arrow;

	std::array<sf::Sprite, 10> craftItems;
	std::array<sf::RectangleShape, 10> craftSlots;

	std::array<InvItem, 4> armor_items; //i think we dont have time to realize armor
	std::array<sf::Sprite, 4> armor_slots; //anyway i will leave it here for better times

};
