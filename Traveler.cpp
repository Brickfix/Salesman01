#include "Traveler.h"
#include <math.h>
#include <iostream>
#include <algorithm>
#include <limits>
#include <time.h>

double calcDist(int x0, int y0, int x1, int y1) {
	double dist = sqrt(pow(x0 - x1, 2) + pow(y0 - y1, 2));
	return dist;
}

Traveler::Traveler(std::vector<int> points) {

	if (nPoints % 2 != 0) {
		throw "points must contain an even number of entries!";
	}
	
	// init private values
	nPoints = points.size() / 2;
	distMat.resize(nPoints * nPoints);

	createDistMat(points);

}

void Traveler::createDistMat(std::vector<int> points) {

	for (int pInd0 = 0; pInd0 < nPoints; pInd0++) {
		for (int pInd1 = nPoints - 1; pInd1 > pInd0; pInd1--) {
			// Calculate once, write twice
			double distance = calcDist(points[2 * pInd0], points[2 * pInd0 + 1], points[2 * pInd1], points[2 * pInd1 + 1]);
			distMat[nPoints * pInd0 + pInd1] = distance;
			distMat[nPoints * pInd1 + pInd0] = distance;
		}
	}
}

std::pair<std::vector<int>, double> Traveler::iterateThroughPoints() {
	
	std::vector<int> indexes;
	std::vector<int> bestIndexes;
	double bestDist = std::numeric_limits<double>::max();

	// int nPoints = points.size()/2;

	// starting point is always fixed at index 0
	for (int i = 1; i < nPoints; i++) {
		indexes.push_back(i);
	}

	time_t start;
	time(&start);

	do {
		int previousPointIndex = 0;
		double currentDist = 0;

		for (std::vector<int>::iterator indexIter = indexes.begin(); indexIter != indexes.end(); ++indexIter) {
			
			currentDist += distMat[previousPointIndex*nPoints + *indexIter];
			
			// update previous point
			previousPointIndex = *indexIter;
		}

		// distance back to last point
		currentDist += distMat[previousPointIndex * nPoints + 0];

		if (currentDist < bestDist) {
			bestIndexes.assign(indexes.begin(), indexes.end());
			bestDist = currentDist;
			std::cout << difftime(time(NULL), start) <<"s : New Best dinstance: " << bestDist << std::endl;
		}

	} while (std::next_permutation(indexes.begin(), indexes.end()));

	// clean up
	indexes.~vector();

	return std::pair<std::vector<int>, double> (bestIndexes,bestDist);
}