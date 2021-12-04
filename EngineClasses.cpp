#include "EngineClasses.h"
//____________________________________________________________________

/** Textures chould be an hierarchy, like
*   TextureHolder <- BlocksHolder
*   TextureHolder <- ItemsHolder
*   TextureHolder <- MobsHolder
*   TextureHolder <- EffectsHolder
*
*   Moreover, it would be better to create main parent class - ResurseHolder (for music, images, fonts and etc.)
*/

extern settings mysetts;
extern FontHolder font_holder;
sf::View g_view;
TextureHolder texture_holder;

void set_view(float x, float y)
{
	
	int x_cor = x;
	int y_cor = y;

	/*sf::Vector2f cam_coord = g_view.getCenter();
	int x_dif = cam_coord.x - x;

	int y_dif = cam_coord.y - y;

	if (x_dif > 5 * 32)
		x_cor = x - 5 * 32;
	else if (x_dif < 5 * 32)
		x_cor = x + 5 * 32;
	if (y_dif > 3 * 32)
		y_cor = y - 3 * 32;
	else if (y_dif < 3 * 32)
		y_cor = y - 3 * 32;

	//if (y < 240) y = 240;
	//if (y > 554) y = 554;*/

	if (x < 220) x_cor = 220;
	if (x > WORLD_WIDTH * 32 - 420) x_cor = WORLD_WIDTH * 32 - 420;

	g_view.setCenter(x_cor + 100, y_cor);
}

//____________________________________________________________________



//____________________________________________________________________

Player::Player(): plR(texture_holder.get(Textures::VAMPIRE)), plL(texture_holder.get(Textures::VAMPIREL))
{
	character.setTexture(plR);
	character.setTextureRect(sf::IntRect(0, 0, 32, 60));
	player_position = sf::Vector2f(5*32.f, 45*32.f);
	character.setPosition(player_position);

	//p_hitbox = sf::FloatRect();
}

void Player::drawU(sf::RenderWindow& window)
{
	window.draw(character);
}

sf::Vector2f Player::get_position()
{
	return player_position;
}

