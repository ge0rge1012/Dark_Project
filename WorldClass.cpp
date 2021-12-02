#include "WorldClass.h"

TextureHolder::TextureHolder() { }
Randomizer random_number;

Randomizer::Randomizer() {
	srand(time(NULL));
}

void Randomizer::initialize() {
	unsigned value = unsigned(std::time(nullptr));
	srand(time(NULL));
	for (int i = 0; i < 1000; ++i)
	{
		N[i] = rand() % 10000;
		//std::cout << N[i] << std::endl;
	}
}

int Randomizer::get_random(int min, int max) {
	if (counter < 999) counter++;
	else counter = 0;
	//std::cout << counter << " " << min + N[counter] % (max - min + 1) << std::endl;
	return min + N[counter] % (max - min + 1);
}

int Randomizer::get_random() {
	srand(time(NULL));
	return rand();
}

void TextureHolder::load(Textures::ID id, const std::string& filename, int type)
{
	std::unique_ptr<sf::Texture> texture(new sf::Texture());
	texture->loadFromFile(filename);

	gTextureMap.insert(std::make_pair(id, std::move(texture)));
	set_type(id, type);
}

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

void TextureHolder::set_type(Textures:: ID id, int type) {
	type_map.insert(std::make_pair(id, type));
}

const int TextureHolder::get_type(Textures::ID id) const 
{
	auto found = type_map.find(id);
	return found->second;
}



// in future make by singleton/think about better decision
extern TextureHolder texture_holder;

bool Block::passable()
{
	return isPassable;
}

bool Block::breakable()
{
	return isBreakable;
}

// let standart texture be just dirt with ORANGE
Block::Block()
{
	sf::Texture& texture = texture_holder.get(Textures::ORANGE);
	id = Textures::ORANGE;
	block.setTexture(texture);
}

Block::Block(Textures::ID id):id(id)
{
	sf::Texture& texture = texture_holder.get(id);
	this->id = id;
	block.setTexture(texture);
}

void Block::drawU(sf::RenderWindow& window)
{
	window.draw(block);
	//std::cout << coordinates.x << " " << coordinates.y << std::endl;
}

void Block::set_coordinates(const sf::Vector2f& coord)
{
	coordinates = coord;
	block.setPosition(coordinates);
}

Textures::ID Block::get_id() {
	return id;
}

void Block::set_id(Textures::ID id) {
	sf::Texture& texture = texture_holder.get(id);
	block.setTexture(texture);
	this->id = id;
}

sf::FloatRect Block::getGlobalBound()
{
	return block.getGlobalBounds();
}


World::World() 
{
	for (int i = 0; i < WORLD_HEIGHT; ++i)
		for (int j = 0; j < WORLD_WIDTH; ++j)
			tilemap[i][j] = nullptr;
}

std::string World::get_id_name(Textures::ID id)
{
	switch (id)
	{
	case Textures::NUL:
		return "NUL";
		break;

	case Textures::MENU:
		return "MENU";
		break;

	case Textures::VAMPIRE:
		return "VAMPIRE";
		break;

	case Textures::VAMPIREL:
		return "VAMPIREL";
		break;

	case Textures::ORANGE:
		return "ORANGE";
		break;

	case Textures::ITEMS:
		return "ITEMS";
		break;

	case Textures::GREY:
		return "GREY";
		break;

	case Textures::ROCK:
		return "ROCK";
		break;

	case Textures::DIRT:
		return "DIRT";
		break;

	case Textures::IRON:
		return "IRON";
		break;

	case Textures::WOOD:
		return "WOOD";
		break;

	case Textures::LEAVES:
		return "LEAVES";
		break;

	default:
		return "ERRORID";
		break;
	}
}

void World::spawn_tree(int x, int y) {
	if (y > 3 && y < 996) {
		Textures::ID leaves = Textures::ID::LEAVES;
		Textures::ID wood = Textures::ID::WOOD;
		for (int i = x; i > x - 4; i--) {
			set_block(i, y, wood);
		}
		for (int i = 0; i < 5; i++) {
			if (tilemap[x - 4][y - 2 + i] == nullptr)
					set_block(x - 4, y - 2 + i, leaves);
		}
		for (int i = 0; i < 5; i++) {
			if (tilemap[x - 3][y - 2 + i] == nullptr)
					set_block(x - 3, y - 2 + i, leaves);
		}
		for (int i = 0; i < 3; i++) {
			if (tilemap[x - 5][y - 1 + i] == nullptr)
					set_block(x - 5, y - 1 + i, leaves);
		}
		if (tilemap[x - 2][y - 1] == nullptr)
				set_block(x - 2, y - 1, leaves);
		if (tilemap[x - 2][y +1] == nullptr)
				set_block(x - 2, y + 1, leaves);
		if (tilemap[x - 6][y] == nullptr)
				set_block(x - 6, y, leaves);
	}
}

