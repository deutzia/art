#pragma once
#include <algorithm>
#include <list>
#include <vector>
#include "Block.hh"
#include "Tools.hh"

/*
 * AverageColorTriangles Block
 *   Given a precalculated color prefix sum of the image as well as a division
 *   of the image into triangles, averages the color of each triangle.
 *   The triangles must be contained within the image dimensions (the dimen-
 *   sions of the prefix sum), otherwise the result is undefined.
 */

class AverageColorTriangles: public Block
{
private:
	void RecolorTriangle(Triangle&, std::vector<std::vector<ColorPrefix>>&);

public:
	// Vector of vectors of color prefix sums.
	// in_color_prefix[x][y] should be a sum of pixels 0, ..., y in the 
	// x-th column
	Input<std::vector<std::vector<ColorPrefix>>>* in_color_prefix;
 	// The triangles must be contained within the image dimensions (the dimen-
 	// sions of the prefix sum), otherwise the result is undefined.
	Input<std::vector<Triangle>>* in_triangles;

	Output<std::vector<Triangle>>* out_triangles;
protected:
	virtual void Compute() override;
public:
	AverageColorTriangles(std::string _name, Logger* _logger);
};
