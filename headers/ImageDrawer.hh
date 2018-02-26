#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Block.hh"
#include "Tools.hh"

/*
 * ImageDrawer Block
 *   Displays a texture in a window
 *   The texture is scaled if it's too large
 *   The window has to be closed manually to continue execution
 */

class ImageDrawer: public Block
{
public:
	Input<sf::Texture>* in_texture;
protected:
	virtual void Compute() override;
public:
	ImageDrawer(std::string _name);
};
