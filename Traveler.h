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
	* Iterates through the given points
	* Returns the indexes and the best distance
	*/
	void iterateThroughPoints();

	/*
	* Initialize class with points vector
	* @param points: vector containing points
	*/
	Traveler(std::vector<int> inputPoints);

	// Restuls

	/* Length of shortest distance */
	double bestDist;
	/* Time total to finish search algorithm */
	double timeToFinish;
	/* Time to find shortest path */
	double timeToFindBest;
	/* Indizes of shortest path (not including starting point index 0) */
	std::vector<int> bestPathIndizes;
	/* true if executed a path searching algorithm else false */
	bool searched;

private:
	/*
	* Number of points
	*/
	int nPoints;

	/*
	* Points
	*/
	std::vector<int> points;

	/*
	* Creates a distance mat
	*/
	std::vector<double> createDistMat(std::vector<int> points);


	/*
	* Calculates the difference between two points
	*/
	double calcDist(int x0, int y0, int x1, int y1);
};

#endif __TRAVELER_H_INCLUDED__