#pragma once
#ifndef __NOCROSS_H_INCLUDED__
#define __NOCROSS_H_INCLUDED__

#include "Traveler.h"
#include <vector>

class NoCross : public Traveler
{
public:
	NoCross(std::vector<int> inputPoints);

	void NextPointNoCross();
};

#endif // __NOCROSS_H_INCLUDED__