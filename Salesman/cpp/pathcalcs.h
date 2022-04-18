#pragma once

#ifndef __PATHCALCS_H_INCLUDED__
#define __PATHCALCS_H_INCLUDED__

#include <deque>

void fillDistanceMat(std::deque<int> points, const int nPoints, double distanceMat[]);

std::deque<int> bruteforce(std::deque<int> points, const int nPoints, double distanceMat[]);

#endif
