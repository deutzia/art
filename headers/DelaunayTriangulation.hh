#pragma once
#include <algorithm>
#include <iomanip>
#include <set>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Block.hh"
#include "Tools.hh"

class DelaunayTriangulation: public Block
{
private:
	std::vector<sf::Vector2f> points;
	std::vector<std::set<unsigned>> neighbours;
public:
	Input<std::vector<sf::Vector2f>>* in_points; 
	Output<std::vector<Triangle>>* out_triangles;
private:
	bool InCircle(unsigned, unsigned, unsigned, unsigned);
	// sort clockwise points in given vector around given point
	void AngularSort(std::vector<unsigned>&, unsigned);
	void AddEdge(unsigned, unsigned);
	void RemoveEdge(unsigned, unsigned);
	std::vector<unsigned> ConvexHull(unsigned, unsigned);
	void Delaunay(unsigned, unsigned);
protected:
	virtual void Compute() override;
public:
	DelaunayTriangulation(std::string _name, Logger* _logger);
};
