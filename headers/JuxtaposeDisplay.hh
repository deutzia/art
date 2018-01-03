#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Block.hh"

/*
 * JuxtaposeDisplay Block
 *   Displays two textures in a window, with a slider to compare them
 *   To move the slider click anywhere on the window
 *   The window is scaled if it's too large (the textures are not modified)
 *   The window has to be manually closed to continue execution
 */

class JuxtaposeDisplay: public Block
{
	// Limits the size of the window
	const float MAX_SCREEN_COVERAGE = .90;
public:
	Input<sf::Texture>* in_texture_left;
	Input<sf::Texture>* in_texture_right;
protected:
	virtual void Compute() override;
public:
	JuxtaposeDisplay(std::string _name, Logger* _logger);
};
