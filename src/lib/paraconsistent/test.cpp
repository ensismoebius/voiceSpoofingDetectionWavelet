#include <map>
#include <cmath>
#include <vector>
#include <cassert>
#include <iostream>
#include "paraconsistent.h"
#include "../linearAlgebra/linearAlgebra.h"

void norm(std::vector<double> &sig)
{
	double energy = 0;

	for (double v : sig)
	{
		energy += std::pow(v, 2);
	}

	for (double &v : sig)
	{
		v = v / energy;
	}
}

void initializeClasses(std::map<std::string, std::vector<std::vector<double>>> &arrClasses, unsigned int &amountOfClasses, unsigned int &featureVectorsPerClass, unsigned int &featureVectorSize)
{
	// Setting up the quantities
	amountOfClasses = 3;
	featureVectorSize = 2;
	featureVectorsPerClass = 4;
	std::vector<std::string> classNames = { "c1", "c2", "c3" };

	// Initializing the "lists"
	for (unsigned int classIndex = 0; classIndex < amountOfClasses; ++classIndex)
	{
		std::vector<std::vector<double>> featureVectorSet(featureVectorsPerClass);

		for (unsigned int featureVectorIndex = 0; featureVectorIndex < featureVectorsPerClass; ++featureVectorIndex)
		{
			std::vector<double> featureVector(featureVectorSize);

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

	linearAlgebra::normalizeVectorToSum1AllPositive(arrClasses[classNames[0]][0]);
	linearAlgebra::normalizeVectorToSum1AllPositive(arrClasses[classNames[0]][1]);
	linearAlgebra::normalizeVectorToSum1AllPositive(arrClasses[classNames[0]][2]);
	linearAlgebra::normalizeVectorToSum1AllPositive(arrClasses[classNames[0]][3]);
	linearAlgebra::normalizeVectorToSum1AllPositive(arrClasses[classNames[1]][0]);
	linearAlgebra::normalizeVectorToSum1AllPositive(arrClasses[classNames[1]][1]);
	linearAlgebra::normalizeVectorToSum1AllPositive(arrClasses[classNames[1]][2]);
	linearAlgebra::normalizeVectorToSum1AllPositive(arrClasses[classNames[1]][3]);
	linearAlgebra::normalizeVectorToSum1AllPositive(arrClasses[classNames[2]][0]);
	linearAlgebra::normalizeVectorToSum1AllPositive(arrClasses[classNames[2]][1]);
	linearAlgebra::normalizeVectorToSum1AllPositive(arrClasses[classNames[2]][2]);
	linearAlgebra::normalizeVectorToSum1AllPositive(arrClasses[classNames[2]][3]);
}

/**
 * To run this test comment out the main function
 * and rename this function to "main"
 * @param argc
 * @param argv
 * @return 0
 */
int main(int argc, char **argv)
{
	unsigned int amountOfClasses;
	unsigned int featureVectorSize;
	unsigned int featureVectorsPerClass;
	std::map<std::string, std::vector<std::vector<double>>> arrClasses;

	initializeClasses(arrClasses, amountOfClasses, featureVectorsPerClass, featureVectorSize);

	double alpha = calculateAlpha(amountOfClasses, featureVectorsPerClass, featureVectorSize, arrClasses);
	double betha = calculateBeta(amountOfClasses, featureVectorsPerClass, featureVectorSize, arrClasses);

	double certaintyDegree_G1 = calcCertaintyDegree_G1(alpha, betha);
	double contradictionDegree_G2 = calcContradictionDegree_G2(alpha, betha);
	double distanceTo1_0 = std::sqrt(std::pow(certaintyDegree_G1 - 1, 2) + std::pow(contradictionDegree_G2, 2));

	assert(betha == 0);
	assert(alpha == 0.97499999999999998);
	assert(distanceTo1_0 == 0.035355339059327411);
	assert(certaintyDegree_G1 == 0.97499999999999998);
	assert(contradictionDegree_G2 == -0.025000000000000022);

	// Calculating the position at the paraconsistent plane
	std::cout << "Certainty degree     :" << certaintyDegree_G1 << std::endl;
	std::cout << "Contradiction degree :" << contradictionDegree_G2 << std::endl;
	std::cout << "Dist from point(1,0) :" << distanceTo1_0 << std::endl;
	std::cout << "----------------------" << std::endl;

	return 0;
}
