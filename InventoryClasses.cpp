#include "InventoryClasses.h"

extern FontHolder font_holder;
extern TextureHolder texture_holder;
extern settings mysetts;
extern sf::View g_view;

Textures::ID InvItem::get_id()
{
	return id;
}

InvItem::InvItem() {
}

void InvItem::set_item_id(Textures::ID id) {
	sf::Texture& texture = texture_holder.get(id);
	sprite.setTexture(texture);
	sprite.setScale(0.3, 0.3);

	this->id = id;

	item_type = texture_holder.get_type(id);
}

void InvItem::set_sprite(sf::Sprite new_sprite) {
	sprite = new_sprite;
}

InvItem::InvItem(Textures::ID id) : id(id)
{
	amount = 0;

	sf::Texture& texture = texture_holder.get(id);
	sprite.setTexture(texture);
	sprite.setScale(0.3, 0.3);

	this->id = id;

	item_type = texture_holder.get_type(id);
}

void InvItem::set_amount(int amount) {
	this->amount = amount;
}

int InvItem::get_amount()
{
	return amount;
}

void InvItem::add_one()
{
	amount++;
}

void InvItem::set_position(sf::Vector2f pos)
{
	sprite.setPosition(pos);
}

void InvItem::drawU(sf::RenderWindow& window)
{
	window.draw(sprite);
}

void InvItem::substract_one()
{
	amount--;
}

void InvItem::add_plenty(int num)
{
	amount += num;
}

void InvItem::set_scale(sf::Vector2f scale)
{
	sprite.setScale(scale);
}

sf::Vector2f InvItem::get_position()
{
	return sprite.getPosition();
}

InvItem::InvItem(Textures::ID id, int kolvo) : id(id)
{
	amount = kolvo;

	sf::Texture& texture = texture_holder.get(id);
	sprite.setTexture(texture);
	sprite.setScale(0.3, 0.3);
}

int InvItem::get_item_type() {
	return item_type;
}

sf::Sprite InvItem::get_sprite() {
	return sprite;
}



//____________________________________________________________________

void Inventory::key_reaction(sf::Keyboard::Key key)
{
	bool first_current = false;
	if (current_item <= 0 || current_item > 8)
	{
		current_item = key;
		first_current = true;
	}

	if (first_current)
	{
		current_item = static_cast<int>(key) - 26;
		cubes[current_item - 1].setOutlineColor(sf::Color::Red);
	}

	else
	{
		int prev_cur = current_item;
		cubes[current_item - 1].setOutlineColor(sf::Color::Green);
		current_item = static_cast<int>(key) - 26;
		cubes[current_item - 1].setOutlineColor(sf::Color::Red);

		int i = 1;
		for (auto it = items.begin(); it != items.end(); it++)
		{
			if (i == prev_cur)
			{
				(*it).set_scale(sf::Vector2f(0.3, 0.3));
				break;
			}
			++i;
		}
	}


	int i = 1;
	for (auto it = items.begin(); it != items.end(); it++)
	{
		if (i == current_item)
		{
			(*it).set_scale(sf::Vector2f(0.5, 0.5));
			break;
		}
		++i;
	}
}

