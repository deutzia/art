#include "Logger.hh"
#include "ImageLoader.hh"
#include "ImageDrawer.hh"

int main(int argc, char *argv[])
{
	Logger logger;
	if (argc != 2)
	{
		logger.Log(Logger::LogLevel::Error) <<"need a file";
		return -1;
	}
	ManualInput<std::string> file;
	file.SetDataManually(std::string(argv[1]));
	ImageLoader loader("loader", &logger);
	ImageDrawer drawer("drawer", &logger);
	loader.file->Connect(&file);
	drawer.picture->Connect(loader.out);
	drawer.ManualUpdate();
	return 0;
}
