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
	std::vector<std::set<uint32_t, bool(*)(uint32_t,uint32_t)>> neighbours;
public:
	Input<std::vector<sf::Vector2f>>* in_points; 
	Output<std::vector<Triangle>>* out_triangles;
private:
	bool InCircle(uint32_t, uint32_t, uint32_t, uint32_t);
	// sort clockwise points in given vector around given point
	void AngularSort(std::vector<uint32_t>&, uint32_t);
	void AddEdge(uint32_t, uint32_t);
	void RemoveEdge(uint32_t, uint32_t);
	std::vector<uint32_t> ConvexHull(uint32_t, uint32_t);
	void Delaunay(uint32_t, uint32_t);
protected:
	virtual void Compute() override;
public:
	DelaunayTriangulation(std::string _name, Logger* _logger);
};
