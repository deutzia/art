#include "Logger.hh"
#include "ImageLoader.hh"
#include "ImageDrawer.hh"
#include "ImageSaver.hh"

int main(int argc, char* argv[])
{
	Logger logger;
	if (argc != 2)
	{
		logger.Log(Logger::LogLevel::Error) << "Please provide an image file for testing.";
		return -1;
	}
	
	ImageLoader loader("test_loader", &logger);
	ManualInput<std::string> file_in(argv[1]);
	loader.file->Connect(&file_in);

	ImageDrawer drawer("test_drawer", &logger);
	drawer.picture->Connect(loader.out);
	drawer.ManualUpdate();

	ImageSaver saver("test_saver", &logger);
	saver.picture->Connect(loader.out);
	std::string path_out(argv[1]);
	path_out.insert(path_out.size()-4, "_save_test");
	ManualInput<std::string> file_out(path_out);
	saver.path->Connect(&file_out);
	saver.ManualUpdate();

	return 0;
}
