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

void TextureHolder::load(Textures::ID id, const std::string& filename)
{
	std::unique_ptr<sf::Texture> texture(new sf::Texture());
	texture->loadFromFile(filename);

	gTextureMap.insert(std::make_pair(id, std::move(texture)));
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
		if (line_of_horizon>60)
			line_of_horizon += random_number.get_random(-3, 0);
		else if (line_of_horizon<10)
			line_of_horizon += random_number.get_random(0, 3);
		else
			line_of_horizon += random_number.get_random(-1, 1);

		std::cout << line_of_horizon << std::endl;

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

void World::create_cave(int x, int y) {
	int cave_height = random_number.get_random(50, 100);
	std::cout << "CAVE HEIGHT" << cave_height << std::endl;
	for (int i = x; i < x + cave_height; i++) {
		int cave_width = random_number.get_random(6, 8);
		for (int j = y; j < y + cave_width; j++) {
			delete_block(i, j);
		}
		y+=cave_width%3;
	}

}

void World::create_mountain() {

}

void World::spawn_resources() {
	for (int i = 0; i < 10000; i++) {
		int randX = random_number.get_random(60, WORLD_HEIGHT - 1);
		int randY = random_number.get_random(0, WORLD_WIDTH - 1);
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

	World::create_surface();
	World::spawn_resources();
	World::create_cave(0, random_number.get_random(25, 60));

}

void World::delete_block(int x, int y)
{
	delete tilemap[x][y];
	tilemap[x][y] = nullptr;
}

void World::destroy_block(sf::Vector2i pos)
{
	if (pos.x < 0 || pos.x > WORLD_HEIGHT*32 - 1 || pos.y < 0 || pos.y > WORLD_WIDTH*32 - 1) return;
	delete tilemap[pos.y / 32][pos.x / 32];
	tilemap[pos.y / 32][pos.x / 32] = nullptr;
}

void World::place_block(sf::Vector2i pos, Textures::ID id)
{
	if (id == Textures::ID::NUL || pos.x < 0 || pos.x > WORLD_HEIGHT*32 - 1 || pos.y < 0 || pos.y > WORLD_WIDTH*32 - 1 || tilemap[pos.y / 32][pos.x / 32] != nullptr) return;
	tilemap[pos.y / 32][pos.x / 32] = new Block(id);
	tilemap[pos.y / 32][pos.x / 32]->set_coordinates(sf::Vector2f((pos.x/32) * 32.f, (pos.y/32)* 32.f));

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

//.....................................................................

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