#include "CudaPath.cuh"
#include "random"
#include <time.h>
#include <stdexcept>

CudaPath::CudaPath(std::vector<int> inputPoints) : Traveler(inputPoints){}

void CudaPath::softMax(std::vector<double>& distances) {

	double total = 0;
	std::vector<double>::iterator distIter;

	// sum up all distances
	for (distIter = distances.begin(); distIter != distances.end(); distIter++) {
		total += *distIter;
	}

	// devide by total sum
	for (int i = 0; i < distances.size(); i++) {
		distances[i] = distances[i] / total;
	}
}

int CudaPath::chooseNext(std::vector<double> & softIndizes, double & rand) {
	
	double cumTotal = 0;
	int index = 0;

	do {
		index++;
		cumTotal += softIndizes[index];
	} while (cumTotal < rand);

	return index;

}

void CudaPath::runParticles() {

	if (searched) {
		throw std::runtime_error("Reset before running new permutation run");
	}

	// init timing
	time_t start;
	time(&start);

	// create the distance matrix
	std::vector<double> distMat = this->createDistMat(this->points);

	std::unordered_set<int> chosenIndizes;

	// start search from point 0
	int currentIndex = 0;
	chosenIndizes.insert(currentIndex);

	// random number init
	std::default_random_engine generator;
	std::uniform_real_distribution<double> dist(0.0, 1.0);

	bestDist = 0;


	for (int i = 1; i < nPoints; i++) {
		// init distances, set entries already chosen to 0
		std::vector<double> distsFromPoint(nPoints, 0);
		for (int j = 1; j < nPoints; j++) {
			if (chosenIndizes.find(j) == chosenIndizes.end()) {
				// j not in set
				distsFromPoint[j] = distMat[currentIndex * nPoints + j];
			}
		}

		softMax(distsFromPoint);

		double roll = dist(generator);

		int nextIndex = chooseNext(distsFromPoint, roll);

		bestDist += distMat[currentIndex * nPoints + nextIndex];
		chosenIndizes.insert(nextIndex);
		bestPathIndizes.push_back(nextIndex);


		currentIndex = nextIndex;
	}

	bestDist += distMat[currentIndex];

	searched = true;
	timeToFinish = difftime(time(NULL), start);
	timeToFindBest = timeToFinish;
}