void World::create_surface() {
	random_number.initialize();
	for (int j = 0; j < WORLD_WIDTH; j++) {
		if (line_of_horizon>WORLD_HEIGHT-85)
			line_of_horizon += random_number.get_random(-3, 0);
		else if (line_of_horizon<30)
			line_of_horizon += random_number.get_random(0, 3);
		else
			line_of_horizon += random_number.get_random(-1, 1);

		//std::cout << line_of_horizon << std::endl;

		for (int i = line_of_horizon; i < WORLD_HEIGHT; i++) {
			//if (i < line_of_horizon)
			//	delete_block(i, j);
			if (i >= line_of_horizon && i < line_of_horizon + 3)
				set_block(i, j, Textures::ID::ORANGE);
			else
				set_block(i, j, Textures::ID::ROCK);
		}
		int tree_chance = random_number.get_random(0, 20);
		if (tree_chance == 20)
			spawn_tree(line_of_horizon, j);
	}
}

void World::create_cave_right(int x, int y) {
	int cave_height = random_number.get_random(100, 135);
	//std::cout << "CAVE HEIGHT" << cave_height << std::endl;
	for (int i = x; i < x + cave_height; i++) {
		int cave_width = random_number.get_random(6, 8);
		for (int j = y; j < y + cave_width; j++) {
			if (i>0 && i<145 && j>0 && j<1000)
				delete_block(i-1, j-1);
			if (i>0 && i<145 && j>0 && j < 1000)
				delete_block(i, j);
		}	

		y += random_number.get_random(-1,3);
		if (random_number.get_random(0, 5) == 5)
			i -= random_number.get_random(1, 2);
		if (random_number.get_random(0, 70) == 70)
			create_cave_left(i, y);
		if (i>55)
			if (random_number.get_random(0, 20) == 20)
				add_enemy(sf::Vector2f(y*32.f, i*32.f), Textures::ID::GREY);
	}
}

void World::create_cave_left(int x, int y) {
	int cave_height = random_number.get_random(100, 135);
	//std::cout << "CAVE HEIGHT" << cave_height << std::endl;
	for (int i = x; i < x + cave_height; i++) {
		int cave_width = random_number.get_random(6, 8);
		for (int j = y; j > y - cave_width; j--) {
			if (i > 0 && i < 145 && j>0 && j < 1000)
				delete_block(i - 1, j + 1);
			if (i > 0 && i < 145 && j>0 && j < 1000)
				delete_block(i, j);
		}

		y -= random_number.get_random(-1, 3);
		if (random_number.get_random(0, 5) == 5)
			i -= random_number.get_random(1, 2);

		if (i>55)
			if (random_number.get_random(0, 20) == 20)
				add_enemy(sf::Vector2f(y * 32.f, i * 32.f), Textures::ID::GREY);
	}
}

void World::create_mountain() {
	int mount_start_y = random_number.get_random(20, 950);
	int mount_start_x;
	int mount_width = random_number.get_random(10, 35);
	int mount_level = WORLD_HEIGHT - 82;
	for (int i = WORLD_HEIGHT - 82; ; i--) {
		if (tilemap[i][mount_start_y] != nullptr) {
			if (tilemap[i][mount_start_y]->get_id() == Textures::ID::ROCK)
				mount_start_x = i;
			else if (tilemap[i][mount_start_y]->get_id() == Textures::ID::DIRT)
				break;
		}
	}
	mount_level = mount_start_x;
	for (int j = mount_start_y; j < mount_start_y + mount_width; j++) {
		if ((j - mount_start_y) < (mount_width / 2) && mount_level > 14)
			mount_level -= random_number.get_random(2, 4);
		else 
			mount_level += random_number.get_random(2, 4);
		
		for (int i = mount_level-3; i < WORLD_HEIGHT-82; i++) {
			//if (i < line_of_horizon)
			//	delete_block(i, j);
			if (i >= mount_level-3 && i < mount_level)
				set_block(i, j, Textures::ID::ORANGE);
			else
				set_block(i, j, Textures::ID::ROCK);
		}
	}
}

