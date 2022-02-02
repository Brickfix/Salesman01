#pragma once
#ifndef __POINTGENERATION_H_INCLUDED__
#define __POINTGENERATION_H_INCLUDED__

#include <vector>
#include <random>
#include <cmath>

/**
* Creates a deque of points.
*
* Even entries represent y-coords.
* Uneven entries represent x-coords.
*
* @param nPoints number of points to generate
* @param imgPixels height/width of pixels in image, assumes square image
* @param padding minimum distance to edge of image
* @param minPointDist minimum distance between points
*
* @return vector of points created
*/
std::vector<int> createPoints(int const nPoints, int const imgPixels, int const padding = 40, int const minPointDist = 40);

#endif


