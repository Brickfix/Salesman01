#include "pch.h"
#include "../Traveler.h"
#include <vector>
#include <utility>
#include <exception>

/*
* Test the following invalid inputs:
* - Vector containing points is uneven
* - Vector containing points is empty
* - Vector containing points contains less then 4 point coordinates (size < 8)
*/
TEST(TravelerTest, TestsInputs) {

	std::vector<int> noInputs;
	std::vector<int> toFewInputs(6, 50);
	std::vector<int> unevenInputs(13, 50);

	ASSERT_THROW({ Traveler invalid(noInputs); }, std::invalid_argument);
	ASSERT_THROW({ Traveler invalid(toFewInputs); }, std::invalid_argument);
	ASSERT_THROW({ Traveler invalid(unevenInputs); }, std::invalid_argument);
}

/*
* Asserts that for a known shortest path, the shortest path is actually found
*/
TEST(TravelerTest, TestsOutputs) {

	/* 
	* Starting points is set at index zero => point[50, 50]
	* Expected indexes will either be [1, 2, 3] or [3, 2, 1]
	* The minimum distance is 200
	*/
	//					p0x, p0y, p1x, p1y, p2x, p2y, p3x, p3y
	int pointsArr[] = { 50,  50,  50,  100, 100, 100, 100, 50 };
	std::vector<int> points(pointsArr, pointsArr + sizeof(pointsArr) / sizeof(int));

	Traveler fourPoints(points);
	std::pair<std::vector<int>, double> results = fourPoints.iterateThroughPoints();
	std::vector<int> indexes = results.first;
	double distance = results.second;

	bool matches = false;
	int validIndexes0[] = { 1, 2, 3 };
	int validIndexes1[] = { 3, 2, 1 };
	int validCount0 = 0;
	int validCount1 = 0;

	for (int i = 0; i < 3; i++) {
		if (indexes[i] == validIndexes0[i]) {
			validCount0++;
		}
		else if (indexes[i] == validIndexes1[i]) {
			validCount1++;
		}
	}

	if ((validCount0 == 3) || (validCount1 == 3)) { matches = true; }

	ASSERT_TRUE(matches) << "Result indexes did not match expectation";

	double expectedDist = 200.0;
	ASSERT_FLOAT_EQ(expectedDist, distance) << "Result distance was not as expected";
}