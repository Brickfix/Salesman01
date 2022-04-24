#include "CudaPath.cuh"
#include <time.h>
#include <stdexcept>
#include <math.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <limits>

CudaPath::CudaPath(std::vector<int> inputPoints) : Traveler(inputPoints){}

__global__
void cudaCopyNormDists(float* distMat, float* distVec, int * setIndizes,  int index, const int nPoints) {

	for (int i = 0; i < nPoints; i++) {
		distVec[i] = std::exp(distMat[index * nPoints + i] / -100);
	}

	for (int i = 0; i < nPoints; i++) {
		distVec[setIndizes[i]] = 0.0;
	}
}


__global__
void cudaSoftMax(float * distances, int distSize) {
	
	float total = 0;

	// sum up all distances
	for (int i = 0; i < distSize; i++) {
		total += distances[i];
	}

	// devide by total sum
	for (int i = 0; i < distSize; i++) {
		distances[i] = distances[i] / total;
	}
}

__global__
void cudaChooseNext(float* distances, float rand, int * index) {
	
	double cumTotal = 0;
	// index[0] = -1;

	do {
		index[0]++;
		cumTotal += distances[index[0]];
	} while (cumTotal < rand);
}

__global__
void particleGoBrrt(float* distMat, float* distances, int* setIndizes, float* rolls, float* totalDistances, const int nPoints) {
	
	int startIndex = 0;
	int threadBlockIdx = blockIdx.x * blockDim.x + threadIdx.x;

	totalDistances[threadBlockIdx] = 0.0;

	for (int i = 1; i < nPoints; i++) {

		// fill distances from startIndex Point with distances to other points
		for (int j = 0; j < nPoints; j++) {
			distances[threadBlockIdx*nPoints + j] = std::exp(distMat[startIndex * j + nPoints] / -100);
		}

		// set already used points to 0
		for (int j = 0; j < nPoints; j++) {
			distances[threadBlockIdx*nPoints + setIndizes[threadBlockIdx * nPoints + j]] = 0.0;
		}

		// calculate cumulative value
		float total = 0.0;
		for (int j = 0; j < nPoints; j++) {
			total += distances[threadBlockIdx * nPoints + j];
		}

		// devide each entry by total sum
		for (int j = 0; j < nPoints; j++) {
			distances[threadBlockIdx * nPoints + j] = distances[threadBlockIdx * nPoints + j] / total;
		}

		// select random
		float cumTotal = 0.0;
		int nextIndex = 0;
		do {
			nextIndex++;
			cumTotal += distances[threadBlockIdx * nPoints + nextIndex];
		} while ((cumTotal < rolls[threadBlockIdx * nPoints + i]) && (nextIndex < nPoints-1));

		// add distance to total distance
		totalDistances[threadBlockIdx] += distMat[startIndex * nPoints + nextIndex];

		// handle indizes
		startIndex = nextIndex;
		setIndizes[threadBlockIdx * nPoints + i] = nextIndex;
	}

	// path back to 0
	totalDistances[threadBlockIdx] += distMat[startIndex * nPoints];

}

__global__
void addTwo(int * x, int * y, int * z) {

	int i = blockIdx.x * blockDim.x + threadIdx.x;
	z[i] = x[i] + y[i];
}

