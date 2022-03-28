#pragma once
#ifndef __SHORTEST_H_INCLUDED__
#define __SHROTEST_H_INCLUDED__

#include "Traveler.h"
#include <vector>

class Shortest : public Traveler
{
public:

	/*
	* Initializes Shortes class
	*/
	Shortest(std::vector<int> inputPoints);

	/*
	* Takes the next point based on the closest point
	*/
	void takeClosestPoint();

};

#endif // __SHROTEST_H_INCLUDED__