void World::spawn_resources() {
	for (int i = 0; i < 10000; i++) {
		int randX = random_number.get_random(line_of_horizon+20, WORLD_HEIGHT - 1);
		int randY = random_number.get_random(0, WORLD_WIDTH - 1);
		if (tilemap[randX][randY]!=nullptr)
			if (tilemap[randX][randY]->get_id() == Textures::ID::ROCK)
				tilemap[randX][randY]->set_id(Textures::ID::IRON);
	}
}

void World::spawn_dungeon() {
	//stmnd dont know how dung should looks like, so waiting for info from Said.
}

void World::generate_world() {
	//for (int i = 29; i < WORLD_HEIGHT; i++) {
	//	for (int j = 0; j < WORLD_WIDTH; j++) {		//flat world generate commented
	//		World::set_block(i, j, Textures::ID::ORANGE);
	//	}
	//}

	create_surface();
	for (int i = 0; i < random_number.get_random(10, 20); i++) {
		create_mountain();
	}
	spawn_resources();
	create_cave_right(40, 1/*random_number.get_random(25, 60)*/);

}

void World::delete_block(int x, int y)
{
	delete tilemap[x][y];
	tilemap[x][y] = nullptr;
}

void World::destroy_block(sf::Vector2i m_pos, sf::Vector2f p_pos)
{
	if (m_pos.x < 0 || m_pos.y > WORLD_HEIGHT*32 - 1 || m_pos.y < 0 || m_pos.x > WORLD_WIDTH*32 - 1 || tilemap[m_pos.y / 32][m_pos.x / 32] == nullptr) return;
	// std::cout << std::endl << "distance " << sqrt(((p_pos.x - pos.x) * (p_pos.x - pos.x) + (p_pos.y - pos.y) * (p_pos.y - pos.y))) << std::endl;
	if (static_cast<int>(sqrt((((p_pos.x - m_pos.x) * (p_pos.x - m_pos.x) + (p_pos.y - m_pos.y) * (p_pos.y - m_pos.y))))) > (7 * 32)) return;
	// std::cout << std::endl << " destroying " << std::endl;

	// choosing one of 4 corners of player to pull checking vector
	// left-top, left-bot, right-top, right-bot
	sf::Vector2f start_pos;
	sf::Vector2f vector_napr = sf::Vector2f(abs(m_pos.x - p_pos.x), abs(m_pos.y - p_pos.y));         // vector of checking
	bool lt = false, lb = false, rt = false, rb = false;
	bool intersected = false;
	sf::Vector2f intersected_cor;
	
	if (m_pos.x <= p_pos.x + 16 && m_pos.y <= p_pos.y + 30)       // player is 32*60
	{
		start_pos = sf::Vector2f(p_pos.x + 2, p_pos.y + 2);       // p_pos is left top corner
		
		if (m_pos.x >= start_pos.x - 2 && m_pos.y <= start_pos.y - 2)
		{
			while (start_pos.y > m_pos.y + 38)
			{
				start_pos = sf::Vector2f(start_pos.x, start_pos.y - 10.f);
				sf::Sprite check;
				check.setPosition(start_pos);
				check.setTextureRect(sf::IntRect(0, 0, 1, 1));
				/*std::cout << std::endl << " player.pos " << p_pos.x << " " << p_pos.y << std::endl;
				std::cout << std::endl << " mouse.pos " << m_pos.x << " " << m_pos.y << std::endl;
				std::cout << std::endl << " check.pos " << check.getPosition().x << " " << check.getPosition().y << std::endl;
				std::cout << std::endl << " start.pos " << start_pos.x << " " << start_pos.y << std::endl;*/
				if (tilemap[start_pos.y / 32][start_pos.x / 32] != nullptr && 
					        check.getGlobalBounds().intersects(tilemap[start_pos.y / 32][start_pos.x / 32]->getGlobalBound()))
				{
					intersected = true;
					intersected_cor = start_pos;
					break;
				}
			}
		}
		else if (m_pos.x <= start_pos.x - 2 && m_pos.y >= start_pos.y - 2)
		{
			while (start_pos.x > m_pos.x + 38)
			{
				start_pos = sf::Vector2f(start_pos.x - 10.f, start_pos.y);
				sf::Sprite check;
				check.setPosition(start_pos);
				check.setTextureRect(sf::IntRect(0, 0, 1, 1));

				if (tilemap[start_pos.y / 32][start_pos.x / 32] != nullptr &&
					check.getGlobalBounds().intersects(tilemap[start_pos.y / 32][start_pos.x / 32]->getGlobalBound()))
				{
					intersected = true;
					intersected_cor = start_pos;
					break;
				}
			}
		}
		else
		{
			while (start_pos.x > m_pos.x + 38 && start_pos.y > m_pos.y + 48)
			{
				start_pos = sf::Vector2f(start_pos.x - 10.f, start_pos.y - 10.f);
				sf::Sprite check;
				check.setPosition(start_pos);
				check.setTextureRect(sf::IntRect(0, 0, 1, 1));

				if (tilemap[start_pos.y / 32][start_pos.x / 32] != nullptr &&
					check.getGlobalBounds().intersects(tilemap[start_pos.y / 32][start_pos.x / 32]->getGlobalBound()))
				{
					intersected = true;
					intersected_cor = start_pos;
					break;
				}
			}
		}
		lt = true;
	}   



	else if (m_pos.x <= p_pos.x + 16 && m_pos.y >= p_pos.y + 30)
	{
		start_pos = sf::Vector2f(p_pos.x + 2, p_pos.y + 58);

		if (m_pos.x >= start_pos.x - 2 && m_pos.y >= start_pos.y + 2)
		{
			while (start_pos.y < m_pos.y - 38)
			{
				start_pos = sf::Vector2f(start_pos.x, start_pos.y + 10.f);
				sf::Sprite check;
				check.setPosition(start_pos);
				check.setTextureRect(sf::IntRect(0, 0, 1, 1));
				if (tilemap[start_pos.y / 32][start_pos.x / 32] != nullptr &&
					check.getGlobalBounds().intersects(tilemap[start_pos.y / 32][start_pos.x / 32]->getGlobalBound()))
				{
					intersected = true;
					intersected_cor = start_pos;
					break;
				}
			}
		}
		else if (m_pos.x <= start_pos.x - 2 && m_pos.y <= start_pos.y - 2)
		{
			while (start_pos.x > m_pos.x + 38)
			{
				start_pos = sf::Vector2f(start_pos.x - 10.f, start_pos.y);
				sf::Sprite check;
				check.setPosition(start_pos);
				check.setTextureRect(sf::IntRect(0, 0, 1, 1));

				if (tilemap[start_pos.y / 32][start_pos.x / 32] != nullptr &&
					check.getGlobalBounds().intersects(tilemap[start_pos.y / 32][start_pos.x / 32]->getGlobalBound()))
				{
					intersected = true;
					intersected_cor = start_pos;
					break;
				}
			}
		}
		else
		{
			while (start_pos.x > m_pos.x + 38 && start_pos.y < m_pos.y + 48)
			{
				start_pos = sf::Vector2f(start_pos.x - 10.f, start_pos.y + 10.f);
				sf::Sprite check;
				check.setPosition(start_pos);
				check.setTextureRect(sf::IntRect(0, 0, 1, 1));

				if (tilemap[start_pos.y / 32][start_pos.x / 32] != nullptr &&
					check.getGlobalBounds().intersects(tilemap[start_pos.y / 32][start_pos.x / 32]->getGlobalBound()))
				{
					intersected = true;
					intersected_cor = start_pos;
					break;
				}
			}
		}

		lb = true;
	}


	// rt
	else if (m_pos.x >= p_pos.x + 16 && m_pos.y <= p_pos.y + 30)
	{
		start_pos = sf::Vector2f(p_pos.x + 30, p_pos.y + 2);   

		if (m_pos.x <= start_pos.x + 2 && m_pos.y <= start_pos.y - 2)
		{
			while (start_pos.y > m_pos.y + 38)
			{
				start_pos = sf::Vector2f(start_pos.x, start_pos.y - 10.f);
				sf::Sprite check;
				check.setPosition(start_pos);
				check.setTextureRect(sf::IntRect(0, 0, 1, 1));
				/*std::cout << std::endl << " player.pos " << p_pos.x << " " << p_pos.y << std::endl;
				std::cout << std::endl << " mouse.pos " << m_pos.x << " " << m_pos.y << std::endl;
				std::cout << std::endl << " check.pos " << check.getPosition().x << " " << check.getPosition().y << std::endl;
				std::cout << std::endl << " start.pos " << start_pos.x << " " << start_pos.y << std::endl;*/
				if (tilemap[start_pos.y / 32][start_pos.x / 32] != nullptr &&
					check.getGlobalBounds().intersects(tilemap[start_pos.y / 32][start_pos.x / 32]->getGlobalBound()))
				{
					intersected = true;
					intersected_cor = start_pos;
					break;
				}
			}
		}
		else if (m_pos.x >= start_pos.x + 2 && m_pos.y >= start_pos.y - 2)
		{
			while (start_pos.x < m_pos.x + 38)
			{
				start_pos = sf::Vector2f(start_pos.x + 10.f, start_pos.y);
				sf::Sprite check;
				check.setPosition(start_pos);
				check.setTextureRect(sf::IntRect(0, 0, 1, 1));

				if (tilemap[start_pos.y / 32][start_pos.x / 32] != nullptr &&
					check.getGlobalBounds().intersects(tilemap[start_pos.y / 32][start_pos.x / 32]->getGlobalBound()))
				{
					intersected = true;
					intersected_cor = start_pos;
					break;
				}
			}
		}
		else
		{
			while (start_pos.x < m_pos.x + 38 && start_pos.y > m_pos.y + 48)
			{
				start_pos = sf::Vector2f(start_pos.x + 10.f, start_pos.y - 10.f);
				sf::Sprite check;
				check.setPosition(start_pos);
				check.setTextureRect(sf::IntRect(0, 0, 1, 1));

				if (tilemap[start_pos.y / 32][start_pos.x / 32] != nullptr &&
					check.getGlobalBounds().intersects(tilemap[start_pos.y / 32][start_pos.x / 32]->getGlobalBound()))
				{
					intersected = true;
					intersected_cor = start_pos;
					break;
				}
			}
		}
		
		rt = true;
	}



	else
	{
		start_pos = sf::Vector2f(p_pos.x + 30, p_pos.y + 58);

		if (m_pos.x <= start_pos.x + 2 && m_pos.y >= start_pos.y + 2)
		{
			while (start_pos.y < m_pos.y - 38)
			{
				start_pos = sf::Vector2f(start_pos.x, start_pos.y + 10.f);
				sf::Sprite check;
				check.setPosition(start_pos);
				check.setTextureRect(sf::IntRect(0, 0, 1, 1));
				if (tilemap[start_pos.y / 32][start_pos.x / 32] != nullptr &&
					check.getGlobalBounds().intersects(tilemap[start_pos.y / 32][start_pos.x / 32]->getGlobalBound()))
				{
					intersected = true;
					intersected_cor = start_pos;
					break;
				}
			}
		}
		else if (m_pos.x >= start_pos.x + 2 && m_pos.y <= start_pos.y - 2)
		{
			while (start_pos.x < m_pos.x + 38)
			{
				start_pos = sf::Vector2f(start_pos.x + 10.f, start_pos.y);
				sf::Sprite check;
				check.setPosition(start_pos);
				check.setTextureRect(sf::IntRect(0, 0, 1, 1));

				if (tilemap[start_pos.y / 32][start_pos.x / 32] != nullptr &&
					check.getGlobalBounds().intersects(tilemap[start_pos.y / 32][start_pos.x / 32]->getGlobalBound()))
				{
					intersected = true;
					intersected_cor = start_pos;
					break;
				}
			}
		}
		else
		{
			while (start_pos.x < m_pos.x + 38 && start_pos.y < m_pos.y + 48)
			{
				start_pos = sf::Vector2f(start_pos.x + 10.f, start_pos.y + 10.f);
				sf::Sprite check;
				check.setPosition(start_pos);
				check.setTextureRect(sf::IntRect(0, 0, 1, 1));

				if (tilemap[start_pos.y / 32][start_pos.x / 32] != nullptr &&
					check.getGlobalBounds().intersects(tilemap[start_pos.y / 32][start_pos.x / 32]->getGlobalBound()))
				{
					intersected = true;
					intersected_cor = start_pos;
					std::cout << std::endl << " intersected " << std::endl;
					std::cout << std::endl << "_________________________" << std::endl;
					break;
				}
			}
		}
		rb = true;
	}







	Textures::ID id = tilemap[m_pos.y / 32][m_pos.x / 32]->get_id();
	delete tilemap[m_pos.y / 32][m_pos.x / 32];
	tilemap[m_pos.y / 32][m_pos.x / 32] = nullptr;
	add_ground_item(id, sf::Vector2f(m_pos.x, m_pos.y));
}

