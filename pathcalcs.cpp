#include <iostream>
#include "pathcalcs.h"
#include <cmath>
#include <math.h>
#include <list>
#include <limits>

struct point {
	int xCoord;
	int yCoord;
	int pIndex;
};

double calc_dist(point pointA, point pointB) {
	
	double dist = sqrt(pow(pointA.xCoord - pointB.xCoord, 2) + pow(pointA.yCoord - pointB.yCoord, 2));
	return dist;
}

double calc_dist(int x0, int y0, int x1, int y1) {
	double dist = sqrt(pow(x0 - x1, 2) + pow(y0 - y1, 2));
	return dist;
}

void fillDistanceMat(std::deque<int> points, const int nPoints, double distanceMat[]) {

	for (int nIter = 0; nIter < nPoints; nIter++) {
		for (int mIter = 0; mIter < nPoints; mIter++) {
			// nIter and mIter iterate over nPoints, but two entries in deque points represent x and y coords of a point.
			// Therefore, nIter and mIter must be dublicated to access the correct values for each point
			distanceMat[nIter + nPoints * mIter] = calc_dist(points[2*nIter], points[2*nIter + 1], points[2*mIter], points[2*mIter + 1]);
		}
	}
}

/*
* Calculates the shortest distance between given points. May not be correct!
*/
double rekursiveRabbitHole(std::list<point> points, point startPoint, point endPoint, const int nPoints, double distanceMat[]) {
	
	std::list<point>::iterator pIter = points.begin();
	
	if (points.size() == 1) {
		point lastPoint = *pIter;
		double distance = distanceMat[lastPoint.pIndex + nPoints * startPoint.pIndex];
		distance += distanceMat[lastPoint.pIndex + nPoints * endPoint.pIndex];
		return distance;
	}

	// initialize current best distance as maximum best distance (lets hope for no overflow)
	double bestDistance = std::numeric_limits<double>::max();

	bool finished = false;

	for (int pInd = 0; pInd < points.size(); pInd++){

		// select new starting point and remove it from list
		point nextStartPoint = *pIter;
		points.erase(pIter);
		// pIter is now invalid (don't ask me why I don't get it yet


		// get distance from new starting point
		double distance = rekursiveRabbitHole(points, nextStartPoint, endPoint, nPoints, distanceMat);
		distance += distanceMat[startPoint.pIndex + nPoints * nextStartPoint.pIndex];
		
		// check if distance is new best distance
		if (distance < bestDistance) {
			bestDistance = distance;
		}
		// reset pIter
		pIter = points.begin();

		if (points.size() == pInd) {
			// can't move iterator further back, we insert the last element at the back
			points.push_back(nextStartPoint);
		}
		else {
			// move pIter to the correct position
			for (int iterInd = 0; iterInd < pInd; iterInd++) ++pIter;
			points.insert(pIter, nextStartPoint);
		}
	}

	return bestDistance;
}

/*
* Creates a deque containing all possible combinations of points, each stored as a deque
*/
std::deque<std::deque<int>> createDequeOfCombinations(std::list<point> points, const int currentPoints) {

	std::deque<std::deque<int>> resultDeque;

	std::list<point>::iterator pIter = points.begin();

	if (points.size() == 2) {
		point p0 = *pIter;
		pIter++;
		point p1 = *pIter;

		std::deque<int> indexDeque0, indexDeque1;
		
		indexDeque0.push_back(p0.pIndex);
		indexDeque0.push_back(p1.pIndex);
		indexDeque1.push_back(p1.pIndex);
		indexDeque1.push_back(p0.pIndex);

		resultDeque.push_back(indexDeque0);
		resultDeque.push_back(indexDeque1);

		// resultDeque[0][0] = p0.pIndex;
		// resultDeque[1][1] = p0.pIndex;
		// resultDeque[0][1] = p1.pIndex;
		// resultDeque[1][0] = p1.pIndex;
		
		return resultDeque;
	}
	
	// iterate over all points in list
	for (int pInd = 0; pInd < points.size(); pInd ++) {
		point nextPoint = *pIter;
		points.erase(pIter);
		// pIter breaks here

		std::deque<std::deque<int>> newIndDeque = createDequeOfCombinations(points, currentPoints-1);
		//for (int iDequeInd = 0; iDequeInd < newIndDeque.size(); iDequeInd++) {
		for (std::deque<std::deque<int>>::iterator iDequeIter = newIndDeque.begin(); iDequeIter != newIndDeque.end(); iDequeIter++){
			// iterate over the deque of deques
			
			// initialize iterator and move to correct position
			// std::deque<std::deque<int>>::iterator iDequeIter = newIndDeque.begin();
			// for (int iterInd = 0; iterInd < iDequeInd; iterInd++) iDequeIter++;
			std::deque<int> iDeque = *iDequeIter;
			// iDeque is one deque containing a combination of previously registered points

			/* Extremely unnecessary loop creates a lot of redundant combinations
			for (int jDequeInd = 0; jDequeInd < iDeque.size(); jDequeInd++) {
				// iterate over the deque of point indices

				// iterate over all possible positions where a new point could be inserted
				for (int posPos = 0; posPos < currentPoints; posPos++) {
					
					// create a copy of the deque, to keep iDeque unchanged
					std::deque<int> localCopy(iDeque);
					std::deque<int>::iterator indexInserter = localCopy.begin();
					// move iterator to new position
					for (int iteratorPos = 0; iteratorPos < posPos; iteratorPos++) indexInserter++;
					localCopy.insert(indexInserter, nextPoint.pIndex);
					resultDeque.push_back(localCopy);
				}
				// add point behind all other points as final option
				// std::deque<int> localCopy(iDeque);
				// localCopy.push_back(nextPoint.pIndex);


			}
			*/

			iDeque.push_front(nextPoint.pIndex);
			resultDeque.push_back(iDeque);
			// newIndDeque.pop_front();
		}
		newIndDeque.~deque();

		// recreate point Iterator
		pIter = points.begin();
		// add back erased point
		if (pInd == points.size()) {
			points.push_back(nextPoint);
		}
		else {
			for (int iterInd = 0; iterInd < pInd; iterInd++) pIter++;
			points.insert(pIter, nextPoint);
		}
	}

	return resultDeque;
}

