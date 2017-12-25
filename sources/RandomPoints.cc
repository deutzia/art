#include <random>
#include "RandomPoints.hh"

void RandomPoints::Compute()
{
	logger->Enter("Computing Block RandomPoints");
	std::vector<sf::Vector2f> result;
	sf::Vector2u bounds = size->GetData();
	result.emplace_back(0.0, 0.0);
	result.emplace_back(0.0, bounds.y);
	result.emplace_back(bounds.x, 0.0);
	result.emplace_back(bounds.x, bounds.y);
    std::random_device rd;
    std::mt19937 gen(rd());
	std::uniform_real_distribution<> random_generator_x(0.0, bounds.x);
	std::uniform_real_distribution<> random_generator_y(0.0, bounds.y);
	for (unsigned i = 0; i < bounds.x * bounds.y / 300; ++i)
		result.emplace_back(random_generator_x(gen), random_generator_y(gen));
	SetData(points, std::move(result));
	logger->Exit();
}

RandomPoints::RandomPoints(std::string _name, Logger* _logger)
: Block(_name, _logger)
{
	size = new Input<sf::Vector2u>(this);
	points = new Output<std::vector<sf::Vector2f> >(this);
}
