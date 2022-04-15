#include "NoCross.h"
#include <math.h>
#include <iostream>
#include <algorithm>
#include <limits>
#include <time.h>
#include <exception>
#include <utility>
#include <unordered_set>
#include <set>
#include <sstream>
#include <unordered_map>
#include <stack>


// empty point
Point::Point() {
	this->x = 0;
	this->y = 0;
	this->index = -1;
}
	
Point::Point(int x, int y, int index) {
	this->x = x;
	this->y = y;
	this->index = index;
}

// Thanks to stackexchange user @cdahms for pointing out how this works

// operators for sets and unordered sets
bool Point::operator==(const Point& pointToCompare) const {
	return this->index == pointToCompare.index;
}

bool Point::operator<(const Point& pointToCompare) const {
	return this->index < pointToCompare.index;
}

	// hash function for maps etc
struct Point::HashFunction
{
	size_t operator()(const Point& point) const {
		size_t hash = std::hash<int>()(point.index);
		return hash;
	}
};




Edge::Edge(Point p1, Point p2) {
	this->p1 = p1;
	this->p2 = p2;
}

std::string Edge::toString() {

	std::ostringstream ss;
	ss << p1.index;
	ss << "->";
	ss << p2.index;


	return ss.str();
}

bool Edge::operator==(const Edge& edge) const {
	return (this->p1 == edge.p1 && this->p2 == edge.p2) || (this->p1 == edge.p2 && this->p2 == edge.p1);
}

bool Edge::operator<(const Edge& edge) const {
	return this->p1 < edge.p1;
}

bool Edge::intersects(const Edge& edge) {

		// check that points don't match
		if (this->p1.index == edge.p1.index || this->p1.index == edge.p2.index || this->p2.index == edge.p1.index || this->p2.index == edge.p2.index) return false;

		// check lines are not parallel

		double check = (this->p1.x - this->p2.x) * (edge.p1.x - edge.p2.x) + (this->p1.y - this->p2.y) * (edge.p1.y - edge.p2.y);
		if (std::abs(check) < 0.001) return false;


		/*
		* Crossing of two straight lines is solution of the two equations
		* A_x + r*(B_x - A_x) = C_x + s*(D_x - C_x)
		* A_y + r*(B_y - A_y) = C_y + s*(D_y - C_y)
		* 
		* where A and B are the points connecting line 1 (this->p1 and this->p2)
		* and C and D are the points connecting line 2 (edge.p1 and edge.p2)
		* 
		* below is the above equation resolved for r = c1/c2
		* if 0 < r < 1 and 0 < s < 1, the lines cross within the region of interest
		*/

		double c1 = (((double)this->p1.x - (double)edge.p1.x) / ((double)edge.p2.x - (double)edge.p1.x) - ((double)this->p1.y - (double)edge.p1.y) / ((double)edge.p2.y - (double)edge.p1.y));
		double c2 = (((double)this->p2.y - (double)this->p1.y) / ((double)edge.p2.y - (double)edge.p1.y) - ((double)this->p2.x - (double)this->p1.x) / ((double)edge.p2.x - (double)edge.p1.x));

		double r = c1 / c2;

		double s = ((double)this->p1.y - (double)edge.p1.y + r * ((double)this->p2.y - (double)this->p1.y)) / ((double)edge.p2.y - (double)edge.p1.y);
		
		if ((s > 0 && s < 1) && (r > 0 && r < 1)) {
			return true;
		}
		else return false;
	}


void Edge::reverse() {

	Point tmp = p1;
	p1 = p2;
	p2 = tmp;
}

struct Edge::HashFunction
{
	size_t operator()(const Edge& edge) const {
		size_t hash1 = std::hash<int>()(edge.p1.index);
		size_t hash2 = std::hash<int>()(edge.p2.index);
		return hash1 ^ hash2;
	}
};



NoCross::NoCross(std::vector<int> inputPoints) : Traveler(inputPoints) {

}

void NoCross::shortestPathIndizes(std::vector<double> & distMat, std::unordered_map<int, Edge>& indexToEdge) {

	// starting at point index 0, later set to last pushed into indices
	int indexLastPushed = 0;

	std::set<int> indexes_visited;
	// std::unordered_map<int, Edge> indexToEdge;

	for (int i = 1; i < nPoints; i++) {

		double bestDistanceToNext = std::numeric_limits<double>::max();
		int currentBestNextIndex;

		for (int j = 1; j < nPoints; j++) {
			// check if j is not already a point in bestPathIndizes
			// if (std::find(bestPathIndizes.begin(), bestPathIndizes.end(), j) == bestPathIndizes.end()) { // linear(size) complexity
			if (indexes_visited.find(j) == indexes_visited.end()) { // log(size) complexity
				double distance = distMat[indexLastPushed * nPoints + j];

				if (distance < bestDistanceToNext) {
					bestDistanceToNext = distance;
					currentBestNextIndex = j;
				}
			}
		}

		indexes_visited.insert(currentBestNextIndex);

		Point p1 = Point(points[indexLastPushed * 2], points[indexLastPushed * 2 + 1], indexLastPushed);
		Point p2 = Point(points[currentBestNextIndex * 2], points[currentBestNextIndex * 2 + 1], currentBestNextIndex);

		indexToEdge.emplace(indexLastPushed, Edge(p1, p2));

		bestPathIndizes.push_back(currentBestNextIndex);
		indexLastPushed = currentBestNextIndex;
		bestDist += bestDistanceToNext;
	}
	
	Point p1 = Point(points[indexLastPushed * 2], points[indexLastPushed * 2 + 1], indexLastPushed);
	Point p2 = Point(points[0], points[1], 0);

	indexToEdge.emplace(indexLastPushed, Edge(p1, p2));


	bestDist += distMat[indexLastPushed]; // distMat[startPoint*nPoints + indexLastPushed] == distMat[0+indexLastPushed] == distMat[indexLastPushed]
}

