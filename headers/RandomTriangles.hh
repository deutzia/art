#include <list>
#include <SFML/Graphics.hpp>
#include "Block.hh"
#include "Tools.hh"

class RandomTriangles: public Block
{
public:
	/* A picture for size reference */
	Input<sf::Texture>* input_image;

	/* List of triangles */
	Output<std::vector<Triangle>>* triangles;

	/* Picture with triangles overlain */
	Output<sf::Texture>* output_image;
protected:
	virtual void Compute() override;
public:
	RandomTriangles(std::string _name, Logger*  _logger);
};