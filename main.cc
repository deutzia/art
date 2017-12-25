#include "Logger.hh"
#include "ImageLoader.hh"
#include "ImageDrawer.hh"
#include "ImageSaver.hh"
#include "RandomTriangles.hh"
#include "TrianglesToTexture.hh"
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
	ManualInput<std::string> in_file(argv[1]);
	loader.in_file->Connect(&in_file);

	RandomTriangles randtris("test_randtris", &logger);
	randtris.in_size->Connect(loader.out_size);

	TrianglesToTexture tristotex("test_tristotex", &logger);
	tristotex.in_size->Connect(loader.out_size);
	tristotex.in_triangles->Connect(randtris.out_triangles);

	ImageDrawer drawer("test_drawer", &logger);
	drawer.in_picture->Connect(tristotex.out_texture);
	// drawer.ManualUpdate();

	ImageSaver saver("test_saver", &logger);
	saver.in_picture->Connect(tristotex.out_texture);

	ManualInput<std::string> savepath("pictures/bichon_save_test.jpg");
	saver.in_path->Connect(&savepath);
	saver.ManualUpdate();

	return 0;
}