bool World::place_block(sf::Vector2i m_pos, Textures::ID id, sf::Vector2f p_pos)
{
	if (id == Textures::ID::NUL || m_pos.x < 0 || m_pos.y > WORLD_HEIGHT * 32 || m_pos.y < 0 || m_pos.x > WORLD_WIDTH * 32 || tilemap[m_pos.y / 32][m_pos.x / 32] != nullptr)
		return false;
	if (static_cast<int>(sqrt((((p_pos.x - m_pos.x) * (p_pos.x - m_pos.x) + (p_pos.y - m_pos.y) * (p_pos.y - m_pos.y))))) > (7 * 32)) return false;

	//std::cout << std::endl << "distance " << ((p_pos.x - pos.x) * (p_pos.x - pos.x) + (p_pos.y - pos.y) * (p_pos.y - pos.y)) << std::endl;
	tilemap[m_pos.y / 32][m_pos.x / 32] = new Block(id);
	tilemap[m_pos.y / 32][m_pos.x / 32]->set_coordinates(sf::Vector2f((m_pos.x/32) * 32.f, (m_pos.y/32)* 32.f));
	return true;

}

void World::add_ground_item(Textures::ID id, sf::Vector2f coord, int kolvo)
{
	gitems.push_back(GroundItem(id, coord, kolvo));
}

