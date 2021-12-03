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
