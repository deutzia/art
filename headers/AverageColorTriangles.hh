#pragma once
#include <algorithm>
#include <list>
#include <vector>
#include "Block.hh"
#include "Tools.hh"

class AverageColorTriangles: public Block
{
private:
	void RecolorTriangle(Triangle&, std::vector<std::vector<ColorPrefix>>&);

public:
	Input<std::vector<std::vector<ColorPrefix>>>* in_color_prefix;
	Input<std::vector<Triangle>>* in_triangles;
	Output<std::vector<Triangle>>* out_triangles;
protected:
	virtual void Compute() override;
public:
	AverageColorTriangles(std::string _name, Logger* _logger);
};
