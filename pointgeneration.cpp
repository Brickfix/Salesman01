#include "pointgeneration.h"

#include <random>
#include <cmath>

std::vector<int> createPoints(
                                int nPoints,
                                std::vector<int> & points,
                                int const imgPixels,
                                int const seed,
                                int const padding,
                                int const minPointDist) {

    // Range of allowed coordinates
    int pointRange = imgPixels - 2 * padding;

    std::default_random_engine randy;
    randy.seed(seed);

    // coordinate generator
    std::uniform_int_distribution<int> dist(0, pointRange);

    // first point needs no check, must be created beforehand to prevent deadlock
    if(points.size()==0){
        points.push_back(padding + dist(randy));
        points.push_back(padding + dist(randy));
        nPoints--;
    }

    // Debug counter
    // int totalGeneratedPointCounter = 1;

    // Following generation loop gets increasingly costly at high point numbers, but is fine for now at as total desired points is rather low
    for (int p = 0; p < nPoints; ) {

        // must be true to store new point
        bool isValidPoint = true;

        // roll x_coord
        int newX = padding + dist(randy);

        // roll y_coord
        int newY = padding + dist(randy);

        // Increase debug counter
        // totalGeneratedPointCounter++;

        // check if point is allowed
        for (int iter = 0; iter < points.size(); iter += 2) {

            // check if newX and newY is not within minimum allowed distance
            if (!(std::abs(points[iter] - newX) > minPointDist) && !(std::abs(points[iter + 1] - newY) > minPointDist)) {
                isValidPoint = false;
                break;
            }
        }

        // add point to deque
        if (isValidPoint) {
            points.push_back(newX);
            points.push_back(newY);
            p++;
        }
    }

    // Debug output
    // std::cout << "Generated a total of " << totalGeneratedPointCounter << " Points" << std::endl;

    return points;

}