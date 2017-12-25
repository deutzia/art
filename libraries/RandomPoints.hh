#include <SFML/Graphics.hpp>
#include <vector>
#include "Block.hh"

class RandomPoints: public Block
{
public:
	Input<sf::Vector2u>* size;
	Output<std::vector<sf::Vector2f> >* points; 
protected:
	virtual void Compute() override;
public:
	RandomPoints(std::string _name, Logger* _logger);
};
