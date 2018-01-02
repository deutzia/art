#include "AverageColorTriangles.hh"
#include "DelaunayTriangulation.hh"
#include "ImageDrawer.hh"
#include "ImageLoader.hh"
#include "ImageSaver.hh"
#include "Logger.hh"
#include "MassCenterColorTriangles.hh"
#include "PointsDrawer.hh"
#include "PrefixSumsOnImage.hh"
#include "RandomPoints.hh"
#include "RandomTriangles.hh"
#include "TrianglesToTexture.hh"


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

	RandomPoints rand_points("RandomPoints", &logger);
	rand_points.in_size->Connect(loader.out_size);

	DelaunayTriangulation delaunay("DelaunayTriangulation", &logger);
	delaunay.in_points->Connect(rand_points.out_points);

	PrefixSumsOnImage prefs("PrefixSumsOnImage", &logger);
	prefs.in_texture->Connect(loader.out_texture);

	AverageColorTriangles avg_color("AverageColorTriangles", &logger);
	avg_color.in_color_prefix->Connect(prefs.out_array);
	avg_color.in_triangles->Connect(delaunay.out_triangles);

	TrianglesToTexture tristotex("test_tristotex", &logger);
	tristotex.in_size->Connect(loader.out_size);
	tristotex.in_triangles->Connect(avg_color.out_triangles);

	ImageDrawer drawer("test_drawer", &logger);
	drawer.in_texture->Connect(tristotex.out_texture);
	drawer.ManualUpdate();

	ImageSaver saver("test_saver", &logger);
	saver.in_texture->Connect(tristotex.out_texture);

	ManualInput<std::string> savepath("pictures/bichon_v3.jpg");
	saver.in_path->Connect(&savepath);
	saver.ManualUpdate();

	return 0;
}