void Inventory::update_statement()
{
	int scr_x = g_view.getCenter().x - mysetts.get_width() / 2;
	int scr_y = g_view.getCenter().y - mysetts.get_height() / 2;

	int pos_x = 164;
	int pos_y = 448;
	for (int i = 0; i < 8; ++i)
	{
		cubes[i].setPosition(sf::Vector2f(scr_x + pos_x, scr_y + pos_y));
		pos_x += 40;
	}

	inv_line.setPosition(sf::Vector2f(scr_x + 120, scr_y + 440));

	for (auto it = items.begin(); it != items.end(); it++)
		if ((*it).get_amount() == 0)
			items.erase(it);

	int i = 0;
	for (auto it = items.begin(); it != items.end(); it++)
	{
		if (i == current_item - 1)
			(*it).set_position(cubes[i].getPosition() + sf::Vector2f(4.f, 4.f));
		else
			(*it).set_position(cubes[i].getPosition() + sf::Vector2f(10.f, 10.f));

		++i;
		if (i == 8) break;
	}

	int back_x = g_view.getCenter().x - 225;
	int back_y = g_view.getCenter().y - 125;

	inventory_sprite.setPosition(back_x, back_y);

	int slot_x = back_x + 14;
	int slot_y = back_y + 111;

	//for (int i = 0; i < 3; i++) { //some shitcode of right positioning inventory slots, cz our designer fucked up with sizes.
	//	slot_x = back_x + 13;
	//	for (int j = 0; j < 10; j++) {
	//		if ((i * 10 + j > 0 && i * 10 + j < 9) || (i * 10 + j > 10 && i * 10 + j < 19) || i * 10 + j==20)
	//			slots[i * 10 + j].setPosition(slot_x + 1, slot_y);
	//		else if (i*10+j>20 && i*10+j<30)
	//			slots[i * 10 + j].setPosition(slot_x+2, slot_y);
	//		else 
	//			slots[i * 10 + j].setPosition(slot_x, slot_y);
	//		slot_x += 43;
	//	}
	//	if (i == 0) slot_y += 43;
	//	else slot_y += 53;
	//}

	for (int i = 0; i < 3; i++) { //right code for slots
		slot_x = back_x + 13;
		for (int j = 0; j < 10; j++) {
			slots[i * 10 + j].setPosition(slot_x, slot_y);
			slot_x += 43;
		}
		if (i == 0) slot_y += 43;
		else slot_y += 51;
	}

	slot_y = back_y + 10;

	for (int i = 0; i < 2; i++) { 
		slot_x = back_x + 31;
		for (int j = 0; j < 2; j++) {
			armor_slots[i * 2 + j].setPosition(slot_x, slot_y);
			slot_x += 43;
		}
		slot_y += 43;
	}

	if (inventory_on) {
		for (int i = 0; i < 20; i++) {
			inv_items[i].set_position(slots[i].getPosition() + sf::Vector2f(13.f, 15.f));
		}
	}

	if (inventory_on) {
		i = 0;
		for (auto it = items.begin(); it != items.end(); it++)
		{
			if (i == current_item - 1)
				(*it).set_position(slots[20 + i].getPosition() + sf::Vector2f(13.f, 15.f));
			else
				(*it).set_position(slots[20 + i].getPosition() + sf::Vector2f(13.f, 15.f));

			++i;
			if (i == 8) break;
		}
	}

}

void Inventory::decrease_item()
{
	if (current_item > items.size()) return;

	int i = 1;
	for (auto it = items.begin(); it != items.end(); it++)
	{
		if (i == current_item)
		{
			if ((*it).get_amount() == 1)
				items.erase(it);

			else
				(*it).substract_one();
			return;
		}
		++i;
	}
}

int Inventory::set_current(int num)
{
	return 1;
}

Textures::ID Inventory::get_current()
{
	if (current_item > items.size() || current_item <= 0)
		return Textures::ID::NUL;

	int i = 1;
	for (auto it = items.begin(); it != items.end(); it++)
	{
		if (i == current_item)
		{
			return (*it).get_id();
		}
		++i;
	}
	return Textures::ID::DIRT;

}

Inventory::Inventory() 
{
	for (int i = 0; i < 8; ++i)
	{
		cubes[i].setSize(sf::Vector2f(32.f, 32.f));
		cubes[i].setFillColor(sf::Color(128, 128, 128));
		cubes[i].setOutlineThickness(3.f);
		cubes[i].setOutlineColor(sf::Color::Green);

	}
	// cubes[0].setOutlineColor(sf::Color::Red);

	inv_line.setSize(sf::Vector2f(400.f, 40.f));
	inv_line.setFillColor(sf::Color::Yellow);

	sf::Texture& texture = texture_holder.get(Textures::INVENTORY);
	inventory_sprite.setTexture(texture);

	sf::Texture& texture_slot = texture_holder.get(Textures::SLOT);
	for (int i = 0; i < 30; i++) {
		slots[i].setTexture(texture_slot);
	}

	for (int i = 0; i < 20; i++) {
		inv_items[i].set_item_id(Textures::ID::DIRT);
	}

	for (int i = 0; i < 4; i++) { //��������� 30 ������
		armor_slots[i].setTexture(texture_slot);
	}
}

void Inventory::drawU(sf::RenderWindow& window)
{
	window.draw(inv_line);
	for (auto it = cubes.begin(); it != cubes.end(); it++)
		window.draw(*it);

	int i = 0;
	for (auto it = items.begin(); it != items.end(); it++)
	{
		std::string kol = std::to_string((*it).get_amount());
		sf::Text text(kol, font_holder.get(Fonts::ID::OLD), 8);
		text.setFillColor(sf::Color::Black);

		if (i == current_item - 1)
			text.setPosition((*it).get_position() + sf::Vector2f(16.f, -12.f));
		else
			text.setPosition((*it).get_position() + sf::Vector2f(10.f, -12.f));
		((*it).drawU(window));
		window.draw(text);
		i++;
		;		if (i == 8) break;
	}
}

