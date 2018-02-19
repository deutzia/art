#pragma once
#include <vector>
#include "Block.hh"
#include "Tools.hh"

class PrefixSumsOnImage: public Block
{
public:
	Input<sf::Texture>* in_texture;
	Output<std::vector<std::vector<ColorPrefix>>>* out_array;
protected:
	virtual void Compute() override;
public:
	PrefixSumsOnImage(std::string _name);
};
