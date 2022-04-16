#include "pch.h"
#include "../NoCross.h"
#include <vector>
#include <limits>

/* Checks that the overall output is correct */
TEST(NoCrossTest, TestOutput) {

	// creates points in a configuration that will cause crossing edges
	// points chosen in a way that configuration of indexes is simple to predict
	// here it should be {0, 1, 2, 4, 3, 0} for the complete trip
	std::vector<int> points { 50, 50, 50, 99, 100, 100, 100, 50, 200, 75};
	std::vector<int> expectedIndizes { 1, 2, 4, 3 };

	NoCross testCross = NoCross(points);
	
	testCross.NextPointNoCross();

	std::vector<int> indizes = testCross.getBestPathIndizes();
	ASSERT_EQ(indizes, expectedIndizes);

	// verify that results are set correctly
	char msgResults[] = "Error in setting result";
	ASSERT_TRUE(testCross.hasSearched()) << msgResults;
	ASSERT_TRUE(testCross.getBestDist() < std::numeric_limits<double>::max()) << msgResults;
	ASSERT_TRUE(testCross.getTimeToFindBest() >= 0) << msgResults;
	ASSERT_TRUE(testCross.getTimeToFinish() >= 0) << msgResults;

	// verify that reset works
	char msgReset[] = "Error in resetting NoCross class";
	testCross.reset();
	ASSERT_FALSE(testCross.hasSearched()) << msgReset;
	ASSERT_TRUE(testCross.getBestDist() == std::numeric_limits<double>::max()) << msgReset;
	ASSERT_TRUE(testCross.getTimeToFindBest() == -1) << msgReset;
	ASSERT_TRUE(testCross.getTimeToFinish() == -1) << msgReset;
}

/* Checks that the individual functions work correctly */
TEST(NoCrossTest, TestFunctions) {

	std::vector<int> points{ 50, 50, 50, 99, 100, 100, 100, 50, 200, 75 };

	// indexes for expected results
	/*indizes only considering closest next point*/
	std::vector<int> shortestPathIndizes{ 1, 2, 3, 4 };
	/*indizes after crossing edge swapped*/
	std::vector<int> noCrossIndizes{ 1, 2, 4, 3 };

	NoCross testCross = NoCross(points);
	std::vector<double> distances = testCross.createDistMat(points);
	std::unordered_map<int, Edge> indexToEdges;

	testCross.shortestPathIndizes(distances, indexToEdges);

	// the indexToEdges map must no be empty
	ASSERT_FALSE(indexToEdges.empty()) << "Edges map not filled";

	// compare shortest path indizes
	testCross.edgeMapToIndexVector(distances, indexToEdges);
	ASSERT_EQ(testCross.getBestPathIndizes(), shortestPathIndizes);

	// check that index 3 and 4 are switched because of crossing edges
	testCross.iterateOverPath(indexToEdges);

	testCross.edgeMapToIndexVector(distances, indexToEdges);
	ASSERT_EQ(testCross.getBestPathIndizes(), noCrossIndizes);

}