#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Block.hh"

/*
 * ImageDrawer Block
 *   Displays a texture in a window
 *   The window is scaled if it's too large (the texture itself is unmodified)
 *   The window has to be closed manually to continue execution
 */

class ImageDrawer: public Block
{
	// Limits the size of the window
	const float MAX_SCREEN_COVERAGE = .90;
public:
	Input<sf::Texture>* in_texture;
protected:
	virtual void Compute() override;
public:
	ImageDrawer(std::string _name, Logger* _logger);
};
