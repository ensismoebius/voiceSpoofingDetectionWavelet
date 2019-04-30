//============================================================================
// Name        : DSPFeature.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <stdlib.h>
#include <cmath>
#include <iostream>

#include "../gnuplot/gnuplotCalls.h"
#include "paraconsistent.h"

static void randomlyPopulateClassFeatureVectors(double**& featureVectors, unsigned int samplesPerClass, unsigned int featureVectorSize) {
	for (unsigned int ci = 0; ci < samplesPerClass; ci++) {
		for (unsigned int fi = 0; fi < featureVectorSize; fi++) {
			featureVectors[ci][fi] = (random() / (double) RAND_MAX);
		}
	}
}

static void clearClasses(double***& arrClasses, unsigned int amountOfClasses, unsigned int featureVectorsPerClass, unsigned int featureVectorSize) {

	for (unsigned int ci = 0; ci < amountOfClasses; ci++) {
		for (unsigned int fvi = 0; fvi < featureVectorsPerClass; fvi++) {
			delete[] arrClasses[ci][fvi];
		}
		delete[] arrClasses[ci];
	}
}

static void show(double***& arrClasses, unsigned int amountOfClasses, unsigned int featureVectorsPerClass, unsigned int featureVectorSize) {

	for (unsigned int ci = 0; ci < amountOfClasses; ci++) {
		std::cout << "Class: " << ci << std::endl;
		for (unsigned int fvc = 0; fvc < featureVectorsPerClass; fvc++) {
			for (unsigned int fvi = 0; fvi < featureVectorSize; fvi++) {
				std::cout << arrClasses[ci][fvc][fvi] << ",";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
}

void initializeClasses(double***& arrClasses, unsigned int amountOfClasses, unsigned int featureVectorsPerClass, unsigned int featureVectorSize) {

	for (unsigned int ci = 0; ci < amountOfClasses; ci++) {

		arrClasses[ci] = new double*[featureVectorsPerClass];
		for (unsigned int fvi = 0; fvi < featureVectorsPerClass; fvi++) {
			arrClasses[ci][fvi] = new double[featureVectorSize];
		}
	}
}

void populateClasses(double***& arrClasses, unsigned int amountOfClasses, unsigned int samplesPerClass, unsigned int featureVectorSize, char genRandom = 0) {

	// random values for test
	if (genRandom) {
		while (amountOfClasses--) {
			randomlyPopulateClassFeatureVectors(arrClasses[amountOfClasses], samplesPerClass, featureVectorSize);
		}
		return;
	}

	//c1
	arrClasses[0][0][0] = 0.90;
	arrClasses[0][0][1] = 0.12;

	arrClasses[0][1][0] = 0.88;
	arrClasses[0][1][1] = 0.14;

	arrClasses[0][2][0] = 0.88;
	arrClasses[0][2][1] = 0.13;

	arrClasses[0][3][0] = 0.89;
	arrClasses[0][3][1] = 0.11;

	//c2
	arrClasses[1][0][0] = 0.55;
	arrClasses[1][0][1] = 0.53;

	arrClasses[1][1][0] = 0.53;
	arrClasses[1][1][1] = 0.55;

	arrClasses[1][2][0] = 0.54;
	arrClasses[1][2][1] = 0.54;

	arrClasses[1][3][0] = 0.56;
	arrClasses[1][3][1] = 0.54;

	//c3
	arrClasses[2][0][0] = 0.10;
	arrClasses[2][0][1] = 0.88;

	arrClasses[2][1][0] = 0.11;
	arrClasses[2][1][1] = 0.86;

	arrClasses[2][2][0] = 0.12;
	arrClasses[2][2][1] = 0.87;

	arrClasses[2][3][0] = 0.11;
	arrClasses[2][3][1] = 0.88;
}

//////////////////////////////////////////////////////////////////

void example() {
	unsigned int amountOfClasses = 3; //5
	unsigned int featureVectorSize = 2; //20
	unsigned int featureVectorsPerClass = 4; //10

	double*** arrClasses = new double**[amountOfClasses];
	initializeClasses(arrClasses, amountOfClasses, featureVectorsPerClass, featureVectorSize);
	populateClasses(arrClasses, amountOfClasses, featureVectorsPerClass, featureVectorSize, 0);
	normalizeClassesFeatureVectors(amountOfClasses, featureVectorsPerClass, featureVectorSize, arrClasses);
	show(arrClasses, amountOfClasses, featureVectorsPerClass, featureVectorSize);

	//////////////////////////////////////////
	/// Here begins the real functionality ///
	//////////////////////////////////////////

	double alpha = calculateAlpha(amountOfClasses, featureVectorsPerClass, featureVectorSize, arrClasses);
	double betha = calculateBeta(amountOfClasses, featureVectorsPerClass, featureVectorSize, arrClasses);

	double certaintyDegree_G1 = calcCertaintyDegree_G1(alpha, betha);
	double contradictionDegree_G2 = calcContradictionDegree_G2(alpha, betha);

	std::cout << "Certainty degree     :" << certaintyDegree_G1 << std::endl;
	std::cout << "Contradiction degree :" << contradictionDegree_G2 << std::endl;

	// calculating the position at the paraconsistent plane
	showInParaconsistentPlane(0, certaintyDegree_G1, contradictionDegree_G2);

	////////////////////////////////////////
	/// Here ends the real functionality ///
	////////////////////////////////////////

	double d_MinusOne_Zero = sqrt(pow(certaintyDegree_G1 + 1, 2) + pow(contradictionDegree_G2, 2));
	double d_One_Zero = sqrt(pow(certaintyDegree_G1 - 1, 2) + pow(contradictionDegree_G2, 2));
	double d_Zero_MinusOne = sqrt(pow(certaintyDegree_G1, 2) + pow(contradictionDegree_G2 + 1, 2));
	double d_Zero_One = sqrt(pow(certaintyDegree_G1, 2) + pow(contradictionDegree_G2 - 1, 2));

	std::cout << "Distance from (-1, 0 ):" << d_MinusOne_Zero << std::endl;
	std::cout << "Distance from ( 1, 0 ):" << d_One_Zero << std::endl;
	std::cout << "Distance from ( 0,-1 ):" << d_Zero_MinusOne << std::endl;
	std::cout << "Distance from ( 0, 1 ):" << d_Zero_One << std::endl;

	//memset(classZeroSimilaritiesVector, 0, 0.0);
	clearClasses(arrClasses, amountOfClasses, featureVectorsPerClass, featureVectorSize);
	delete[] arrClasses;
}

int main() {
	example();
}
