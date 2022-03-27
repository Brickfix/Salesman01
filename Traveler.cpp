#include "Traveler.h"
#include <math.h>
#include <iostream>
#include <algorithm>
#include <limits>
#include <time.h>
#include <exception>


Traveler::Traveler(std::vector<int> inputPoints) {

	int inputSize = inputPoints.size();

	// Handling exceptions
	if (inputSize == 0) {

		throw std::invalid_argument( "points may not be empty" );
	}
	else if (inputSize % 2 != 0) {
		throw std::invalid_argument(" points must contain an even number of entries, one for x and one for y coordinate" );
	}
	else if (inputSize <= 6) {
		throw std::invalid_argument(" there exists no fastest combination for three or fewer points" );
	}
	
	// init private values
	nPoints = inputSize / 2;
	points = inputPoints;

	// set results to defaults
	reset();
}

void Traveler::reset() {
	bestDist = std::numeric_limits<double>::max();
	timeToFinish = -1;
	timeToFindBest = -1;
	bestPathIndizes.clear();
	searched = false;
}

double Traveler::calcDist(int x0, int y0, int x1, int y1) {
	double dist = sqrt(pow(x0 - x1, 2) + pow(y0 - y1, 2));
	return dist;
}


std::vector<double> Traveler::createDistMat(std::vector<int> points) {

	std::vector<double> distMat;
	distMat.resize(nPoints * nPoints);

	for (int pInd0 = 0; pInd0 < nPoints; pInd0++) {
		for (int pInd1 = nPoints - 1; pInd1 > pInd0; pInd1--) {
			// Calculate once, write twice
			double distance = calcDist(points[2 * pInd0], points[2 * pInd0 + 1], points[2 * pInd1], points[2 * pInd1 + 1]);
			distMat[nPoints * pInd0 + pInd1] = distance;
			distMat[nPoints * pInd1 + pInd0] = distance;
		}
	}

	return distMat;
}

void Traveler::iterateThroughPoints() {

	if (searched) {
		throw std::runtime_error("Reset before running new permutation run");
	}
	
	std::vector<int> indexes;
	std::vector<int> bestIndexes;
	
	// init timing
	time_t start;
	time(&start);

	std::vector<double> distMat = createDistMat(points);

	// starting point is always fixed at index 0
	for (int i = 1; i < nPoints; i++) {
		indexes.push_back(i);
	}

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
			bestPathIndizes.assign(indexes.begin(), indexes.end());
			bestDist = currentDist;

			timeToFindBest = difftime(time(NULL), start);

			// this costs a lot of time
			// std::cout << timeToFindBest <<"s : New Best dinstance: " << bestDist << std::endl;
		}

	} while (std::next_permutation(indexes.begin(), indexes.end()));

	searched = true;
	timeToFinish = difftime(time(NULL), start);
	
}

void Traveler::permutateHalf() {

	if (searched) {
		throw std::runtime_error("Reset before running new permutation run");
	}


	std::vector<int> indexes;
	std::vector<int> bestIndexes;

	// init timing
	time_t start;
	time(&start);

	std::vector<double> distMat = createDistMat(points);

	// starting point is always fixed at index 0
	for (int i = 1; i < nPoints; i++) {
		indexes.push_back(i);
	}

	do {
		int previousPointIndex = 0;
		double currentDist = 0;
		
		if (indexes.back() > indexes.front()) {
			for (std::vector<int>::iterator indexIter = indexes.begin(); indexIter != indexes.end(); ++indexIter) {

				currentDist += distMat[previousPointIndex * nPoints + *indexIter];

				// update previous point
				previousPointIndex = *indexIter;
			}

			// distance back to last point
			currentDist += distMat[previousPointIndex * nPoints + 0];

			if (currentDist < bestDist) {
				bestPathIndizes.assign(indexes.begin(), indexes.end());
				bestDist = currentDist;

				timeToFindBest = difftime(time(NULL), start);

				// std::cout << difftime(time(NULL), start) <<"s : New Best dinstance: " << bestDist << std::endl;
			}
		}

	} while (std::next_permutation(indexes.begin(), indexes.end()));

	searched = true;
	timeToFinish = difftime(time(NULL), start);
}

void Traveler::takeClosestPoint() {
	if (searched) {
		throw std::runtime_error("Reset before running new permutation run");
	}

	std::vector<int> indices;

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
			if (std::find(indices.begin(), indices.end(), j) != indices.end()) {
				j++;
			}
			double distance = distMat[indexLastPushed * nPoints + j];

			if (distance < bestDistanceToNext) {
				bestDistanceToNext = distance;
				currentBestNextIndex = j;
			}
		}

		indices.push_back(currentBestNextIndex);
		indexLastPushed = currentBestNextIndex;
		bestDist += bestDistanceToNext;
	}

	bestDist += distMat[indexLastPushed]; // distMat[startPoint*nPoints + indexLastPushed] == distMat[0+indexLastPushed] == distMat[indexLastPushed]
	
	// best path is total path time
	timeToFinish = difftime(time(NULL), start);
	timeToFindBest = timeToFinish;
}