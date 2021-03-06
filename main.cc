#include "AverageColorTriangles.hh"
#include "DelaunayTriangulation.hh"
#include "ImageDrawer.hh"
#include "ImageLoader.hh"
#include "ImageSaver.hh"
#include "MassCenterColorTriangles.hh"
#include "PointsDrawer.hh"
#include "PrefixSumsOnImage.hh"
#include "RandomPoints.hh"
#include "RandomTriangles.hh"
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

	PrefixSumsOnImage prefsums("prefsums");
	prefsums.in_texture->Connect(loader.out_texture);





	TrianglesToTexture tris1("tris1");
	TrianglesToTexture tris1_2("tris1_2");

	RandomPoints randpoints("tris1_randpoints");
	randpoints.in_size->Connect(loader.out_size);

	DelaunayTriangulation delaunay("tris1_delaunay");
	delaunay.in_points->Connect(randpoints.out_points);

	AverageColorTriangles avgcolor("tris1_avgcolor");
	avgcolor.in_triangles->Connect(delaunay.out_triangles);
	avgcolor.in_color_prefix->Connect(prefsums.out_array);

	tris1.in_size->Connect(loader.out_size);
	tris1.in_triangles->Connect(avgcolor.out_triangles);


	MassCenterColorTriangles centcolor("tris1_2_centcolor");
	centcolor.in_texture->Connect(loader.out_texture);
	centcolor.in_triangles->Connect(delaunay.out_triangles);

	tris1_2.in_size->Connect(loader.out_size);
	tris1_2.in_triangles->Connect(centcolor.out_triangles);





	TrianglesToTexture tris2("tris2");
	TrianglesToTexture tris2_2("tris2_2");

	RandomTriangles randtris2("tris2_randtris");
	ManualInput<int> in_iterations(10000);
	randtris2.in_size->Connect(loader.out_size);
	randtris2.in_iterations->Connect(&in_iterations);

	AverageColorTriangles avgcolor2("tris2_avgcolor");
	avgcolor2.in_triangles->Connect(randtris2.out_triangles);
	avgcolor2.in_color_prefix->Connect(prefsums.out_array);

	tris2.in_size->Connect(loader.out_size);
	tris2.in_triangles->Connect(avgcolor2.out_triangles);



	MassCenterColorTriangles centcolor2("tris2_2_centcolor");
	centcolor2.in_texture->Connect(loader.out_texture);
	centcolor2.in_triangles->Connect(randtris2.out_triangles);

	tris2_2.in_size->Connect(loader.out_size);
	tris2_2.in_triangles->Connect(centcolor2.out_triangles);
	
	



	SideBySideCompare<3, 2> sidebyside("test_sidebyside");
	sidebyside.in_size->Connect(loader.out_size);
	sidebyside.in_textures[0][0]->Connect(loader.out_texture);
	sidebyside.in_textures[1][0]->Connect(tris1.out_texture);
	sidebyside.in_textures[2][0]->Connect(tris2.out_texture);
	sidebyside.in_textures[0][1]->Connect(loader.out_texture);
	sidebyside.in_textures[1][1]->Connect(tris1_2.out_texture);
	sidebyside.in_textures[2][1]->Connect(tris2_2.out_texture);
	
	ImageDrawer drawer("test_drawer");
	drawer.in_texture->Connect(sidebyside.out_texture);
	drawer.ManualUpdate();

	ImageSaver saver("test_saver");
	saver.in_texture->Connect(sidebyside.out_texture);

	ManualInput<std::string> savepath("pictures/save_test.jpg");
	saver.in_path->Connect(&savepath);
	saver.ManualUpdate();

	return 0;
}