sf::FloatRect Player::getGlobalBounds()
{
	return character.getGlobalBounds();
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

void Player::update_statement(const sf::Time delta_time, const World& chunk)
{
	sf::Vector2f movement(0.f, 0.f);
	sf::FloatRect nextPos;
	bool jump = false;
	const float x_crop = 8.f;


	bool smth_is_under = false;

	// getting left top coordinate of little chunk to check block collisions only there
	// lots of validation
	// a kind of optimisation

	int i1 = player_position.x;
	i1 /= 32;
	int j1 = player_position.y;
	j1 /= 32;

	// swapping, because we need:)
	int temp = i1;
	i1 = j1;
	j1 = temp;

	i1 -= 1;
	if (i1 < 0) i1 = 0;
	j1 -= 1;
	if (j1 < 0) j1 = 0;

	int LESS_HEIGHT = WORLD_HEIGHT;
	int LESS_WIDTH = WORLD_WIDTH;
	if (WORLD_HEIGHT - i1 > 4) LESS_HEIGHT = i1+4;
	if (WORLD_WIDTH -  j1 > 4) LESS_WIDTH = j1+4;

	for (int i = i1; i < LESS_HEIGHT; ++i)
		for (int j = j1; j < LESS_WIDTH; ++j)
		{
			if (chunk.tilemap[i][j] != nullptr && !(chunk.tilemap[i][j]->passable())) {
				sf::FloatRect characterBounds1 = character.getGlobalBounds();
				sf::FloatRect characterBounds(characterBounds1.left + x_crop/2, characterBounds1.top, characterBounds1.width-x_crop, characterBounds1.height);
				sf::FloatRect blockBounds = chunk.tilemap[i][j]->getGlobalBound();

				//std::cout << characterBounds.width << " " << characterBounds.height;

				nextPos = characterBounds;
				nextPos.top += gravity;


				if (blockBounds.intersects(nextPos)) {
					// std::cout << "i" << i << " j" << j << std::endl;

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

	
	
	// if (isMovingDown)  movement.y += player_speed;   // going down by pressing keys, when we have gravity? lol
	if (isMovingLeft) {
		movement.x -= player_speed; 
		character.setTexture(plL);
		character.setTextureRect(sf::IntRect(0, 0, 32, 60));
	}
	if (isMovingRigth) {
		movement.x += player_speed;
		character.setTexture(plR);
		character.setTextureRect(sf::IntRect(0, 0, 32, 60));
	}

	//int blockX = character.getPosition().y / 32;
	//int blockY = character.getPosition().x / 32; 
	//
	//if (chunk.tilemap[blockX][blockY] == nullptr)
	//	
	//
	//if (chunk.tilemap[blockX][blockY] != nullptr)
	//	if ((chunk.tilemap[blockX][blockY]->get_id() != Textures::ID::LADDER_LEFT
	//		|| chunk.tilemap[blockX][blockY]->get_id() != Textures::ID::LADDER_RIGHT) && !onGround)
	//	{
	//		movement.y += gravityAccum; gravityAccum += gravity;
	//	}
	//	else if (isMovingUp) { movement.y -= player_speed; };

	//if (chunk.tilemap[blockX][blockY] != nullptr)
	//	if (isMovingUp && (chunk.tilemap[blockX][blockY]->get_id() == Textures::ID::LADDER_LEFT || chunk.tilemap[blockX][blockY]->get_id() == Textures::ID::LADDER_RIGHT))
	//		movement.y -= player_speed;

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

	i1 = player_position.x;
	i1 /= 32;
	j1 = player_position.y;
	j1 /= 32;

	temp = i1;
	i1 = j1;
	j1 = temp;

	i1 -= 3;
	if (i1 < 0) i1 = 0;
	j1 -= 3;
	if (j1 < 0) j1 = 0;

	LESS_HEIGHT = WORLD_HEIGHT;
	LESS_WIDTH = WORLD_WIDTH;
	if (WORLD_HEIGHT - i1 > 8) LESS_HEIGHT = i1 + 8;
	if (WORLD_WIDTH - j1 > 8) LESS_WIDTH = j1 + 8;

	for (int i = i1; i < LESS_HEIGHT; ++i)
		for (int j = j1; j < LESS_WIDTH; ++j)
		{
			if (chunk.tilemap[i][j] != nullptr && !(chunk.tilemap[i][j]->passable())) {
				sf::FloatRect characterBounds1 = character.getGlobalBounds();
				sf::FloatRect characterBounds(characterBounds1.left + x_crop / 2, characterBounds1.top, characterBounds1.width - x_crop, characterBounds1.height);
				sf::FloatRect blockBounds = chunk.tilemap[i][j]->getGlobalBound();

				nextPos = characterBounds;
				nextPos.left += movement.x * delta_time.asSeconds();
				nextPos.top += movement.y * delta_time.asSeconds();

				if (blockBounds.intersects(nextPos)) {
					// std::cout << "Collision! ";

					// bottom collision
					if (characterBounds.top < blockBounds.top
						&& characterBounds.top + characterBounds.height < blockBounds.top + blockBounds.height
						&& characterBounds.left < blockBounds.left + blockBounds.width
						&& characterBounds.left + characterBounds.width > blockBounds.left
						)
					{
						onGround = true;
						// std::cout << " BOTCOL ";
						if (!jump) movement.y = 0.f;
						if (jump) { jump = false; }
						character.setPosition(characterBounds.left - x_crop / 2, blockBounds.top - characterBounds.height);
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
						character.setPosition(characterBounds.left - x_crop / 2, blockBounds.top + blockBounds.height);
					}

					// right collision
					else if (characterBounds.left < blockBounds.left
						&& characterBounds.left + characterBounds.width < blockBounds.left + blockBounds.width
						&& characterBounds.top < blockBounds.top + blockBounds.height
						&& characterBounds.top + characterBounds.height > blockBounds.top
						)
					{
						movement.x = 0.f;
						character.setPosition(blockBounds.left - characterBounds.width - x_crop/2, characterBounds.top);
					}

					// left collision
					else if (characterBounds.left > blockBounds.left
						&& characterBounds.left + characterBounds.width > blockBounds.left + blockBounds.width
						&& characterBounds.top < blockBounds.top + blockBounds.height
						&& characterBounds.top + characterBounds.height > blockBounds.top
						)
					{
						movement.x = 0.f;
						character.setPosition(blockBounds.left + blockBounds.width - x_crop/2, characterBounds.top);
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

	set_view(player_position.x, player_position.y);
}

void Player::screen_collision(int win_width, int win_height)
{
	if (character.getPosition().x < 0)
		character.setPosition(0, character.getPosition().y);

	if (character.getPosition().y < 0)
		character.setPosition(character.getPosition().x, 0);

	if (character.getPosition().x + character.getGlobalBounds().width >= WORLD_WIDTH*32 - 32)
		character.setPosition(WORLD_WIDTH*32 - character.getGlobalBounds().width - 32, character.getPosition().y);

	if (character.getPosition().y + character.getGlobalBounds().height > WORLD_HEIGHT*32)
	{
		character.setPosition(character.getPosition().x, WORLD_HEIGHT*32 - character.getGlobalBounds().height);
	}
}

//______________________________________________________________________________________________________

Game* Game::game_ptr = nullptr;  // because we can't initialize static nonconst variables inside of a class

Game::~Game() { delete game_ptr;  game_ptr = nullptr; }

void Game::raising_items()
{
	for (auto it = chunk.gitems.begin(); it != chunk.gitems.end(); it++)
	{
		// std::cout <<  "player " << player->getGlobalBounds().left << player->getGlobalBounds().top << player->getGlobalBounds().width << player->getGlobalBounds().height;
		// std::cout << "player " << (*it).getGlobalBounds().left << (*it).getGlobalBounds().top << (*it).getGlobalBounds().width << (*it).getGlobalBounds().height;
		if (player->getGlobalBounds().intersects((*it).getGlobalBounds()))
		{
			std::cout << std::endl << " raising " << (*it).get_amount() << " of " << World::get_id_name((*it).get_id()) << std::endl;
			inventory.add_item((*it).get_id(), (*it).get_amount());
			chunk.gitems.erase(it);
			break;
		}
	}
}

void Game::merging_ground_items()
{
	static int i = 0;
	bool broken = false;
	//std::cout << std::endl << " gitems length = " << chunk.gitems.size() << std::endl;
	/*for (auto it = chunk.gitems.begin(); it != chunk.gitems.end(); it++)
		std::cout << (*it).get_amount() << " ";*/
	// checking once in 20 ticks
	if (i >= 20)
	{
		i = 0;

		// reusing i here to make first iteration go untill the element before not, not fully
		for (auto it1 = chunk.gitems.begin(); it1 != chunk.gitems.end(); it1++, i++)
		{
			if (i == chunk.gitems.size() - 2) {
				i = 0;                           // making i 0, to count 20 ticks again
				break; }

			auto it_pointer = it1;
			for (auto it2 = ++it_pointer; it2 != chunk.gitems.end(); it2++)
			{
				if ((*it1).getGlobalBounds().intersects((*it2).getGlobalBounds()) && ((*it1).get_id() == (*it2).get_id()))
				{
					std::cout << std::endl << "merged";
					if ((*it1).getGlobalBounds().left < (*it2).getGlobalBounds().left)
					{
						(*it1).add_plenty((*it2).get_amount());
						chunk.gitems.erase(it2);
						broken = true;
						break;

					}
					else
					{
						(*it2).add_plenty((*it1).get_amount());
						chunk.gitems.erase(it1);
						broken = true;
						break;
					}
				}
			}
			if (broken) break;
		}
	}

	i++;
}

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
	text.setString("Welcome to our game!\nPress SPACE to start:)");
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
	
	//type: 0 - NON-item;
	//		1 - NON-FUNCTIONAL-ITEM (mostly for crafting); 
	//		2 - ITEM-BLOCK; 
	//		3 - ITEM_TOOL; 
	//		4 - ITEM_FOOD;
	texture_holder.load(Textures::VAMPIRE,  "media/textures/animals/gg_32_64.png", 0); 
	texture_holder.load(Textures::VAMPIREL, "media/textures/animals/gg_32_64l.png", 0);
	texture_holder.load(Textures::GREY,     "media/textures/animals/skeleton_grey.png", 0);
	texture_holder.load(Textures::MENU, "media/images/backgroundv1.png", 0);
	texture_holder.load(Textures::INVENTORY, "media/inventory_450_250.png", 0);
	texture_holder.load(Textures::SLOT, "media/textures/instruments/inventory_set.png", 0);

	texture_holder.load(Textures::IRON_ING, "media/textures/blocks/none.png", 1);
	texture_holder.load(Textures::ORICHALCUM_ING, "media/textures/blocks/none.png", 1);

	texture_holder.load(Textures::ORANGE,   "media/textures/blocks/block_orange/block_orange_32_32.png", 2);
	texture_holder.load(Textures::ROCK,     "media/textures/blocks/block_rock/block_rock_32_32v2.png", 2);
	texture_holder.load(Textures::DIRT,     "media/textures/blocks/block_dirt/block_dirt_32_32.png", 2);
	texture_holder.load(Textures::IRON,		"media/textures/blocks/block_iron/block_iron_32_32.png",2);
	texture_holder.load(Textures::WOOD,     "media/textures/blocks/block_tree/block_tree.png",2);
	texture_holder.load(Textures::LEAVES,   "media/textures/blocks/block_leaves/leaves_orange/leaves_orange.png",2);
	texture_holder.load(Textures::LADDER_LEFT, "media/textures/blocks/ladder/ladder_left.png", 2);
	texture_holder.load(Textures::LADDER_RIGHT, "media/textures/blocks/ladder/ladder_right.png", 2);
	texture_holder.load(Textures::LADDER, "media/textures/blocks/ladder/ladder.png", 2);
	texture_holder.load(Textures::GRASS, "media/textures/blocks/grass/grass_32_32.png", 2);
	texture_holder.load(Textures::WORKBENCH, "media/textures/blocks/block_workbench/workbench.png", 2);
	texture_holder.load(Textures::BOX, "media/textures/blocks/block_box/box.png", 2);
	texture_holder.load(Textures::BAKE, "media/textures/blocks/block_bake/bake.png", 2);


	//std::cout << "LEAVES ITEM TYPE" << texture_holder.get_type(Textures::ID::LEAVES) << std::endl;
	//std::cout << "VAMPIRE ITEM TYPE" << texture_holder.get_type(Textures::ID::VAMPIRE) << std::endl;

	font_holder.load(Fonts::OLD, "media/fonts/CyrilicOld.ttf");

	Game* game = get_game_object();

	game->run();
}

// is needed to make configurating constructors in future 
Game::Game() : g_window(sf::VideoMode(mysetts.get_width(), mysetts.get_height()), "game_project")
{
	player = new Player();
	g_view.reset(sf::FloatRect(0, 0, mysetts.get_width(), mysetts.get_height()));
	g_view.setCenter(player->getplayercoordinateX() + 100, player->getplayercoordinateY());

	sf::Texture& men = texture_holder.get(Textures::ID::MENU);
	menu.setTexture(men);
	menu.setColor(sf::Color(64, 64, 64, 255));

	// chunk.test_world();
	chunk.generate_world();
	//chunk.test_world();
	chunk.add_enemy(sf::Vector2f(5*32.f, 40*32.f), Textures::ID::GREY);

	chunk.set_block(42, 5, Textures::ID::LADDER_LEFT);
	chunk.set_block(43, 5, Textures::ID::LADDER_LEFT);

	inventory.add_item(Textures::DIRT, 10);
	inventory.add_item(Textures::ORANGE, 666);
	inventory.add_item(Textures::WOOD, 666);
	inventory.add_item(Textures::IRON, 5);
	inventory.add_item(Textures::ROCK, 40);
	inventory.add_item(Textures::LEAVES, 666);
	inventory.add_item(Textures::LADDER, 100);
}

void Game::run()
{
	/*boot_screen();

	UserInput inp;
	nick = inp.inputting(g_window);
	nick_under_head.set_string(nick);
	nick_under_head.set_coordinates(player->getplayercoordinateX(), player->getplayercoordinateY());*/

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

		default:
			mouse_processor();
			break;
		}
	}
}

void Game::update(const sf::Time delta_time)
{
	raising_items();
	merging_ground_items();
	player->screen_collision(mysetts.get_width(), mysetts.get_height());
	player->update_statement(delta_time, chunk);

	for (auto it = chunk.enemies.begin(); it != chunk.enemies.end(); it++)
		(*it).update_statement(delta_time, chunk, sf::Vector2f (player->getplayercoordinateX(), player->getplayercoordinateY()));
	for (auto it = chunk.gitems.begin(); it != chunk.gitems.end(); it++)
		(*it).update_statement(delta_time, chunk);

	nick_under_head.set_coordinates(player->getplayercoordinateX(), player->getplayercoordinateY());

	sf::Vector2f cam_pos = g_view.getCenter();
	menu.setPosition(cam_pos.x - mysetts.get_width()/2, cam_pos.y - mysetts.get_height()/2);
	inventory.update_statement();

}

void Game::render()
{
	g_window.setView(g_view);
	g_window.clear();                  // making black (without anything)
	draw_objects();                    // drawing objects 
	g_window.display();                // drawing display (screen)
}

void Game::handle_events(sf::Keyboard::Key key, bool isPressed)
{
	if (key == sf::Keyboard::W ||
		key == sf::Keyboard::A ||
		key == sf::Keyboard::S ||
		key == sf::Keyboard::D)
		player->key_reaction(key, isPressed);
	if ((static_cast<int>(key) >= sf::Keyboard::Num1 && static_cast<int>(key) <= static_cast<int>(sf::Keyboard::Num9)) && isPressed)
		inventory.key_reaction(key);

	if (!inventory.get_invent_on() && key == sf::Keyboard::E && isPressed) //Inventory on E button
		inventory.turnGUI(true);
	else if (inventory.get_invent_on() && key == sf::Keyboard::E && isPressed)
		inventory.turnGUI(false);
}

void Game::mouse_processor()
{
	// we can look for mouse coordinates on computer screeen bounds or only inside window of game, where top left corner is always 0;0
	// so I had to transfer it into global world coordinates
	sf::Vector2i mouse_pos = sf::Mouse::getPosition(g_window);
	if (mouse_pos.x < 0 || mouse_pos.y < 0 || mouse_pos.x > g_window.getSize().x - 1 || mouse_pos.y > g_window.getSize().y - 1)
		return;

	sf::Vector2f cam_pos = g_view.getCenter();
	// std::cout << g_view.getCenter().x << " " << g_view.getCenter().y << std::endl;
	sf::Vector2i real_pos = sf::Vector2i((cam_pos.x - (mysetts.get_width() / 2) + static_cast<float>(mouse_pos.x) / (static_cast<float>(g_window.getSize().x / static_cast<float>(mysetts.get_width())))),
		                                 (cam_pos.y - (mysetts.get_height() / 2) + static_cast<float>(mouse_pos.y) / (static_cast<float>(g_window.getSize().y / static_cast<float>(mysetts.get_height())))));
	

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		//std::cout << "mousecoord " << mouse_pos.x << " " << mouse_pos.y << std::endl;
		//std::cout << "realcoord " << real_pos.x << " " << real_pos.y << std::endl;
		chunk.destroy_block(real_pos, player->get_position());
	}

	else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		std::cout << "mx=" << real_pos.x / 32<< "my=" << real_pos.y / 32 << " px=" 
			<< static_cast<int>(player->getplayercoordinateX()) / 32 << "py=" 
			<< static_cast<int>(player->getplayercoordinateY()) / 32 << std::endl;

		if ((real_pos.x / 32 == static_cast<int>(player->getplayercoordinateX()) / 32) &&
			   ((real_pos.y / 32 == static_cast<int>(player->getplayercoordinateY()) / 32) ||
				(real_pos.y / 32 == static_cast<int>(player->getplayercoordinateY()) / 32 + 1)))
		{
			std::cout << "Dont place";
			chunk.place_block(real_pos, Textures::NUL, player->get_position());
		}

		else
		{
			if (chunk.place_block(real_pos, inventory.get_current(), player->get_position()))
			{
				inventory.decrease_item();
			}
		}
	}
}

void Game::draw_objects()              // so here we can order for all objects to draw themselves
{
	g_window.draw(menu);
	player->drawU(g_window);

	chunk.drawU(g_window, sf::Vector2f(player->getplayercoordinateX(), player->getplayercoordinateY()));

	for (auto it = chunk.enemies.begin(); it != chunk.enemies.end(); it++)
		(*it).drawU(g_window);
	for (auto it = chunk.gitems.begin(); it != chunk.gitems.end(); it++)
		(*it).drawU(g_window);

	nick_under_head.drawU(g_window);
	inventory.drawU(g_window);
	inventory.drawGUI(1, g_window);
}

//______________________________________________________________________________________________________