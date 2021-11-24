#include "EngineClasses.h"

//____________________________________________________________________

/** Textures chould be an hierarchy, like
*   TextureHolder <- BlocksHolder
*   TextureHolder <- ItemsHolder
*   TextureHolder <- MobsHolder
*   TextureHolder <- EffectsHolder
*
*   Moreover, it would be better to create main parent class - ResureHolder (for music, images, fonts and etc.)
*/

settings mysetts;

TextureHolder::TextureHolder() { }

void TextureHolder::load(Textures::ID id, const std::string& filename)
{
	std::unique_ptr<sf::Texture> texture(new sf::Texture());
	texture->loadFromFile(filename);

	gTextureMap.insert(std::make_pair(id, std::move(texture)));
}
//elencomment
sf::Texture& TextureHolder::get(Textures::ID id)
{
	auto found = gTextureMap.find(id);
	return *found->second;
}

const sf::Texture& TextureHolder::get(Textures::ID id) const
{
	auto found = gTextureMap.find(id);
	return *found->second;
}

// in future make by singleton/think about better decision
TextureHolder texture_holder;

//____________________________________________________________________

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

bool Block::passable()
{
	return isPassable;
}

Block::Block()
{
	sf::Texture& texture = texture_holder.get(Textures::BLOCKS);
	block.setTexture(texture);
}

void Block::drawU(sf::RenderWindow& window)
{
	block.setPosition(coordinates);
	window.draw(block);
	//std::cout << coordinates.x << " " << coordinates.y << std::endl;
}

void Block::set_coordinates(const sf::Vector2f& coord)
{
	coordinates = coord;
}

sf::FloatRect Block::getGlobalBound()
{
	return block.getGlobalBounds();
}

//____________________________________________________________________

