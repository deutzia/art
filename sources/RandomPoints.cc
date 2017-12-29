#include "RandomPoints.hh"

void RandomPoints::Compute()
{
	logger->Enter("Computing Block RandomPoints");
	std::vector<sf::Vector2f> result;
	sf::Vector2u bounds = in_size->GetData();
	result.emplace_back(0, 0);
	result.emplace_back(0, bounds.y);
	result.emplace_back(bounds.x, 0);
	result.emplace_back(bounds.x, bounds.y);
	for (unsigned i = 0; i < bounds.x * bounds.y / 100; ++i)
		result.emplace_back(random_float(0, bounds.x), random_float(0, bounds.y));
	SetData(out_points, std::move(result));
	logger->Exit();
}

RandomPoints::RandomPoints(std::string _name, Logger* _logger)
: Block(_name, _logger)
{
	in_size = new Input<sf::Vector2u>(this);
	out_points = new Output<std::vector<sf::Vector2f>>(this);
}
