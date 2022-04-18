#pragma once
#ifndef __TRAVELER_H_INCLUDED__
#define __TRAVELER_H_INCLUDED__
#include <vector>
#include <utility>

/**
Class Traveler finds the shortest path through given points
*/
class Traveler
{
public:

	/**
	Initialize class with points vector
	@param inputPoints: pointer to vector containing points
	*/
	Traveler(std::vector<int> inputPoints);

	// remove default constructor
	Traveler() = delete;

	/**
	Resets the result field variables
	Good for comparing different iteration/permutation methods
	*/
	void reset();

	// Restuls

	/**
	* returns the best distance
	*/
	double getBestDist() { return bestDist; };

	/** returns time to finish */
	double getTimeToFinish() { return timeToFinish; };

	/** returns time to find best path */
	double getTimeToFindBest() { return timeToFindBest; };

	/** returns vector containing best path indizes */
	std::vector<int> getBestPathIndizes() { return bestPathIndizes; };

	/** returns if best path method has already been run */
	bool hasSearched() { return searched; };

	/**
	* Calculates the distance between two points
	* @param x0 xCoord of first point
	* @param y0 yCoord of first point
	* @param x1 xCoord of second point
	* @param y1 yCoord of second point
	*/
	static double calcDist(int x0, int y0, int x1, int y1);

	/**
	Creates a distance mat
	@param points 
	*/
	std::vector<double> createDistMat(std::vector<int> points);

protected:
	/** Number of points */
	int nPoints;
	/** Length of shortest distance */
	double bestDist;
	/** Time total to finish search algorithm */
	double timeToFinish;
	/** Time to find shortest path */
	double timeToFindBest;
	/** Indizes of shortest path (not including starting point index 0) */
	std::vector<int> bestPathIndizes;
	/** true if executed a path searching algorithm else false */
	bool searched;

	/**
	Points
	*/
	std::vector<int> points;

};

#endif //__TRAVELER_H_INCLUDED__