Chunk::Chunk()
{
	for (int i = 0; i < 15; ++i)
		for (int j = 0; j < 20; ++j)
			tilemap[i][j] = nullptr;
}

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
	text.setPosition(mysetts.get_width()/2.5f - 80, mysetts.get_height()/2);
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
					std::cout << symb;
				}

				else if (static_cast<int>(event.key.code) >= 26 && static_cast<int>(event.key.code) <= static_cast<int>(sf::Keyboard::Num9))
				{
					symb = static_cast<int>(event.key.code) + 22;
					smth = true;
					std::cout << symb;
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

//____________________________________________________________________

Player::Player()
{
	sf::Texture& texture = texture_holder.get(Textures::VAMPIRE);
	character.setTexture(texture);
	player_position = sf::Vector2f(50.f, 390.f);
	character.setPosition(player_position);
}

void Player::drawU(sf::RenderWindow& window)
{
	window.draw(character);
}

float Player::getplayercoordinateX() {
	return player_position.x;
}

float Player::getplayercoordinateY() {	
	return player_position.y;
}

void Player::key_reaction(sf::Keyboard::Key key, bool isPressed)
{
	if (key == sf::Keyboard::W)        isMovingUp = isPressed;
	else if (key == sf::Keyboard::A)   isMovingLeft = isPressed;
	else if (key == sf::Keyboard::S)   isMovingDown = isPressed;
	else if (key == sf::Keyboard::D)   isMovingRigth = isPressed;
}

void Player::update_statement(const sf::Time delta_time, const Chunk& chunk)
{
	sf::Vector2f movement(0.f, 0.f);
	sf::FloatRect nextPos;
	bool jump = false;


	// should be done with calcing bottom block, just palliative
	bool smth_is_under = false;
	for (int i = 0; i < CHUNK_HEIGHT; ++i)
		for (int j = 0; j < CHUNK_WIDTH; ++j)
		{
			if (chunk.tilemap[i][j] != nullptr && !(chunk.tilemap[i][j]->passable())) {
				sf::FloatRect characterBounds = character.getGlobalBounds();
				sf::FloatRect blockBounds = chunk.tilemap[i][j]->getGlobalBound();

				nextPos = characterBounds;
				nextPos.top += gravity;


				if (blockBounds.intersects(nextPos)) {

					// bottom collision
					if (characterBounds.top < blockBounds.top
						&& characterBounds.top + characterBounds.height < blockBounds.top + blockBounds.height
						&& characterBounds.left < blockBounds.left + blockBounds.width
						&& characterBounds.left + characterBounds.width > blockBounds.left
						)
					{
						smth_is_under = true;
					}
				}
			}
		}
	if (!smth_is_under) onGround = false;

	// if (isMovingUp)    movement.y -= player_speed;   // isn't needed untill we have vertical stairs, jumping by negative gravity
	// if (isMovingDown)  movement.y += player_speed;   // going down by pressing keys, when we have gravity? lol
	if (isMovingLeft)  movement.x -= player_speed;
	if (isMovingRigth) movement.x += player_speed;
	if (!onGround) { movement.y += gravityAccum; gravityAccum += gravity; }

	// there is no gravitation on the floor (it can be, doesn't matter, but...)
	if (onGround) {
		gravityAccum = 0;

		if (isMovingUp) {
			gravityAccum -= jumpVelocity; 	             // negative gravitation for jumping
			isMovingUp = false;
			jump = true;
		}
	}


	for (int i = 0; i < CHUNK_HEIGHT; ++i)
		for (int j = 0; j < CHUNK_WIDTH; ++j)
		{
			if (chunk.tilemap[i][j] != nullptr && !(chunk.tilemap[i][j]->passable())) {
				sf::FloatRect characterBounds = character.getGlobalBounds();
				sf::FloatRect blockBounds = chunk.tilemap[i][j]->getGlobalBound();

				nextPos = characterBounds;
				nextPos.left += movement.x * delta_time.asSeconds();
				nextPos.top += movement.y * delta_time.asSeconds();

				if (blockBounds.intersects(nextPos)) {
					std::cout << "Collision! ";

					// bottom collision
					if (characterBounds.top < blockBounds.top
						&& characterBounds.top + characterBounds.height < blockBounds.top + blockBounds.height
						&& characterBounds.left < blockBounds.left + blockBounds.width
						&& characterBounds.left + characterBounds.width > blockBounds.left
						)
					{
						onGround = true;
						std::cout << " BOTCOL ";
						if (!jump) movement.y = 0.f;
						if (jump) { jump = false; }
						character.setPosition(characterBounds.left, blockBounds.top - characterBounds.height);
					}

					// top collision
					else if (characterBounds.top > blockBounds.top
						&& characterBounds.top + characterBounds.height > blockBounds.top + blockBounds.height
						&& characterBounds.left < blockBounds.left + blockBounds.width
						&& characterBounds.left + characterBounds.width > blockBounds.left
						)
					{
						movement.y = 0.f;
						gravityAccum = 0;
						character.setPosition(characterBounds.left, blockBounds.top + blockBounds.height);
					}

					// right collision
					else if (characterBounds.left < blockBounds.left
						&& characterBounds.left + characterBounds.width < blockBounds.left + blockBounds.width
						&& characterBounds.top < blockBounds.top + blockBounds.height
						&& characterBounds.top + characterBounds.height > blockBounds.top
						)
					{
						movement.x = 0.f;
						character.setPosition(blockBounds.left - characterBounds.width, characterBounds.top);
					}

					// left collision
					else if (characterBounds.left > blockBounds.left
						&& characterBounds.left + characterBounds.width > blockBounds.left + blockBounds.width
						&& characterBounds.top < blockBounds.top + blockBounds.height
						&& characterBounds.top + characterBounds.height > blockBounds.top
						)
					{
						movement.x = 0.f;
						character.setPosition(blockBounds.left + blockBounds.width, characterBounds.top);
					}
				}
			}
		}


	if (jump) {
		movement.y += gravityAccum;
		onGround = false;
	}
	// distance = speed * time
	character.move(movement * delta_time.asSeconds());
	player_position.x = character.getGlobalBounds().left;
	player_position.y = character.getGlobalBounds().top;
}

void Player::screen_collision(int win_width, int win_height)
{
	if (character.getPosition().x < 0)
		character.setPosition(0, character.getPosition().y);

	if (character.getPosition().y < 0)
		character.setPosition(character.getPosition().x, 0);

	if (character.getPosition().x + character.getGlobalBounds().width > win_width)
		character.setPosition(win_width - character.getGlobalBounds().width, character.getPosition().y);

	if (character.getPosition().y + character.getGlobalBounds().height > win_height)
	{
		character.setPosition(character.getPosition().x, win_height - character.getGlobalBounds().height);
	}
}

//______________________________________________________________________________________________________

Game* Game::game_ptr = nullptr;  // because we can't initialize static nonconst variables inside of a class

Game::~Game() { delete game_ptr;  game_ptr = nullptr; }

Game* Game::get_game_object()
{
	if (game_ptr == nullptr) game_ptr = new Game();
	return game_ptr;
}

void Game::boot_screen()
{
	sf::Font& font = font_holder.get(Fonts::OLD);
	sf::Text text("", font, 30);
	//text.setOutlineColor(sf::Color::Red);
	text.setFillColor(sf::Color::Red);
	text.setStyle(sf::Text::Bold);
	text.setString("����� ���������� � ���� ����!\nPress SPACE to start:)");
	text.setPosition(mysetts.get_width()/4.5f, mysetts.get_height()/2.4f);
	

	while (g_window.isOpen())
	{
		// dont do this, just example of using image
		/*sf::Image booting;
		booting.loadFromFile("media/images/booter.png");
		sf::Texture bt;
		bt.loadFromImage(booting);
		sf::Sprite sprbt;
		sprbt.setTexture(bt);
		sprbt.setPosition(0.f, 0.f);


		g_window.clear();
		g_window.draw(sprbt);
		g_window.display();*/

		g_window.clear();
		g_window.draw(text);
		g_window.display();

		bool space = false;
		sf::Event event;
		while (g_window.pollEvent(event))        // SFML is saving all events in a queue, so checking them all
		{

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
			{
				space = true;
				break;
			}

		}
		if (space) break;
	}
}

void Game::start_game()
{
	// make configurations: game mode, choose character model and etc.
	// so main menu will be opened here in future
	texture_holder.load(Textures::VAMPIRE, "media/textures/animals/gg_32_64.png"); //changed texture
	texture_holder.load(Textures::BLOCKS, "media/textures/blocks/ground.png");
	font_holder.load(Fonts::OLD, "media/fonts/CyrilicOld.ttf");

	Game* game = get_game_object();

	game->run();
}

// is needed to make configurating constructors in future 
Game::Game() : g_window(sf::VideoMode(mysetts.get_width(), mysetts.get_height()), "game_project")
{
	player = new Player();

	// generating map for tests
	// of course, it wouldnt be here

	for (int i = 14; i < 15; ++i)
		for (int j = 0; j < 20; ++j)
		{
			chunk.tilemap[i][j] = new Block;
			chunk.tilemap[i][j]->set_coordinates(sf::Vector2f(j * 32.f, i * 32.f));
		}
	chunk.tilemap[13][15] = new Block;
	chunk.tilemap[13][15]->set_coordinates(sf::Vector2f(15 * 32.f, 13 * 32.f));

	chunk.tilemap[13][16] = new Block;
	chunk.tilemap[13][16]->set_coordinates(sf::Vector2f(16 * 32.f, 13 * 32.f));

	chunk.tilemap[12][16] = new Block;
	chunk.tilemap[12][16]->set_coordinates(sf::Vector2f(16 * 32.f, 12 * 32.f));

	chunk.tilemap[11][17] = new Block;
	chunk.tilemap[11][17]->set_coordinates(sf::Vector2f(17 * 32.f, 11 * 32.f));

	chunk.tilemap[9][14] = new Block;
	chunk.tilemap[9][14]->set_coordinates(sf::Vector2f(14 * 32.f, 9 * 32.f));

	chunk.tilemap[13][19] = new Block;
	chunk.tilemap[13][19]->set_coordinates(sf::Vector2f(19 * 32.f, 13 * 32.f));

	chunk.tilemap[9][12] = new Block;
	chunk.tilemap[9][12]->set_coordinates(sf::Vector2f(12 * 32.f, 9 * 32.f));

	chunk.tilemap[9][10] = new Block;
	chunk.tilemap[9][10]->set_coordinates(sf::Vector2f(10 * 32.f, 9 * 32.f));

	chunk.tilemap[10][19] = new Block;
	chunk.tilemap[10][19]->set_coordinates(sf::Vector2f(19 * 32.f, 10 * 32.f));

	chunk.tilemap[10][9] = new Block;
	chunk.tilemap[10][9]->set_coordinates(sf::Vector2f(9 * 32.f, 10 * 32.f));

	chunk.tilemap[11][7] = new Block;
	chunk.tilemap[11][7]->set_coordinates(sf::Vector2f(7 * 32.f, 11 * 32.f));

	chunk.tilemap[12][6] = new Block;
	chunk.tilemap[12][6]->set_coordinates(sf::Vector2f(6 * 32.f, 12 * 32.f));

	chunk.tilemap[13][5] = new Block;
	chunk.tilemap[13][5]->set_coordinates(sf::Vector2f(5 * 32.f, 13 * 32.f));
}

void Game::run()
{
	boot_screen();

	UserInput inp;
	nick = inp.inputting(g_window);
	nick_under_head.set_string(nick);
	nick_under_head.set_coordinates(player->getplayercoordinateX(), player->getplayercoordinateY());

	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (g_window.isOpen())
	{
		process_events();
		timeSinceLastUpdate += clock.restart();

		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;
			process_events();
			update(TimePerFrame);
		}

		render();
	}
}