void World::add_ground_item(Textures::ID id, sf::Vector2f coord)
{
	gitems.push_back(GroundItem(id, coord));
}

void World::set_block (int x, int y, Textures::ID id) {
	if (id == Textures::ID::ORANGE) {
		if (tilemap[x - 1][y] != nullptr)
			if ((tilemap[x - 1][y]->get_id() == Textures::ID::ORANGE || tilemap[x - 1][y]->get_id() == Textures::ID::DIRT))
				id = Textures::ID::DIRT;
		else if (tilemap[x + 1][y] != nullptr)
				if (tilemap[x + 1][y]->get_id() == Textures::ID::ORANGE)
					tilemap[x + 1][y]->set_id(Textures::ID::DIRT);
	}
	tilemap[x][y] = new Block(id);
	tilemap[x][y]->set_coordinates(sf::Vector2f(y * 32.f, x * 32.f));
}

void World::drawU(sf::RenderWindow& window, sf::Vector2f p_coordinates)
{
	int i1 = p_coordinates.x;
	i1 /= 32;
	int j1 = p_coordinates.y;
	j1 /= 32;

	int temp = i1;
	i1 = j1;
	j1 = temp;

	i1 -= 20;
	if (i1 < 0) i1 = 0;
	j1 -= 20;
	if (j1 < 0) j1 = 0;

	// std::cout << i1 << "i1 " << j1 << "j1" << std::endl;

	int LESS_HEIGHT = i1 + 40;
	int LESS_WIDTH = j1 + 40;
	if (LESS_HEIGHT >= WORLD_HEIGHT) LESS_HEIGHT = WORLD_HEIGHT - 1;
	if (LESS_WIDTH >= WORLD_WIDTH)   LESS_WIDTH  =  WORLD_WIDTH - 1;

	
	if (WORLD_HEIGHT - i1 > 40) LESS_HEIGHT = i1 + 40;
	if (WORLD_WIDTH - j1 > 45) LESS_WIDTH = j1 + 45;

	for (int i = i1; i < LESS_HEIGHT; ++i)
		for (int j = j1; j < LESS_WIDTH; ++j)
			if (tilemap[i][j] != nullptr)
				tilemap[i][j]->drawU(window);
}

