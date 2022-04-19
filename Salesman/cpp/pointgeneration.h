#pragma once
#ifndef __POINTGENERATION_H_INCLUDED__
#define __POINTGENERATION_H_INCLUDED__

#include <vector>

/**
* @file
* 
* Creates a vector of points.
*
* Even entries represent y-coords.
* Uneven entries represent x-coords.
*
* @param nPoints number of points to generate
* @param points vector, either empty or containing predefined points
* @param imgPixels height/width of pixels in image, assumes square image
* @param seed seed initializing point generation
* @param padding minimum distance to edge of image
* @param minPointDist minimum distance between points
*
* @return vector of points created
*/
std::vector<int> createPoints(
    int const nPoints,
    std::vector<int> & points,
    int const imgPixels,
    int const seed = 711,
    int const padding = 40,
    int const minPointDist = 40);

#endif


