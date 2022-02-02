#pragma once
// import guard
#ifndef __IMGFUNCS_H_INCLUDED__
#define __IMGFUNCS_H_INCLUDED__

#include <vector>

#include "Libraries/blend2d/src/blend2d.h"

/**
* Saves the image
* @param img : input image
* @param name : Name of the file
*/
void saveBLImg(BLImage img, const char* name);

/**
* creates an image of the points with connecting lines between the points
* 
* @param points :		odd entries specify x, even entries specify y
* @param length :		pixels of the square image per side
* @param pointRadius :	radius of the circle drawn per point
*/
BLImage createImage(std::vector<int> points, int length, int pointRadius);

/*
* draws a route along the given points following the given indices
* 
* @param img :			Pointer to the BLImage img
* @param points :		The points to draw
* @param indices :		The indices determining the order of points
* @param firstPoint:	First point, default 0
*/
void drawBestRoute(BLImage & img, std::vector<int> points, std::vector<int> indices, int firstPoint=0);

#endif // !__IMGFUNCS_H_INCLUDED__