void World::test_world()
{
	// dont want to write it everytime even using copy-paste
	Textures::ID g = Textures::ID::ORANGE;
	for (int i = 14; i < WORLD_HEIGHT; ++i)
		for (int j = 0; j < WORLD_WIDTH; ++j)
		{
			set_block(i, j, g);
		}

	set_block(13, 15, g);

	set_block(13, 16, g);

	set_block(12, 15, g);

	set_block(11, 17, g);

	set_block(9, 14, g);

	set_block(13, 19, g);

	set_block(13, 14, g);

	set_block(9, 12, g);

	set_block(9, 10, g);

	set_block(10, 19, g);

	set_block(10, 9, g);

	set_block(11, 7, g);

	set_block(12, 6, g);

	set_block(13, 5, g);

	set_block(13, 0, g);
}

void World::add_enemy(sf::Vector2f position, Textures::ID id)
{

	enemies.push_back(Enemy(position, id));
}


//____________________________________________________________________

Enemy::Enemy(sf::Vector2f position, Textures::ID id)
{
	sf::Texture& texture = texture_holder.get(id);
	type = id;
	character.setTexture(texture);
	enemy_position = position;
	character.setPosition(enemy_position);
	if (type == Textures::ID::GREY)
	{
		character.setTextureRect(sf::IntRect(0, 0, 32, 29));
		enemy_speed /= 2;
	}
}

void Enemy::drawU(sf::RenderWindow& window)
{
	window.draw(character);
}

float Enemy::getenemycoordinateX()
{
	return enemy_position.x;
}

float Enemy::getenemycoordinateY()
{
	return enemy_position.y;
}

