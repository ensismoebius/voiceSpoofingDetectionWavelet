#include <map>
#include <cmath>
#include <vector>
#include <cassert>
#include <iostream>
#include <gtest/gtest.h>
#include "paraconsistent.h"
#include "../linearAlgebra/linearAlgebra.h"

unsigned int amountOfClasses;
unsigned int featureVectorSize;
unsigned int featureVectorsPerClass;
std::map<std::string, std::vector<std::vector<long double>>> arrClasses;

void initializeClasses(std::map<std::string, std::vector<std::vector<long double>>> &arrClasses, unsigned int &amountOfClasses, unsigned int &featureVectorsPerClass, unsigned int &featureVectorSize)
{
	// Setting up the quantities
	amountOfClasses = 3;
	featureVectorSize = 2;
	featureVectorsPerClass = 4;
	std::vector<std::string> classNames = { "c1", "c2", "c3" };

	// Initializing the "lists"
	for (unsigned int classIndex = 0; classIndex < amountOfClasses; ++classIndex)
	{
		std::vector<std::vector<long double>> featureVectorSet(featureVectorsPerClass);

		for (unsigned int featureVectorIndex = 0; featureVectorIndex < featureVectorsPerClass; ++featureVectorIndex)
		{
			std::vector<long double> featureVector(featureVectorSize);

			featureVectorSet.push_back(featureVector);
		}

		arrClasses[classNames[classIndex]] = featureVectorSet;
	}

	// Populating
	arrClasses[classNames[0]][0] = { 0.90, 0.12 };
	arrClasses[classNames[0]][1] = { 0.88, 0.14 };
	arrClasses[classNames[0]][2] = { 0.88, 0.13 };
	arrClasses[classNames[0]][3] = { 0.89, 0.11 };

	arrClasses[classNames[1]][0] = { 0.55, 0.53 };
	arrClasses[classNames[1]][1] = { 0.53, 0.55 };
	arrClasses[classNames[1]][2] = { 0.54, 0.54 };
	arrClasses[classNames[1]][3] = { 0.56, 0.54 };

	arrClasses[classNames[2]][0] = { 0.10, 0.88 };
	arrClasses[classNames[2]][1] = { 0.11, 0.86 };
	arrClasses[classNames[2]][2] = { 0.12, 0.87 };
	arrClasses[classNames[2]][3] = { 0.11, 0.88 };
}

TEST(paraconsistentTest, alpha)
{
	long double alpha = calculateAlpha(amountOfClasses, featureVectorsPerClass, featureVectorSize, arrClasses);
	ASSERT_NEAR(alpha, 0.9749999, 0.000001);
}

TEST(paraconsistentTest, betha){
	long double betha = calculateBeta(amountOfClasses, featureVectorsPerClass, featureVectorSize, arrClasses);
	ASSERT_EQ(betha, 0);
}

TEST(paraconsistentTest, distanceTo1_0)
{
	long double alpha = calculateAlpha(amountOfClasses, featureVectorsPerClass, featureVectorSize, arrClasses);
	long double betha = calculateBeta(amountOfClasses, featureVectorsPerClass, featureVectorSize, arrClasses);

	double certaintyDegree_G1 = calcCertaintyDegree_G1(alpha, betha);
	double contradictionDegree_G2 = calcContradictionDegree_G2(alpha, betha);
	double distanceTo1_0 = std::sqrt(std::pow(certaintyDegree_G1 - 1, 2) + std::pow(contradictionDegree_G2, 2));

	ASSERT_NEAR(distanceTo1_0, 0.035, 0.001);
}

TEST(paraconsistentTest, certaintyDegree_G1)
{
	long double alpha = calculateAlpha(amountOfClasses, featureVectorsPerClass, featureVectorSize, arrClasses);
	long double betha = calculateBeta(amountOfClasses, featureVectorsPerClass, featureVectorSize, arrClasses);
	double certaintyDegree_G1 = calcCertaintyDegree_G1(alpha, betha);

	ASSERT_NEAR(certaintyDegree_G1, 0.975, 0.0001);
}

TEST(paraconsistentTest, contradictionDegree_G2)
{
	long double alpha = calculateAlpha(amountOfClasses, featureVectorsPerClass, featureVectorSize, arrClasses);
	long double betha = calculateBeta(amountOfClasses, featureVectorsPerClass, featureVectorSize, arrClasses);
	double contradictionDegree_G2 = calcContradictionDegree_G2(alpha, betha);

	ASSERT_NEAR(contradictionDegree_G2, -0.025, 0.0001);
}

int main(int argc, char **argv)
{
	initializeClasses(arrClasses, amountOfClasses, featureVectorsPerClass, featureVectorSize);
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
