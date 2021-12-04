#include "InventoryClasses.h"

extern FontHolder font_holder;
extern TextureHolder texture_holder;
extern settings mysetts;
extern sf::View g_view;

Textures::ID InvItem::get_id()
{
	return id;
}

InvItem::InvItem(Textures::ID id) : id(id)
{
	amount = 0;

	sf::Texture& texture = texture_holder.get(id);
	sprite.setTexture(texture);
	sprite.setScale(0.3, 0.3);
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
		sf::Text text(kol, font_holder.get(Fonts::ID::OLD), 10);
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

BlockGUI::BlockGUI() {

	sf::Texture& texture = texture_holder.get(Textures::GUIBACK);
	Background.setTexture(texture);
}

void BlockGUI::drawGUI(int type, sf::RenderWindow& window) {
	drawBack(window);
}

void BlockGUI::drawBack(sf::RenderWindow& window) {
	window.draw(Background);
}

void BlockGUI::update_statement() {
	int scr_x = g_view.getCenter().x - 225;
	int scr_y = g_view.getCenter().y - 125;

	Background.setPosition(scr_x, scr_y);
}