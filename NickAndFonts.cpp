#include "NickAndFonts.h"
#include "settings.h"
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

extern settings mysetts;
FontHolder::FontHolder() { }

void FontHolder::load(Fonts::ID id, const std::string& filename)
{
	std::unique_ptr<sf::Font> font(new sf::Font());
	font->loadFromFile(filename);

	gFontMap.insert(std::make_pair(id, std::move(font)));
}

sf::Font& FontHolder::get(Fonts::ID id)
{
	auto found = gFontMap.find(id);
	return *found->second;
}

const sf::Font& FontHolder::get(Fonts::ID id) const
{
	auto found = gFontMap.find(id);
	return *found->second;
}

// in future make by singleton/think about better decision
FontHolder font_holder;

//____________________________________________________________________


NickName::NickName()
{
	text.setFont(font_holder.get(Fonts::OLD));
	text.setFillColor(sf::Color::Red);
	text.setCharacterSize(10);
}

void NickName::set_coordinates(float x, float y)
{
	int x_offset = 0;

	if (nick_length <= 9) x_offset += 10 - nick_length + 3;
	else x_offset -= (nick_length % 10) / 2;
	text.setPosition(x + x_offset, y - 10);
	// std::cout << std::endl << x << " " << y << std::endl;
}

void NickName::set_string(std::string nick)
{
	text.setString(nick);
	nick_length = nick.length();
}

void NickName::drawU(sf::RenderWindow& window)
{
	window.draw(text);
}

//____________________________________________________________________

UserInput::UserInput()
{
	text.setFont(font_holder.get(Fonts::OLD));
	text.setFillColor(sf::Color::Red);
	text.setString("____________________");
	inp = "____________________";
	text.setPosition(mysetts.get_width() / 2.5f - 80, mysetts.get_height() / 2);
}

void UserInput::set_size(int size)
{
	text.setCharacterSize(size);
}

void UserInput::set_coordinates(float x, float y)
{
	text.setPosition(x, y);
}

std::string UserInput::inputting(sf::RenderWindow& window)
{
	sf::Font& font = font_holder.get(Fonts::OLD);
	sf::Text text1("", font, 20);
	//text.setOutlineColor(sf::Color::Red);
	text1.setFillColor(sf::Color::Red);
	text1.setStyle(sf::Text::Bold);
	text1.setString("Input your nickname:");
	text1.setPosition(mysetts.get_width() / 3.f, mysetts.get_height() / 2.5f);

	while (window.isOpen())
	{
		window.clear();
		window.draw(text);
		window.draw(text1);
		window.display();

		bool enter = false;
		sf::Event event;
		while (window.pollEvent(event))        // SFML is saving all events in a queue, so checking them all
		{
			sf::Keyboard::Key key;
			char symb = ' ';

			bool backspace = false;
			bool smth = false;             // if it's needed to be added
			switch (event.type)
			{
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Enter)
				{
					enter = true;
					break;
				}

				else if (event.key.code == sf::Keyboard::BackSpace)
				{
					backspace = true;
					break;
				}

				// THEIR ENUM STARTS FROM UNKNOWN = -1, A = 0 (-> B = 1). AAAAAAAAAAAAA. 
				// It can be useful to read docs smts...
				else if (static_cast<int>(event.key.code) >= 0 && static_cast<int>(event.key.code) <= static_cast<int>(sf::Keyboard::Z))
				{
					symb = static_cast<int>(event.key.code) + 97;
					smth = true;
					//std::cout << symb;
				}

				else if (static_cast<int>(event.key.code) >= 26 && static_cast<int>(event.key.code) <= static_cast<int>(sf::Keyboard::Num9))
				{
					symb = static_cast<int>(event.key.code) + 22;
					smth = true;
					//std::cout << symb;
				}
				break;

			case  sf::Event::Closed:
				window.close();
				break;
			}
			if (enter) break;

			if (backspace)
			{
				for (int i = inp.length() - 1; i >= 0; --i)
				{
					if (inp[i] != '_')
					{
						inp[i] = '_';
						break;
					}
				}
				backspace = false;
			}

			if (smth)
			{
				for (int i = 0; i < inp.length(); ++i)
				{
					if (inp[i] == '_')
					{
						inp[i] = symb;
						break;
					}
				}
				smth = false;
			}
		}
		if (enter) break;
		text.setString(inp);

	}

	std::string output{};
	for (int i = 0; i < inp.length(); ++i)
	{
		if (inp[i] != '_') output += inp[i];
		else break;
	}

	return output;
}

std::string UserInput::get_input()
{
	std::string output{};
	for (int i = 0; i < inp.length(); ++i)
	{
		if (inp[i] != '_') output += inp[i];
		else break;
	}

	return output;
}