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

	RandomTriangles randtris("test_randtris", &logger);
	randtris.size->Connect(loader.size);
	randtris.ManualUpdate();

	return 0;
}