void testBasicStuff() {

	int N = 1 << 10;
	int* x, * y, * z, * d_x, * d_y, * d_z;

	x = (int*)malloc(N * sizeof(int));
	y = (int*)malloc(N * sizeof(int));
	z = (int*)malloc(N * sizeof(int));

	for (int i = 0; i < N; i++) {
		x[i] = 1;
		y[i] = 2;
	}

	// allocate memory

	cudaMalloc(&d_x, N * sizeof(int));
	cudaMalloc(&d_y, N * sizeof(int));
	cudaMalloc(&d_z, N * sizeof(int));

	cudaMemcpy(d_x, x, N * sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(d_y, y, N * sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(d_z, z, N * sizeof(int), cudaMemcpyHostToDevice);

	addTwo << <(N + 255 / 256), 256 >> > (d_x, d_y, d_z);

	cudaMemcpy(z, d_z, N * sizeof(int), cudaMemcpyDeviceToHost);

	double maxError = 0.0;
	for (int i = 0; i < N; i++) {
		maxError = std::max(maxError, std::abs((double)z[i] - 3.0));
	}

	// free memory
	cudaFree(d_x);
	cudaFree(d_y);
	cudaFree(d_z);

	free(x);
	free(y);
	free(z);
}

void CudaPath::runOneParticle(
		const std::vector<double>& distMat,
		std::default_random_engine & generator,
		std::vector<int> & travelIndizes,
		double & travelDist) {

	std::unordered_set<int> chosenIndizes;
	// start search from point 0
	int currentIndex = 0;
	chosenIndizes.insert(currentIndex);

	// random number init
	// std::default_random_engine generator;
	std::uniform_real_distribution<double> dist(0.0, 1.0);

	travelDist = 0;

	// allocate distance Mat on graphics card
	float * distMatSimple, * d_distMatSimple;
	distMatSimple = (float*)malloc(nPoints * nPoints * sizeof(float));
	cudaMalloc(&d_distMatSimple, nPoints * nPoints * sizeof(float));

	// copy distMat into distMatSimple
	for (int i = 0; i < nPoints * nPoints; i++) {
		distMatSimple[i] = (float)distMat[i];
	}

	// copy to graphics card
	cudaMemcpy(d_distMatSimple, distMatSimple, nPoints * nPoints * sizeof(float), cudaMemcpyHostToDevice);

	// initialize indizes already set
	int* setIndizes, * d_setIndizes;
	setIndizes = (int*)malloc(nPoints * sizeof(int));
	cudaMalloc(&d_setIndizes, nPoints * sizeof(int));

	for (int i = 0; i < nPoints; i++) {
		setIndizes[i] = 0;
	}

	cudaMemcpy(d_setIndizes, setIndizes, nPoints * sizeof(int), cudaMemcpyHostToDevice);

	for (int i = 1; i < nPoints; i++) {
		// init distances, set entries already chosen to 0

		// init distances, allocate memory space
		float* distsFromPoint, * d_dists;
		distsFromPoint = (float*)malloc(this->nPoints * sizeof(float));
		cudaMalloc(&d_dists, nPoints * sizeof(float));

		cudaCopyNormDists<<<1,1>>>(d_distMatSimple, d_dists, d_setIndizes, i, nPoints);
		
		cudaMemcpy(distsFromPoint, d_dists, nPoints * sizeof(float), cudaMemcpyDeviceToHost);

		cudaSoftMax<<<1,1>>>(d_dists, nPoints);

		cudaMemcpy(distsFromPoint, d_dists, nPoints * sizeof(float), cudaMemcpyDeviceToHost);

		float roll = dist(generator);

		int * index, * d_index;

		index = (int*)malloc(sizeof(int));
		index[0] = -1;
		cudaMalloc(&d_index, sizeof(int));
		cudaMemcpy(d_index, index, sizeof(int), cudaMemcpyHostToDevice);
		
		cudaChooseNext<<<1,1>>>(d_dists, roll, d_index);

		cudaMemcpy(index, d_index, sizeof(int), cudaMemcpyDeviceToHost);

		travelDist += distMat[currentIndex * nPoints + index[0]];
		
		chosenIndizes.insert(index[0]);
		travelIndizes.push_back(index[0]);
		setIndizes[i] = index[0];
		cudaMemcpy(d_setIndizes, setIndizes, nPoints * sizeof(int), cudaMemcpyHostToDevice);


		currentIndex = index[0];

		// Free memory
		cudaFree(d_index);
		cudaFree(d_dists);

		free(distsFromPoint);
		free(index);
	}

	travelDist += distMat[currentIndex];


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

	// parameters for number of threads
	int threadBlocks = 3000;
	int threads = 256;
	int nParticles = threadBlocks * threads;

	// allocate memory and init distance matrix
	float* distMatSimple, * d_distMatSimple;
	distMatSimple = (float*)malloc(nPoints * nPoints * sizeof(float));
	cudaMalloc(&d_distMatSimple, nPoints * nPoints * sizeof(float));

	// copy distMat into distMatSimple TODO if time rewrite for float
	for (int i = 0; i < nPoints * nPoints; i++) {
		distMatSimple[i] = (float)distMat[i];
	}

	// init array containing distances from current point
	float* distances, * d_distances;
	distances = (float*)malloc(nPoints * nParticles * sizeof(float));
	cudaMalloc(&d_distances, nPoints * nParticles * sizeof(float));

	// init random rolls
	std::default_random_engine generator;
	std::uniform_real_distribution<double> dist(0.0, 1.0);

	float* rolls, * d_rolls;
	rolls = (float*)malloc(nParticles * nPoints * sizeof(float));
	cudaMalloc(&d_rolls, nParticles * nPoints * sizeof(float));

	for (int i = 0; i < nPoints * nParticles; i++) {
		rolls[i] = dist(generator);
	}

	// array containing set indizes per particle

	int* setIndizes, * d_setIndizes;
	setIndizes = (int*)malloc(nPoints * nParticles * sizeof(int));
	cudaMalloc(&d_setIndizes, nPoints * nParticles * sizeof(int));

	for (int i = 0; i < nPoints * nParticles; i++) {
		setIndizes[i] = 0;
	}

	// array containing total travel distance
	float* totalDistances, * d_totalDistances;
	totalDistances = (float*)malloc(nParticles * sizeof(float));
	cudaMalloc(&d_totalDistances, nParticles * sizeof(float));

	// copy from host to device
	cudaMemcpy(d_distMatSimple, distMatSimple, nPoints * nPoints * sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(d_rolls, rolls, nParticles * nPoints * sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(d_setIndizes, setIndizes, nParticles * nPoints * sizeof(int), cudaMemcpyHostToDevice);
	


	particleGoBrrt << <threadBlocks, threads>> > (d_distMatSimple, d_distances, d_setIndizes, d_rolls, d_totalDistances, nPoints);

	// copy back results
	cudaMemcpy(setIndizes, d_setIndizes, nPoints * nParticles * sizeof(int), cudaMemcpyDeviceToHost);
	cudaMemcpy(totalDistances, d_totalDistances, nParticles * sizeof(float), cudaMemcpyDeviceToHost);

	// for debug purpose
	cudaMemcpy(distances, d_distances, nPoints * nParticles * sizeof(int), cudaMemcpyDeviceToHost);

	// find shortest total distance travelled by a particle
	int bestParticle;
	float bestDistance = std::numeric_limits<float>::max();
	
	// iter defined out of loop for debugging
	int iter;
	for (iter = 0; iter < nParticles; iter++) {
		if (bestDistance > totalDistances[iter]) {
			bestDistance = totalDistances[iter];
			bestParticle = iter;
			timeToFindBest = difftime(time(NULL), start);
		}
	}

	this->bestDist = (double)bestDistance;

	// copy indizes in result field

	for (int i = 1; i < nPoints; i++) {
		this->bestPathIndizes.push_back(setIndizes[bestParticle * nPoints + i]);
	}

	// free memory
	free(setIndizes);
	free(rolls);
	free(distMatSimple);
	free(distances);

	cudaFree(d_setIndizes);
	cudaFree(d_rolls);
	cudaFree(d_distMatSimple);
	cudaFree(d_distances);

	searched = true;
	timeToFinish = difftime(time(NULL), start);
}