void Inventory::add_item(Textures::ID id, int kolvo)
{
	if (kolvo <= 0) return;
	bool added_to_end = false;
	auto iter = items.end();
	for (auto it = items.begin(); it != items.end(); it++)
		if ((*it).get_id() == id)
			iter = it;
	if (iter != items.end())
		(*iter).add_plenty(kolvo);
	else
	{
		items.push_back(InvItem(id, 0));
		added_to_end = true;
	}

	if (added_to_end)
	{
		int i = 0;
		for (auto it = items.begin(); it != items.end(); it++)
		{
			if (i == items.size() - 1)
			{
				(*it).add_plenty(kolvo);
			}
			++i;
		}
	}
	// std::cout << "amount" << (*(items.begin())).get_amount();
}

void Inventory::add_invent_item(Textures::ID id, int kolvo) {

	bool isAdded = false;
	for (int i = 0; i < 20; i++) {
		if (inv_items[i].get_id() == id)
		{
			inv_items[i].add_plenty(kolvo);
			isAdded = true;
			break;
		}
		if (!isAdded)
		{
			if (inv_items[i].get_id() == Textures::NUL || inv_items[i].get_amount() == 0) {
				inv_items[i].set_item_id(id);
				inv_items[i].set_amount(kolvo);
				break;
			}
		}
	}
}

void Inventory::drawGUI(int type, sf::RenderWindow& window) {
	if (inventory_on) //���� ��������� ������� (�������� ������� ��� ������� �)
	{
		drawBack(window); //��������� ��������� (���� ���������)
		for (int i = 0; i < 30; i++) { //��������� 30 ������
			window.draw(slots[i]);
		}
		for (int i = 0; i < 4; i++) { //��������� 30 ������
			window.draw(armor_slots[i]);
		}
		for (int i = 0; i < 20; i++) { //���� � ����� ������ 0 �� �������������� �� ��� ����� � ��������������� ����� ���������
			if(inv_items[i].get_amount()>0)
				inv_items[i].drawU(window);
		}
		for (int i = 0; i < 20; i++) { //����� �������������� ������� � �����������
			if (inv_items[i].get_amount() > 0) {
				std::string kol = std::to_string(inv_items[i].get_amount());
				sf::Text text(kol, font_holder.get(Fonts::ID::OLD), 8);
				text.setFillColor(sf::Color::Black);
				text.setPosition(inv_items[i].get_position() + sf::Vector2f(10.f, -12.f));
				window.draw(text);
			}
		}

	}
}

int Inventory::get_pos_now(sf::Vector2i m_position) {
	int slot_now = 21;
	for (int i = 0; i < 20; i++) {
		if ((slots[i].getPosition().x < m_position.x) && (slots[i].getPosition().x + 38 > m_position.x)
			&& (slots[i].getPosition().y < m_position.y) && (slots[i].getPosition().y + 38 > m_position.y))
			slot_now = i;
	}
	if (slot_now<20)
		return slot_now;
}

void Inventory::change_slots(int new_slot, int old_slot) {
	Textures::ID temp_id = inv_items[new_slot].get_id();
	int temp_amount = inv_items[new_slot].get_amount();
	sf::Sprite temp_sprite = inv_items[new_slot].get_sprite();

	inv_items[new_slot].set_item_id(inv_items[old_slot].get_id());
	//inv_items[new_slot].set_id(inv_items[old_slot].get_id());
	inv_items[new_slot].set_sprite(inv_items[old_slot].get_sprite());
	inv_items[new_slot].set_amount(inv_items[old_slot].get_amount());

	inv_items[old_slot].set_item_id(temp_id);
	//inv_items[old_slot].set_id(temp_id);
	inv_items[old_slot].set_sprite(temp_sprite);
	inv_items[old_slot].set_amount(temp_amount);
}

bool Inventory::is_slot_empty(int slot) {
	if (inv_items[slot].get_amount() == 0) return true;
	else return false;
}

void Inventory::drawBack(sf::RenderWindow& window) {
	window.draw(inventory_sprite);
}

void Inventory::turnGUI(bool on) {
	inventory_on = on;
}

bool Inventory::get_invent_on() {
	return inventory_on;
}

bool Inventory::is_in_hand() {
	return in_hand;
}
void Inventory::turn_in_hand(bool on) {
	in_hand = on;
}

int Inventory::get_save_slot() {
	return saved_slot;
}

void Inventory::save_slot(int slot) {
	saved_slot = slot;
}