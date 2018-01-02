#include "RandomPoints.hh"

class cmp
{
public:
	bool operator()(sf::Vector2f a, sf::Vector2f b)
	{
		if (a.x == b.x)
			return a.y < b.y;
		return a.x < b.x;
	}
};

void RandomPoints::Compute()
{
	logger->Enter("Computing Block RandomPoints");
	sf::Vector2u bounds = in_size->GetData();
	std::set<sf::Vector2f, cmp> result;
	result.emplace(0, 0);
	result.emplace(0, bounds.y);
	result.emplace(bounds.x, 0);
	result.emplace(bounds.x, bounds.y);
	while (result.size() < bounds.x * bounds.y / 100)
	{
		int int_part_x = random_int(0, bounds.x - 1);
		int int_part_y = random_int(0, bounds.y - 1);
		float frac_part_x = (random_int(0, 128))/128.0;
		float frac_part_y = (random_int(0, 128))/128.0;
		result.emplace(int_part_x + frac_part_x,
			int_part_y + frac_part_y);
	}
	std::vector<sf::Vector2f> result_vector;
	for (auto p: result)
		result_vector.push_back(p);
	SetData(out_points, std::move(result_vector));
	logger->Exit();
}

RandomPoints::RandomPoints(std::string _name, Logger* _logger)
: Block(_name, _logger)
{
	in_size = new Input<sf::Vector2u>(this);
	out_points = new Output<std::vector<sf::Vector2f>>(this);
}
