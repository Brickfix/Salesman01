#include "pch.h"
#include "../BruteForce.h"
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

TEST(TravelerTest, TestDefaults) {

	std::vector<int> points { 50,  50,  50,  100, 100, 100, 100, 50 };

	Traveler defaultTestTraveler(points);

	EXPECT_FALSE(defaultTestTraveler.hasSearched()) << "Search flag must be false if no search was conducted";
	EXPECT_EQ(0, defaultTestTraveler.getBestPathIndizes().size()) << "BestPathIndizes vector must be empty without search";

	// other defaults must not really be tested
	ASSERT_EQ(-1, defaultTestTraveler.getTimeToFindBest()) << "timeToFindBest must be -1 if no best path search was yet conducted";
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
	//						p0x, p0y, p1x, p1y, p2x, p2y, p3x, p3y
	std::vector<int> points { 50,  50,  50,  100, 100, 100, 100, 50 };

	BruteForce fourPoints(points);
	fourPoints.iterateThroughPoints();

	EXPECT_TRUE(fourPoints.hasSearched()) << "Searched flag not set to true";

	std::vector<int> indexes = fourPoints.getBestPathIndizes();
	double distance = fourPoints.getBestDist();

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
	ASSERT_DOUBLE_EQ(expectedDist, distance) << "Result distance was not as expected";

	// reset for quicker alternative
	fourPoints.reset();
	matches = false;
	validCount0 = 0;
	validCount1 = 0;

	// make sure Traveler class has correctly reset
	ASSERT_FALSE(fourPoints.hasSearched());
	ASSERT_NE(fourPoints.getBestDist(), distance);

	fourPoints.permutateHalf();

	indexes = fourPoints.getBestPathIndizes();

	// only validIndexesO should have been searched
	for (int j = 0; j < 3; j++) {
		if (indexes[j] == validIndexes0[j]) {
			validCount0++;
		}
		else if (indexes[j] == validIndexes1[j]) {
			validCount1++;
		}
	}

	ASSERT_EQ(3, validCount0);
	ASSERT_NE(3, validCount1);
}


int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}