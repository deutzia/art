#include "ImageLoader.hh"

void ImageLoader::Compute()
{
	logger->Enter("Computing Block ImageLoader");
	sf::Texture texture;
	if (!texture.loadFromFile(in_file->GetData()))
	{
		std::string msg = "Failed to open file " + in_file->GetData();
		logger->Log(Logger::LogLevel::Error) << msg;
		throw msg;
	}
	SetData(out_size, texture.getSize());
	SetData(out_picture, std::move(texture));
	logger->Exit();
}

ImageLoader::ImageLoader(std::string _name, Logger* _logger)
: Block(_name, _logger)
{
	in_file = new Input<std::string>(this);
	out_picture = new Output<sf::Texture>(this);
	out_size = new Output<sf::Vector2u>(this);
}

