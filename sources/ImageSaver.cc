#include "ImageSaver.hh"

void ImageSaver::Compute()
{
	logger->Enter("Computing Block ImageSaver");

	if (!in_picture->GetData().copyToImage().saveToFile(in_path->GetData()))
	{
		std::string error_msg = "Failed to save image file: " + in_path->GetData();
		logger->Log(Logger::LogLevel::Error) << error_msg;
		throw error_msg;
	}

	logger->Exit();
}

ImageSaver::ImageSaver(std::string _name, Logger* _logger)
: Block(_name, _logger)
{
	in_picture = new Input<sf::Texture>(this);
	in_path = new Input<std::string>(this);
}