bool Enemy::may_jump_left(const World& chunk, sf::Vector2f p_coor)
{
	int x = p_coor.x;
	x /= 32;
	int y = p_coor.y;
	y /= 32;

	int temp = x;
	x = y;
	y = temp;

	if (x > 0 && y >0 )
	{
		if ((chunk.tilemap[x][y-1] != nullptr) && (chunk.tilemap[x - 1][y - 1] != nullptr) &&
			(!chunk.tilemap[x][y-1]->passable()) && (!chunk.tilemap[x - 1][y - 1]->passable()))
			return true;
	}
	
	return false;
}

bool Enemy::may_jump_right(const World& chunk, sf::Vector2f p_coor)
{
	int x = p_coor.x;
	x /= 32;
	int y = p_coor.y;
	y /= 32;

	int temp = x;
	x = y;
	y = temp;

	if (x < (WORLD_HEIGHT-1)*32 && y < (WORLD_WIDTH-1)*32)
	{
		if ((chunk.tilemap[x][y + 1] != nullptr) && (chunk.tilemap[x - 1][y + 1] != nullptr) &&
			(!chunk.tilemap[x][y + 1]->passable()) && (!chunk.tilemap[x - 1][y + 1]->passable()))
			return true;
	}

	return false;
}

void Enemy::update_statement(const sf::Time delta_time, const World& chunk, sf::Vector2f p_coor)
{
	const int AIarea = 4 * 32; // area of mobs working. After set a higher value.
	const int AIstop = 18;
	sf::Vector2f movement(0.f, 0.f);
	sf::FloatRect nextPos;
	bool jump = false;
	const float x_crop = 10.f;

	float e_movement_x =  p_coor.x - enemy_position.x;

	if ( ((e_movement_x <= AIarea && e_movement_x > 0) || (e_movement_x * -1 <= AIarea && e_movement_x < 0)) && (
		  (e_movement_x >= AIstop && e_movement_x > 0) || (e_movement_x * -1 >= AIstop && e_movement_x < 0))    )
	{
		if (e_movement_x > 0)
		{
			isMovingRigth = true;
			isMovingLeft = false;
		}
		else if (e_movement_x < 0)
		{
			isMovingLeft = true;
			isMovingRigth = false;
		}
	}

	else
	{
		isMovingRigth = false;
		isMovingLeft = false;
	}

	bool smth_is_under = false;

	// getting left top coordinate of little chunk to check block collisions only there
	// lots of validation
	// a kind of optimisation

	int i1 = enemy_position.x;
	i1 /= 32;
	int j1 = enemy_position.y;
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
	if (WORLD_HEIGHT - i1 > 6) LESS_HEIGHT = i1 + 6;
	if (WORLD_WIDTH - j1 > 6) LESS_WIDTH = j1 + 6;

	for (int i = i1; i < LESS_HEIGHT; ++i)
		for (int j = j1; j < LESS_WIDTH; ++j)
		{
			if (chunk.tilemap[i][j] != nullptr && !(chunk.tilemap[i][j]->passable())) {
				sf::FloatRect characterBounds1 = character.getGlobalBounds();
				sf::FloatRect characterBounds(characterBounds1.left + x_crop / 2, characterBounds1.top, characterBounds1.width - x_crop, characterBounds1.height);
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

	// if (isMovingUp)    movement.y -= player_speed;   // isn't needed untill we have vertical stairs, jumping by negative gravity
	// if (isMovingDown)  movement.y += player_speed;   // going down by pressing keys, when we have gravity? lol

	if (isMovingLeft)  movement.x -= enemy_speed;
	if (isMovingRigth) movement.x += enemy_speed;

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

	i1 = enemy_position.x;
	i1 /= 32;
	j1 = enemy_position.y;
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
						if (!Enemy::may_jump_right(chunk, p_coor)) isMovingUp = true;
						// std::cout << "rh";
						character.setPosition(blockBounds.left - characterBounds.width - x_crop / 2, characterBounds.top);
					}

					// left collision
					else if (characterBounds.left > blockBounds.left
						&& characterBounds.left + characterBounds.width > blockBounds.left + blockBounds.width
						&& characterBounds.top < blockBounds.top + blockBounds.height
						&& characterBounds.top + characterBounds.height > blockBounds.top
						)
					{
						movement.x = 0.f;
						if (!Enemy::may_jump_left(chunk, p_coor)) isMovingUp = true;
						// std::cout << "lf";
						character.setPosition(blockBounds.left + blockBounds.width - x_crop / 2, characterBounds.top);
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
	enemy_position.x = character.getGlobalBounds().left;
	enemy_position.y = character.getGlobalBounds().top;
}

//____________________________________________________________________

Textures::ID GroundItem::get_id()
{
	return id;
}

GroundItem::GroundItem(Textures::ID id, sf::Vector2f coord) : id(id)
{
	amount = 1;

	sf::Texture& texture = texture_holder.get(id);
	sprite.setTexture(texture);
	sprite.setScale(0.2, 0.2);
	sprite.setPosition(coord);
}

GroundItem::GroundItem(Textures::ID id, sf::Vector2f coord, int kolvo)
{
	amount = kolvo;

	sf::Texture& texture = texture_holder.get(id);
	sprite.setTexture(texture);
	sprite.setScale(0.2, 0.2);
	sprite.setPosition(coord);
}

int GroundItem::get_amount()
{
	return amount;
}

void GroundItem::set_position(sf::Vector2f pos)
{
	sprite.setPosition(pos);
}

void GroundItem::drawU(sf::RenderWindow& window)
{
	window.draw(sprite);
}


void GroundItem::add_plenty(int num)
{
	amount += num;
}

sf::FloatRect GroundItem::getGlobalBounds()
{
	return sf::FloatRect(sprite.getGlobalBounds());
}

void GroundItem::update_statement(const sf::Time delta_time, const World& chunk)
{
	sf::Vector2f movement(0.f, 0.f);
	sf::FloatRect nextPos;

	bool smth_is_under = false;

	// getting left top coordinate of little chunk to check block collisions only there
	// lots of validation
	// a kind of optimisation

	int i1 = sprite.getPosition().x;
	i1 /= 32;
	int j1 = sprite.getPosition().y;
	j1 /= 32;

	// swapping, because we need:)
	int temp = i1;
	i1 = j1;
	j1 = temp;

	i1 -= 1;
	if (i1 < 0) i1 = 0;
	j1 -= 3;
	if (j1 < 0) j1 = 0;

	int LESS_HEIGHT = WORLD_HEIGHT;
	int LESS_WIDTH = WORLD_WIDTH;
	if (WORLD_HEIGHT - i1 > 6) LESS_HEIGHT = i1 + 6;
	if (WORLD_WIDTH - j1 > 6) LESS_WIDTH = j1 + 6;

	for (int i = i1; i < LESS_HEIGHT; ++i)
		for (int j = j1; j < LESS_WIDTH; ++j)
		{
			if (chunk.tilemap[i][j] != nullptr && !(chunk.tilemap[i][j]->passable())) {
				sf::FloatRect characterBounds1 = sprite.getGlobalBounds();
				sf::FloatRect characterBounds(characterBounds1.left, characterBounds1.top, characterBounds1.width, characterBounds1.height);
				sf::FloatRect blockBounds = chunk.tilemap[i][j]->getGlobalBound();

				//std::cout << characterBounds.width << " " << characterBounds.height;

				nextPos = characterBounds;
				nextPos.top += gravity = 9;


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



	if (!onGround)
	{
		movement.y += gravityAccum; 
		gravityAccum += gravity;
	}

	if (onGround) gravityAccum = 0;

	// there is no gravitation on the floor (it can be, doesn't matter, but...)

	i1 = sprite.getPosition().x;
	i1 /= 32;
	j1 = sprite.getPosition().y;
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
				sf::FloatRect characterBounds1 = sprite.getGlobalBounds();
				sf::FloatRect characterBounds(characterBounds1.left, characterBounds1.top, characterBounds1.width, characterBounds1.height);
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
						movement.y = 0.f;

						sprite.setPosition(characterBounds.left, blockBounds.top - characterBounds.height);
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
						sprite.setPosition(characterBounds.left, blockBounds.top + blockBounds.height);
					}

					/*// right collision
					else if (characterBounds.left < blockBounds.left
						&& characterBounds.left + characterBounds.width < blockBounds.left + blockBounds.width
						&& characterBounds.top < blockBounds.top + blockBounds.height
						&& characterBounds.top + characterBounds.height > blockBounds.top
						)
					{
						movement.x = 0.f;
						sprite.setPosition(blockBounds.left - characterBounds.width, characterBounds.top);
					}

					// left collision
					else if (characterBounds.left > blockBounds.left
						&& characterBounds.left + characterBounds.width > blockBounds.left + blockBounds.width
						&& characterBounds.top < blockBounds.top + blockBounds.height
						&& characterBounds.top + characterBounds.height > blockBounds.top
						)
					{
						movement.x = 0.f;
						sprite.setPosition(blockBounds.left + blockBounds.width, characterBounds.top);
					}*/
				}
			}
		}
	sprite.move(movement* delta_time.asSeconds());
}
