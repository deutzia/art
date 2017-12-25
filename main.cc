#include "Logger.hh"
#include "ImageLoader.hh"
#include "ImageDrawer.hh"
#include "ImageSaver.hh"
#include "RandomTriangles.hh"
#include "PointsDrawer.hh"
#include "RandomPoints.hh"

int main(int argc, char* argv[])
{
	Logger logger;
	logger.SetLoggerLevel(Logger::LogLevel::Debug);
	if (argc != 2)
	{
		logger.Log(Logger::LogLevel::Error) << "Please provide an image file for testing.";
		return -1;
	}
	
	ImageLoader loader("test_loader", &logger);
	ManualInput<std::string> file_in(argv[1]);
	loader.file->Connect(&file_in);

	// RandomTriangles randtris("test_randtris", &logger);
	// randtris.size->Connect(loader.size);

	// ImageSaver saver("test_saver", &logger);
	// saver.picture->Connect(randtris.output_image);

	// ManualInput<std::string> savepath("pictures/bichon_save_test.jpg");
	// saver.path->Connect(&savepath);
	// saver.ManualUpdate();

	return 0;
}
