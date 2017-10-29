#include "ImageLoader.hh"

void ImageLoader::Compute()
{
	logger->Enter("Computing Block ImageLoader");
	sf::Texture texture;
	if (!texture.loadFromFile(file->GetData()))
	{
		logger->Log(Logger::LogLevel::Error) << "Failed to open file " <<file->GetData();
		throw std::string("Failed to open file ")+(file->GetData());
	}
	SetData(out, std::move(texture));
	logger->Exit();
}

ImageLoader::ImageLoader(std::string _name, Logger* _logger)
: Block(_name, _logger)
{
	file = new Input<std::string>(this);
	out = new Output<sf::Texture>(this);
}
