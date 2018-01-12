#include "BasicEdgeDetection.hh"

unsigned BasicEdgeDetection::PixelsDifference(sf::Color c1, sf::Color c2)
{
	return std::abs(int(c1.r) - c2.r) + std::abs(int(c1.g) - c2.g)
		+ std::abs(int(c1.b) - c2.b) + std::abs(int(c1.a) - c2.a);
}

unsigned BasicEdgeDetection::DifferencesFromNeighbours(int x, int y)
{
	unsigned result = 0;
	for (int i = 0; i < 4; ++i)
	{
		int nx = x + BasicEdgeDetection::translations[i][0];
		int ny = y + BasicEdgeDetection::translations[i][1];
		if (0 <= nx && nx < size.x && 0 <= ny && ny < size.y)
			result += BasicEdgeDetection::PixelsDifference(image.getPixel(x, y),
				image.getPixel(nx, ny));
	}
	return pow(result, 1.5);
}

void BasicEdgeDetection::Compute()
{
	logger->Enter("Computing Block BasicEdgeDetection");

	sf::Texture texture = in_texture->GetData();
	size = texture.getSize();
	image = texture.copyToImage();

	std::vector<std::vector<unsigned>> values;
	values.reserve(size.x);
	for (unsigned x = 0; x < size.x; ++x)
	{
		values.emplace_back();
		values[x].reserve(size.y);
		for (unsigned y = 0; y < size.y; ++y)
			values[x].push_back(0);
	}

	for (unsigned x = 0; x < size.x; ++x)
		for (unsigned y = 0; y < size.y; ++y)
			values[x][y] = DifferencesFromNeighbours(x, y);

	SetData(out_values, std::move(values));

	logger->Exit();
}

BasicEdgeDetection::BasicEdgeDetection(std::string _name, Logger* _logger)
: Block(_name, _logger)
{
	in_texture = new Input<sf::Texture>(this);
	out_values = new Output<std::vector<std::vector<unsigned>>>(this);
}
