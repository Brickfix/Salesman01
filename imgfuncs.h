#pragma once
// import guard
#ifndef __IMGFUNCS_H_INCLUDED__
#define __IMGFUNCS_H_INCLUDED__

#include <blend2d.h>
#include <deque>


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
BLImage createImage(std::deque<int> points, int length, int pointRadius);

BLImage drawBestRoute(BLImage img, std::deque<int> points, std::deque<int> indexes);

#endif // !__IMGFUNCS_H_INCLUDED__
