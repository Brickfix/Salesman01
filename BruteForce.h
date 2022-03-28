#pragma once
#ifndef __BRUTEFORCE_H_INCLUDED__
#define __BRUTEFORCE_H_INCLUDED__
#include <vector>
#include <utility>
#include "Traveler.h"

/*
* Class Traveler finds the shortest path through given points
*/
class BruteForce : public Traveler
{
public:

	/*
	* Iterates through the given points
	* Returns the indexes and the best distance
	*/
	void iterateThroughPoints();

	/*
	* Permutates only half of the point while
	* respecting that backwards paths don't add
	* any new information, left-round or right-round
	* should be identical
	*/
	void permutateHalf();


	/*
	* Initialize class with points vector
	* @param points: pointer to vector containing points
	*/
	BruteForce(std::vector<int> inputPoints);

};

#endif //__BRUTEFORCE_H_INCLUDED__