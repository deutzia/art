#include "HilbertCurvePoints.hh"

sf::Vector2f HilbertCurvePoints::CoordsFromDist(uint32_t s, int n)
{
	uint32_t x = 0, y = 0;
	for (int i = 0; i < 2 * n; i += 2)
	{
		int sa = (s >> (i + 1)) & 1; // Get bit i + 1 of s.
		int sb = (s >> i) & 1;	// Get bit i of s.

		if ((sa ^ sb) == 0) // If sa,sb = 00 or 11,
		{
			uint32_t temp = x; // swap x and y,
			x = y ^ (-sa); // and if sa = 1,
			y = temp ^ (-sa); // complement them.
		}
		x = (x >> 1) | (sa << 31); // Prepend sa to x and
		y = (y >> 1) | ((sa ^ sb) << 31); // (sa ^ sb) to y.
	}
	return sf::Vector2f(x >> (32 - n), y >> (32 - n));
}

void HilbertCurvePoints::Compute()
{
	logger->Enter("Computing Block HilbertCurvePoints");

	sf::Vector2u size = in_size->GetData();
	std::vector<sf::Vector2f> points;
	std::vector<std::vector<uint32_t>> values = in_values->GetData();

	// find order of curve to be generated
	uint32_t n = 1, s = std::max(size.x, size.y);
	while (n < 16 && (1u << n) < s)
		n++;

	logger->LogV() << "Generating Hilbert Curve of order " << n;

	double hil_size = (1 << n);
	double multiplier_x = size.x / hil_size;
	double multiplier_y = size.y / hil_size;

	uint32_t long long sum = 0;
	for (uint32_t x = 0; x < size.x; ++x)
		for (uint32_t y = 0; y < size.y; ++y)
			sum += values[x][y];
	// when accumulated sum is bigger than this value, a point should be placed
	uint32_t border_value = sum / (size.x * size.y) * 100;

	// place points along the curve when details are detected
	sum = 0;
	auto prog = logger->InitializeProgress(
		Logger::LogLevel::Verbose, 0, (1u << (2*n)));
	for (uint32_t i = 0; i < (1u << (2*n)); ++i)
	{
		sf::Vector2f p = CoordsFromDist(i, n);
		p.x = p.x * multiplier_x;
		p.y = p.y * multiplier_y;
		sum += values[p.x][p.y];
		if (sum >= border_value)
		{
			points.push_back(p);
			sum -= border_value;
		}
		prog.Count();
	}

	SetData(out_points, std::move(points));

	logger->Exit();
}

HilbertCurvePoints::HilbertCurvePoints(std::string _name, Logger* _logger)
: Block(_name, _logger)
{
	in_size = new Input<sf::Vector2u>(this);
	in_values = new Input<std::vector<std::vector<uint32_t>>>(this);
	out_points = new Output<std::vector<sf::Vector2f>>(this);
}
