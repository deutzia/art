#include "ImageSaver.hh"

void ImageSaver::Compute()
{
	logger->Enter("Computing Block ImageSaver");

	if (!picture->GetData().copyToImage().saveToFile(path->GetData()))
	{
		std::string error_msg = "Failed to save image file: " + path->GetData();
		logger->Log(Logger::LogLevel::Error) << error_msg;
		throw error_msg;
	}

	logger->Exit();
}

ImageSaver::ImageSaver(std::string _name, Logger* _logger)
: Block(_name, _logger)
{
	picture = new Input<sf::Texture>(this);
	path = new Input<std::string>(this);
}