std::deque<int> selectShortestRout(std::deque<std::deque<int>> possibleRoutes, int startPointInd, const int nPoints, double distanceMat[]) {
	double bestDistance, distance;
	int bestIndex, currentIndex;

	bestDistance = std::numeric_limits<double>::max();
	bestIndex = currentIndex = 0;

	// loop with iterator over all deques containing route indexes
	for (std::deque<std::deque<int>>::iterator dequeIter = possibleRoutes.begin(); dequeIter != possibleRoutes.end(); dequeIter++) {
		
		// init current deque of routes
		std::deque<int> currentDeque = *dequeIter;
		
		// loop over point indexes in deque
		// first point must be set before loop
		int thisPoint, nextPoint;
		std::deque<int>::iterator indexIter = currentDeque.begin();
		thisPoint = *indexIter;
		distance = distanceMat[startPointInd + nPoints * thisPoint];
		indexIter++;

		for (indexIter; indexIter != currentDeque.end(); indexIter++) {
			
			nextPoint = *indexIter;
			distance += distanceMat[thisPoint + nPoints * nextPoint];
			thisPoint = nextPoint;
		}

		// back to orign
		distance += distanceMat[nextPoint + nPoints * startPointInd];

		if (distance < bestDistance) {
			bestDistance = distance;
			bestIndex = currentIndex;
			std::cout << "New best distance: " << distance << " at index " << currentIndex << std::endl;
		}

		currentIndex++;
	}

	std::deque<std::deque<int>>::iterator dequeIter = possibleRoutes.begin();
	for (int iterInd = 0; iterInd < bestIndex; iterInd++) dequeIter++;


	std::cout << "Best distance is: " << bestDistance << std::endl;

	return *dequeIter;
}


std::deque<int> bruteforce(std::deque<int> points, const int nPoints, double distanceMat[]) {
	
	// register all points as point struct in point list
	std::list<point> pList;
	for (int pInd = 0; pInd < nPoints; pInd++) {
		point newPoint;
		newPoint.xCoord = points[2 * pInd];
		newPoint.yCoord = points[2 * pInd + 1];
		newPoint.pIndex = pInd;
		pList.push_back(newPoint);
	}

	// std::list<point>::iterator pListIterator = pList.begin();
	// pListIterator++;

	point firstPoint = *pList.begin();
	pList.erase(pList.begin());

	double distance = rekursiveRabbitHole(pList, firstPoint, firstPoint, nPoints, distanceMat);
	std::cout << "shortest distance could be: " << distance << std::endl;


	std::deque<std::deque<int>> dequeOfIndices;
	dequeOfIndices = createDequeOfCombinations(pList, nPoints - 1);
	std::cout << "created all possible combinations" << std::endl;

	std::deque<int> bestDeque = selectShortestRout(dequeOfIndices, firstPoint.pIndex, nPoints, distanceMat);
	bestDeque.push_front(firstPoint.pIndex);

	dequeOfIndices.~deque();

	return bestDeque;
}