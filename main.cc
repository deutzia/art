#include "AverageColorTriangles.hh"
#include "DelaunayTriangulation.hh"
#include "ImageDrawer.hh"
#include "ImageLoader.hh"
#include "ImageSaver.hh"
#include "JuxtaposeDisplay.hh"
#include "MassCenterColorTriangles.hh"
#include "PointsDrawer.hh"
#include "PrefixSumsOnImage.hh"
#include "RandomPoints.hh"
#include "RandomTriangles.hh"
#include "ShaderTest.hh"
#include "SideBySideCompare.hh"
#include "TrianglesToTexture.hh"
// for some reasons this has to be at the end of headers, otherwise
// it does not compile; just leave it be
#include <X11/Xlib.h>

int main(int argc, char* argv[])
{
	XInitThreads();
	
	Block::logger.SetLoggerLevel(Logger::LogLevel::Debug);
	if (argc != 2)
	{
		Block::logger.Log(Logger::LogLevel::Error) << "Please provide an image file for testing.";
		return -1;
	}
	
	ImageLoader loader("loader");
	ManualInput<std::string> in_file(argv[1]);
	loader.in_file->Connect(&in_file);

	ShaderTest shader("shader");
	ManualInput<int> kernel(3);
	shader.in_kernel_size->Connect(&kernel);
	shader.in_texture->Connect(loader.out_texture);

	JuxtaposeDisplay drawer("drawer");
	drawer.in_texture_left->Connect(loader.out_texture);
	drawer.in_texture_right->Connect(shader.out_texture);
	drawer.ManualUpdate();

	ImageSaver saver("test_saver");
	saver.in_texture->Connect(shader.out_texture);
	ManualInput<std::string> savepath("pictures/save_test.jpg");
	saver.in_path->Connect(&savepath);
	saver.ManualUpdate();

	return 0;
}
