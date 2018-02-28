#include "ImageLoader.hh"

void ImageLoader::Compute()
{
	sf::Texture texture;
	if (!texture.loadFromFile(in_file->GetData()))
	{
		std::string msg = "Failed to open file " + in_file->GetData();
		logger.LogE() << msg;
		throw msg;
	}
	texture = LimitTextureSize(texture);
	SetData(out_size, texture.getSize());
	SetData(out_texture, std::move(texture));
}

ImageLoader::ImageLoader(std::string _name)
: Block(_name)
{
	in_file = new Input<std::string>(this);
	out_texture = new Output<sf::Texture>(this);
	out_size = new Output<sf::Vector2u>(this);
}

