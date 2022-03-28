#include "Shortest.h"
#include <math.h>
#include <iostream>
#include <algorithm>
#include <limits>
#include <time.h>
#include <exception>
#include <utility>

Shortest::Shortest(std::vector<int> inputPoints) : Traveler(inputPoints) {

}

void Shortest::takeClosestPoint() {
	if (searched) {
		throw std::runtime_error("Reset before running new permutation run");
	}

	// init timing
	time_t start;
	time(&start);

	std::vector<double> distMat = createDistMat(points);

	// starting at point index 0, later set to last pushed into indices
	int indexLastPushed = 0;
	bestDist = 0;

	for (int i = 1; i < nPoints; i++) {

		double bestDistanceToNext = std::numeric_limits<double>::max();
		int currentBestNextIndex;

		for (int j = 1; j < nPoints; j++) {
			// check if j is not already a point in bestPathIndizes
			if (std::find(bestPathIndizes.begin(), bestPathIndizes.end(), j) == bestPathIndizes.end()) {
				double distance = distMat[indexLastPushed * nPoints + j];

				if (distance < bestDistanceToNext) {
					bestDistanceToNext = distance;
					currentBestNextIndex = j;
				}
			}
		}

		bestPathIndizes.push_back(currentBestNextIndex);
		indexLastPushed = currentBestNextIndex;
		bestDist += bestDistanceToNext;
	}

	bestDist += distMat[indexLastPushed]; // distMat[startPoint*nPoints + indexLastPushed] == distMat[0+indexLastPushed] == distMat[indexLastPushed]


	searched = true;
	// best path is total path time
	timeToFinish = difftime(time(NULL), start);
	timeToFindBest = timeToFinish;