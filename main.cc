#include "Logger.hh"
#include "ImageLoader.hh"
#include "ImageDrawer.hh"
#include "ImageSaver.hh"
#include "RandomTriangles.hh"
#include "PointsDrawer.hh"
#include "RandomPoints.hh"

class SizePrinter: public Block
{
public:
	Input<sf::Vector2u>* in;
protected:
	virtual void Compute() override
	{
		logger->Log() << in->GetData().x <<" " <<in->GetData().y <<"\n";
	}
public:
	SizePrinter(std::string _name, Logger* _logger)
	:Block(_name, _logger)
	{
		in = new Input<sf::Vector2u>(this);
	}
};

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

	ImageDrawer drawer("test_drawer", &logger);
	drawer.picture->Connect(loader.out);
	//drawer.ManualUpdate();

	ImageSaver saver("test_saver", &logger);
	saver.picture->Connect(loader.out);
	std::string path_out(argv[1]);
	path_out.insert(path_out.size()-4, "_save_test");
	ManualInput<std::string> file_out(path_out);
	saver.path->Connect(&file_out);
	//saver.ManualUpdate();

	RandomTriangles randtris("test_randtris", &logger);
	randtris.input_image->Connect(loader.out);
	randtris.ManualUpdate();
	RandomPoints points("test_random_points", &logger);
	points.size->Connect(loader.size);

	PointsDrawer drawer("test_points_drawer", &logger);
	drawer.size->Connect(loader.size);
	drawer.points->Connect(points.points);
	drawer.ManualUpdate();

	// ImageDrawer drawer("test_drawer", &logger);
	// drawer.picture->Connect(loader.picture);
	// drawer.ManualUpdate();

	// SizePrinter printer("test_printer", &logger);
	// printer.in->Connect(loader.size);
	// printer.ManualUpdate();

	// ImageSaver saver("test_saver", &logger);
	// saver.picture->Connect(loader.picture);
	// std::string path_out(argv[1]);
	// path_out.insert(path_out.size()-4, "_save_test");
	// ManualInput<std::string> file_out(path_out);
	// saver.path->Connect(&file_out);
	// saver.ManualUpdate();

	return 0;
}
