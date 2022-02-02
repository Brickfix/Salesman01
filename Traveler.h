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
	* Points to iterate through
	*/
	std::vector<int> points;

	/*
	* Distance Matrix between all points
	*/
	std::vector<double> distMat;

	/*
	* Creates a distance mat
	*/
	void createDistMat();

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
};

#endif __TRAVELER_H_INCLUDED__