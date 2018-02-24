#pragma once
#include <SFML/Graphics.hpp>
#include "Block.hh"
#include "Tools.hh"

class ShaderTest: public Block
{
public:
	Input<sf::Texture>* in_texture;
	Input<int>* in_kernel_size;
	Output<sf::Texture>* out_texture;

protected:
	virtual void Compute() override;

public:
	ShaderTest(std::string _name);
};
