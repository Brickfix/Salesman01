#pragma once
#ifndef __TRAVELER_H_INCLUDED__
#define __TRAVELER_H_INCLUDED__
#include <vector>
#include <utility>

/*
* Class Traveler finds the shortest path through given points
*/
class Traveler
{
public:
	/*
	* Distance Matrix between all points
	*/
	std::vector<double> distMat;

	/*
	* Iterates through the given points
	* Returns the indexes and the best distance
	* 
	* @return std::pair<std::vector<int>,double> (indexes, bestDistance)
	*/
	std::pair<std::vector<int>, double> iterateThroughPoints();

	/*
	* Initialize class with points vector
	* @param points: vector containing points
	*/
	Traveler(std::vector<int> points);

private:
	/*
	* Number of points
	*/
	int nPoints;

	/*
	* Creates a distance mat
	*/
	void createDistMat(std::vector<int> points);
};

#endif __TRAVELER_H_INCLUDED__