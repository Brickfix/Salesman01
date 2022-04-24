#pragma once
#ifndef __CUDAPATH_H_INCLUDED__
#define __CUDAPAHT_H_INCLUDED__
// #include <cuda_runtime.h>
#include "Traveler.h"
#include <unordered_set>
#include <random>


/**
* Class CudaPath finds a Path based on multiple particles selecting the next node randomly depending on distance
*/
class CudaPath : public Traveler {
public:
	/**
	 * @brief Construct a new Cuda Path object
	 * 
	 */
	CudaPath(std::vector<int>points);

	/**
	* Initialize Particles and collect results
	*/
	void runParticles();


	/**
	* Perform semi-random search for path with one particle
	* 
	* @param distMat
	* @param generator
	* @param travelIndizes
	* @param travelDist
	*/
	void runOneParticle(const std::vector<double>& distMat, std::default_random_engine& generator, std::vector<int>& travelIndizes, double& travelDist);

};


#endif // !__CUDAPATH_H_INCLUDED__