#pragma once
#ifndef __CUDAPATH_H_INCLUDED__
#define __CUDAPAHT_H_INCLUDED__
// #include <cuda_runtime.h>
#include "Traveler.h"
#include <unordered_set>

class CudaPath : public Traveler {
public:
	CudaPath(std::vector<int>points);

	void runParticles();

	void softMax(std::vector<double>& distances);

	int chooseNext(std::vector<double>& softDistances, double & rand);
};


#endif // !__CUDAPATH_H_INCLUDED__