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

