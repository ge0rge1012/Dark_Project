#include "WorldClass.h"

TextureHolder::TextureHolder() { }

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

// let standart texture be just dirt with grass
Block::Block()
{
	sf::Texture& texture = texture_holder.get(Textures::GRASS);
	block.setTexture(texture);
}

Block::Block(Textures::ID id)
{
	sf::Texture& texture = texture_holder.get(id);
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

World::World() 
{
	for (int i = 0; i < WORLD_HEIGHT; ++i)
		for (int j = 0; j < WORLD_WIDTH; ++j)
			tilemap[i][j] = nullptr;
}

void World::create_surface() {

}

void World::create_cave(int x, int y) {

}

void World::create_mountain() {

}

void World::spawn_dungeon() {
	//stmnd dont know how dung should looks like, so waiting for info from Said.
}

void World::generate_world() {
	for (int i = 29; i < WORLD_HEIGHT; i++) {
		for (int j = 0; j < WORLD_WIDTH; j++) {
			World::set_block(i, j, Textures::ID::GRASS);
		}
	}
	World::create_surface();
	World::create_cave(20, 0);
}

void World::delete_block(int x, int y)
{
	delete tilemap[x][y];
	tilemap[x][y] = nullptr;
}

void World::set_block (int x, int y, Textures::ID id) {
	tilemap[x][y] = new Block(id);
	tilemap[x][y]->set_coordinates(sf::Vector2f(y * 32.f, x * 32.f));
}

void World::test_world()
{
	// dont want to write it everytime even using copy-paste
	Textures::ID g = Textures::ID::GRASS;
	for (int i = 14; i < 18; ++i)
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
		std::cout << "!";
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

void Enemy::update_statement(const sf::Time delta_time, const World& chunk)
{
	sf::Vector2f movement(0.f, 0.f);
	sf::FloatRect nextPos;
	bool jump = false;
	const float x_crop = 8.f;


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

	i1 -= 5;
	if (i1 < 0) i1 = 0;
	j1 -= 5;
	if (j1 < 0) j1 = 0;

	int LESS_HEIGHT = WORLD_HEIGHT;
	int LESS_WIDTH = WORLD_WIDTH;
	if (WORLD_HEIGHT - i1 > 15) LESS_HEIGHT = i1 + 15;
	if (WORLD_WIDTH - j1 > 15) LESS_WIDTH = j1 + 15;

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
					std::cout << "i" << i << " j" << j << std::endl;

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
	/*if (isMovingLeft)  movement.x -= enemy_speed;
	if (isMovingRigth) movement.x += enemy_speed;*/
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

	i1 -= 5;
	if (i1 < 0) i1 = 0;
	j1 -= 5;
	if (j1 < 0) j1 = 0;

	LESS_HEIGHT = WORLD_HEIGHT;
	LESS_WIDTH = WORLD_WIDTH;
	if (WORLD_HEIGHT - i1 > 15) LESS_HEIGHT = i1 + 15;
	if (WORLD_WIDTH - j1 > 15) LESS_WIDTH = j1 + 15;

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