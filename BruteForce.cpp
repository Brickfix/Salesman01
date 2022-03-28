#include "BruteForce.h"
#include <math.h>
#include <iostream>
#include <algorithm>
#include <limits>
#include <time.h>
#include <exception>


BruteForce::BruteForce(std::vector<int> inputPoints) : Traveler(inputPoints) {
	
}

void BruteForce::iterateThroughPoints() {

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

			// this costs a lot of time
			// std::cout << timeToFindBest <<"s : New Best dinstance: " << bestDist << std::endl;
		}

	} while (std::next_permutation(indexes.begin(), indexes.end()));

	searched = true;
	timeToFinish = difftime(time(NULL), start);

}

void BruteForce::permutateHalf() {

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