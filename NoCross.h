#pragma once
#ifndef __NOCROSS_H_INCLUDED__
#define __NOCROSS_H_INCLUDED__

#include "Traveler.h"
#include <vector>
#include <unordered_map>
#include <string>


class Point {
public:
	int x;
	int y;
	int index;

	Point();

	Point(int x, int y, int index);

	bool operator==(const Point& pointToCompare) const;

	bool operator<(const Point& pointToCompare) const;

	struct HashFunction;
};

class Edge {
public:
	Point p1;
	Point p2;

	Edge(Point p1, Point p2);

	std::string toString();

	bool operator==(const Edge& edge) const;

	//for set, set orders it for us
	bool operator<(const Edge& edge) const;

	/*
	* Returns true if to edges intersect
	* 
	* @param edge
	* @return bool
	*/
	bool intersects(const Edge& edge);

	/*
	* Reverses points p1 and p2
	*/
	void reverse();

	/*
	* For maps etc
	*/
	struct HashFunction;
};

class NoCross : public Traveler
{
public:
	NoCross(std::vector<int> inputPoints);

	/* Finds a path without crossing to minimize distance */
	void NextPointNoCross();

	/*
	* Generates a path with the next point chosen as the closest point
	* @param std::vector<double> &	 distMat
	* @param std::unordered_map<int, Edge>& indexToEdge
	*/
	void shortestPathIndizes(std::vector<double> & distMat, std::unordered_map<int, Edge>& indexToEdge);

	/* Iterate over path until no crossing edges found */
	void iterateOverPath(std::unordered_map<int, Edge>& indexToEdge);


	/* Finds and reorders edges crossing each other */
	bool reorderedCrossingEdges(std::unordered_map<int, Edge>& indexToEdge);

	/* 
	* creates index containing solution
	Resets bestPathIndizes and bestDest
	*/
	void edgeMapToIndexVector(std::vector<double>& distMat, std::unordered_map<int, Edge>& indexToEdge);

};

#endif // __NOCROSS_H_INCLUDED__