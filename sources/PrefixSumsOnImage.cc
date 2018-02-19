#include "PrefixSumsOnImage.hh"

void PrefixSumsOnImage::Compute()
{
	sf::Texture texture = in_texture->GetData();
	sf::Image image = texture.copyToImage();
	sf::Vector2u size = image.getSize();
	std::vector<std::vector<ColorPrefix>> result;
	std::vector<ColorPrefix> empty;
	result.reserve(size.x);
	for (unsigned i = 0; i < size.x; ++i)
	{
		result.push_back(empty);
		result[i].reserve(size.y);
	}
	for (unsigned i = 0; i < size.x; ++i)
	{
		result[i].emplace_back(image.getPixel(i, 0));
		for (unsigned j = 1; j < size.y; ++j)
			result[i].emplace_back(result[i][j-1] + image.getPixel(i, j));
	}
	SetData(out_array, std::move(result));
}
    
PrefixSumsOnImage::PrefixSumsOnImage(std::string _name)
: Block(_name)
{
	in_texture = new Input<sf::Texture>(this);
	out_array = new Output<std::vector<std::vector<ColorPrefix>>>(this);
}
