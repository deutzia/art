#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Block.hh"

/*
 * SideBySideCompare<x_num, y_num> Block
 *   Combines a x_num by y_num array of textures into one texture, for 
 *    easy comparison.
 *   The textures are scaled before combining, so that the resulting texture
 *    fits on screen and in the memory.
 */

template<uint32_t x_num, uint32_t y_num>
class SideBySideCompare: public Block
{
	const float MAX_SCREEN_COVERAGE = 1.00; // One full screen
public:
	Input<sf::Vector2u>* in_size;
	std::array<std::array<Input<sf::Texture>*, y_num>, x_num> in_textures;

	Output<sf::Texture>* out_texture;
protected:
	virtual void Compute();
public:
	SideBySideCompare(std::string _name);

};

template<uint32_t x_num, uint32_t y_num>
void SideBySideCompare<x_num, y_num>::Compute()
{
	sf::Vector2u size = in_size->GetData();

	sf::Vector2u screen_size = {
		sf::VideoMode::getDesktopMode().width, 
		sf::VideoMode::getDesktopMode().height };
	sf::Vector2u max_size = {
		uint32_t(MAX_SCREEN_COVERAGE * screen_size.x / x_num),
		uint32_t(MAX_SCREEN_COVERAGE * screen_size.y / y_num) };

	// The 2D array is stored in a 1D array instead
	std::array<sf::Sprite, x_num * y_num> sprites;
	// The textures must exist as long as the sprites use them
	std::array<sf::Texture, x_num * y_num> textures; 

	logger.Log(Logger::LogLevel::Debug) << "Preparing textures...";
	auto timer = logger.InitializeProgress( 
		Logger::LogLevel::Debug, 0, x_num * y_num );
	for (uint32_t y = 0; y < y_num; ++y)
	{
		for (uint32_t x = 0; x < x_num; ++x)
		{
			uint32_t i = x + y * x_num;
			textures[i] = in_textures[x][y]->GetData();
			if (textures[i].getSize() != size)
			{
				std::ostringstream errmsg;
				errmsg << "in_textures[" << x << "][" << y << "] has wrong size";
				logger.Log(Logger::LogLevel::Error) << errmsg.str();
				throw errmsg.str();
			}
			textures[i] = LimitTextureSize(textures[i], max_size);
			sf::Vector2u texture_size = textures[i].getSize();

			sprites[i].setTexture(textures[i]);
			sprites[i].setPosition(x * texture_size.x, y * texture_size.y);
			timer.Count();
		}
	}
	logger.Log(Logger::LogLevel::Debug) << "Done!";

	sf::Vector2u texture_size = textures[0].getSize();
	sf::Vector2u picture_size = {
		texture_size.x * x_num, 
		texture_size.y * y_num };
	sf::RenderTexture render_texture;
	render_texture.create(picture_size.x, picture_size.y);
	render_texture.clear();
    render_texture.display(); // This fixes upside down textures
	for (sf::Sprite &sprite: sprites)
	 	render_texture.draw(sprite);    
	SetData(out_texture, render_texture.getTexture());
}

template<uint32_t x_num, uint32_t y_num>
SideBySideCompare<x_num, y_num>::SideBySideCompare(std::string _name)
: Block(_name)
{
	in_size = new Input<sf::Vector2u>(this);

	for (auto &row: in_textures)
		for (auto &texture: row)
			texture = new Input<sf::Texture>(this);

	out_texture = new Output<sf::Texture>(this);
}
