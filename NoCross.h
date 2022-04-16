#pragma once
#ifndef __NOCROSS_H_INCLUDED__
#define __NOCROSS_H_INCLUDED__

#include "Traveler.h"
#include <vector>
#include <unordered_map>
#include <string>

/** 
* Point specified by coords and index
*/
class Point {
public:
	int x;
	int y;
	int index;

	/** 
	Initialize new empty point with default values
	*/
	Point();

	/*
	Initialize new point
	@param int x
	@param int y
	@param int index
	*/
	Point(int x, int y, int index);

	/** 
	* Sets == operator
	*/
	bool operator==(const Point& pointToCompare) const;

	/**
	* Sets < operator
	*/
	bool operator<(const Point& pointToCompare) const;

	/**
	* Hash function for maps, sets etc
	*/
	struct HashFunction;
};

/**
* Edge between two points
* Contains utility function to check if edge crosses other edge
*/
class Edge {
public:
	Point p1;
	Point p2;


	/**
	Initialize new Edge
	@param Point p1
	@param Point p2
	*/
	Edge(Point p1, Point p2);

	/**
	convert to String
	*/
	std::string toString();

	/**
	Specify == operator
	@param edge
	*/
	bool operator==(const Edge& edge) const;

	/** Specify < operator
	@param edge
	@return bool*/
	bool operator<(const Edge& edge) const;

	/**
	Returns true if to edges intersect
	
	@param edge
	@return bool
	*/
	bool intersects(const Edge& edge);

	/**
	Reverses points p1 and p2
	*/
	void reverse();

	/**
	For maps etc
	*/
	struct HashFunction;
};

/**
* NoCross tries to find a path without crossing edges.
* Uses similar Algorithm to Shortest
*/
class NoCross : public Traveler
{
public:
	NoCross(std::vector<int> inputPoints);

	/** Finds a path without crossing to minimize distance */
	void NextPointNoCross();

	/**
	Generates a path with the next point chosen as the closest point
	@param std::vector<double> &	 distMat
	@param std::unordered_map<int, Edge>& indexToEdge
	*/
	void shortestPathIndizes(std::vector<double> & distMat, std::unordered_map<int, Edge>& indexToEdge);

	/**
	Iterate over path until no crossing edges found
	@param indexToEdge
	*/
	void iterateOverPath(std::unordered_map<int, Edge>& indexToEdge);


	/**
	Finds and reorders edges crossing each other
	@param indexToEdge
	*/
	bool reorderedCrossingEdges(std::unordered_map<int, Edge>& indexToEdge);

	/**
	creates index containing solution
	Resets bestPathIndizes and bestDest
	@param distMat
	@param indexToEdge
	*/
	void edgeMapToIndexVector(std::vector<double>& distMat, std::unordered_map<int, Edge>& indexToEdge);

};

#endif // __NOCROSS_H_INCLUDED__