void Game::process_events()
{
	sf::Event event;
	while (g_window.pollEvent(event))        // SFML is saving all events in a queue, so checking them all
	{
		switch (event.type)
		{
		case sf::Event::KeyPressed:
			handle_events(event.key.code, true);
			break;

		case sf::Event::KeyReleased:
			handle_events(event.key.code, false);
			break;

		case  sf::Event::Closed:
			g_window.close();
			break;
		}
	}
}

void Game::update(const sf::Time delta_time)
{
	player->screen_collision(mysetts.get_width(), mysetts.get_height());
	player->update_statement(delta_time, chunk);
	nick_under_head.set_coordinates(player->getplayercoordinateX(), player->getplayercoordinateY());
}

void Game::render()
{
	g_window.clear();                  // making black (without anything)
	draw_objects();                    // drawing objects (for now only one)
	g_window.display();                // drawing display (screen)
}

void Game::handle_events(sf::Keyboard::Key key, bool isPressed)
{
	if (key == sf::Keyboard::W ||
		key == sf::Keyboard::A ||
		key == sf::Keyboard::S ||
		key == sf::Keyboard::D)
		player->key_reaction(key, isPressed);
}

void Game::draw_objects()              // so here we can order for all objects to draw themselves
{
	player->drawU(g_window);
	nick_under_head.drawU(g_window);

	// this shit is needed to be drawed not here
	for (int i = 0; i < 15; ++i)
		for (int j = 0; j < 20; ++j)
			if (chunk.tilemap[i][j] != nullptr)
				chunk.tilemap[i][j]->drawU(g_window);
}

//______________________________________________________________________________________________________