void NoCross::iterateOverPath(std::unordered_map<int, Edge>& indexToEdge) {

	bool crossing;
	do {
		crossing = reorderedCrossingEdges(indexToEdge);
	} while (crossing);

}

bool NoCross::reorderedCrossingEdges(std::unordered_map<int, Edge>& indexToEdge) {

	/* FIFO container of edges to reverse */
	std::stack<Edge> edgesToReverse;

	std::unordered_map<int, Edge>::iterator iter0;
	std::unordered_map<int, Edge>::iterator iter1;

	for (iter0 = indexToEdge.begin(); iter0 != indexToEdge.end(); iter0++) {
		for (iter1 = indexToEdge.begin(); iter1 != indexToEdge.end(); iter1++) {
			if (iter0 != iter1) {
				Edge edgeAB = iter0->second;
				Edge edgeCD = iter1->second;

				if (edgeAB.intersects(edgeCD)) {
					// iterate over edges between intersecting edges and reverse pointing order

					int indexStart = edgeAB.p1.index;
					int indexEnd = edgeCD.p2.index;

					int indexOfEdgeToReverse = edgeAB.p2.index;
					int indexOfEdgeToReach = edgeCD.p1.index;

					// reconnect first edge

					// temporary storage of edgeAB.p2
					Point pointToReconnect = edgeAB.p2;

					edgeAB.p2 = edgeCD.p1;
					indexToEdge.erase(edgeAB.p1.index);
					indexToEdge.emplace(edgeAB.p1.index, edgeAB);

					indexToEdge.erase(edgeCD.p1.index);

					// Find all edges to reverse, remove from index map and add to stack

					while (indexOfEdgeToReverse != indexOfEdgeToReach) {

						Edge nextEdge = indexToEdge.at(indexOfEdgeToReverse);
						edgesToReverse.push(nextEdge);

						indexToEdge.erase(indexOfEdgeToReverse);

						indexOfEdgeToReverse = nextEdge.p2.index;
					}

					// Reverse Edges on stack an insert back into index map
					while (!edgesToReverse.empty()) {
						Edge newEdge = edgesToReverse.top();
						newEdge.reverse();
						indexToEdge.emplace(newEdge.p1.index, newEdge);
						edgesToReverse.pop();
					}

					edgeCD.p1 = pointToReconnect;
					indexToEdge.emplace(edgeCD.p1.index, edgeCD);

					// detected cross
					return true;
				}
			}
		}
	}

	return false;
}

void NoCross::edgeMapToIndexVector(std::vector<double> & distMat, std::unordered_map<int, Edge> & indexToEdge) {
	
	bestPathIndizes.clear();
	bestDist = 0;


	int indexPrev = indexToEdge.at(0).p2.index;

	bestPathIndizes.push_back(indexPrev);
	bestDist = distMat[indexPrev];

	bool loopAgain = true;

	do {
		Edge nextEdge = indexToEdge.at(indexPrev);
		int indexNext = nextEdge.p2.index;

		bestDist += distMat[indexNext * nPoints + indexPrev];

		if (indexNext != 0) bestPathIndizes.push_back(indexNext);
		else loopAgain = false;
		indexPrev = indexNext;

	} while (loopAgain);


}

void NoCross::NextPointNoCross(){

	if (searched) {
		throw std::runtime_error("Reset before running new permutation run");
	}

	// init timing
	time_t start;
	time(&start);

	/* Distance Matrix */
	std::vector<double> distMat = createDistMat(points);

	/* mapping index to connecting edge */
	std::unordered_map<int, Edge> indexToEdge;

	// Basically what Shortest.takeClosestPoint does
	shortestPathIndizes(distMat, indexToEdge);

	// std::unordered_set < Point, Point::HashFunction > pointsVisited;
	
	iterateOverPath(indexToEdge);

	edgeMapToIndexVector(distMat, indexToEdge);


	searched = true;
	// best path is total path time
	timeToFinish = difftime(time(NULL), start);
	timeToFindBest = timeToFinish;
}