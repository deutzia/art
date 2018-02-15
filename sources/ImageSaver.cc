#include "ImageSaver.hh"

void ImageSaver::Compute()
{
	if (!in_texture->GetData().copyToImage().saveToFile(in_path->GetData()))
	{
		std::string error_msg = "Failed to save image file: " + in_path->GetData();
		logger.LogE() << error_msg;
		throw error_msg;
	}
}

ImageSaver::ImageSaver(std::string _name)
: Block(_name)
{
	in_texture = new Input<sf::Texture>(this);
	in_path = new Input<std::string>(this);
}

