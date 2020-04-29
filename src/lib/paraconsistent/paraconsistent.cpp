/*
 * paraconsistent.cpp
 *
 *  Created on: 30 de abr de 2019
 *      Author: ensis
 */

#include <map>
#include <limits>
#include <vector>

double calcCertaintyDegree_G1(double alpha, double betha) {
	return alpha - betha;
}
double calcContradictionDegree_G2(double alpha, double betha) {
	return alpha + betha - 1;
}

static void normalize(double *&featureVector, unsigned int size) {
	float sum = 0;

	for (unsigned int i = 0; i < size; i++) {
		sum += featureVector[i];
	}

	while (size--) {
		featureVector[size] /= sum;
	}
}
static void normalize(std::vector<double> &featureVector, unsigned int size) {
	float sum = 0;

	for (unsigned int i = 0; i < size; i++) {
		sum += featureVector[i];
	}

	while (size--) {
		featureVector[size] /= sum;
	}
}

static void normalizeFeatureVectors(double **&featureVectors, unsigned int vectorSize, unsigned int subVectorsSize) {

	while (vectorSize--) {
		for (unsigned int i = 0; i < subVectorsSize; i++) {
			normalize(featureVectors[vectorSize], subVectorsSize);
		}
	}
}
static void normalizeFeatureVectors(std::vector<std::vector<double>> &featureVectors, unsigned int vectorSize, unsigned int subVectorsSize) {

	while (vectorSize--) {
		for (unsigned int i = 0; i < subVectorsSize; i++) {
			normalize(featureVectors[vectorSize], subVectorsSize);
		}
	}
}

void normalizeClassesFeatureVectors(unsigned int amountOfClasses, unsigned int featureVectorsPerClass, unsigned int featureVectorSize, double ***arrClasses) {
	for (unsigned int i = 0; i < amountOfClasses; i++) {
		normalizeFeatureVectors(arrClasses[i], featureVectorsPerClass, featureVectorSize);
	}
}

void normalizeClassesFeatureVectors(unsigned int amountOfClasses, unsigned int featureVectorsPerClass, unsigned int featureVectorSize, std::vector<std::vector<std::vector<double>>> &arrClasses) {
	for (unsigned int i = 0; i < amountOfClasses; i++) {
		normalizeFeatureVectors(arrClasses[i], featureVectorsPerClass, featureVectorSize);
	}
}

double calculateAlpha(unsigned int amountOfClasses, unsigned int featureVectorsPerClass, unsigned int featureVectorSize, std::map<std::string, std::vector<std::vector<double>>> &arrClasses) {
	std::map<std::string, std::vector<double>> arrLargestRange;
	std::map<std::string, std::vector<double>> arrSmallestRange;

	double alpha = std::numeric_limits<double>::max();
	double temp = 0;

	// ci = class index
	// ii = item index
	// fvi = featureVectorIndex

	// initializes the vectors
	for (std::pair<std::string, std::vector<std::vector<double>>> clazz : arrClasses) {
		// creates sub vector
		arrLargestRange[clazz.first].resize(featureVectorSize, std::numeric_limits<double>::min());
		arrSmallestRange[clazz.first].resize(featureVectorSize, std::numeric_limits<double>::max());
	}

	// Calculating the range vectors
	for (std::pair<std::string, std::vector<std::vector<double>>> clazz : arrClasses) {
		for (unsigned int ii = 0; ii < featureVectorSize; ii++) {
			for (unsigned int fvi = 0; fvi < featureVectorsPerClass; fvi++) {
				double item = clazz.second[fvi][ii];
				arrLargestRange[clazz.first][ii] = arrLargestRange[clazz.first][ii] < item ? item : arrLargestRange[clazz.first][ii];
				arrSmallestRange[clazz.first][ii] = arrSmallestRange[clazz.first][ii] > item ? item : arrSmallestRange[clazz.first][ii];
			}
		}

		// Finding alpha
		for (unsigned int si = 0; si < featureVectorSize; ++si) {
			temp += 1 - (arrLargestRange[clazz.first][si] - arrSmallestRange[clazz.first][si]);
		}

		temp /= featureVectorSize;
		alpha = alpha > temp ? temp : alpha;
		temp = 0;
	}

	return alpha;
}
double calculateAlpha(unsigned int amountOfClasses, unsigned int featureVectorsPerClass, unsigned int featureVectorSize, double ***&arrClasses) {

	double **arrLargestRange = new double*[amountOfClasses];
	double **arrSmallestRange = new double*[amountOfClasses];

	double alpha = std::numeric_limits<double>::max();
	double temp = 0;

	// ci = class index
	// ii = item index
	// fvi = featureVectorIndex

	// initializes the vectors
	for (unsigned int ci = 0; ci < amountOfClasses; ci++) {
		// creates sub vector
		arrLargestRange[ci] = new double[featureVectorSize];
		arrSmallestRange[ci] = new double[featureVectorSize];
		for (unsigned int ii = 0; ii < featureVectorSize; ii++) {
			// cleans undesirable values
			arrLargestRange[ci][ii] = std::numeric_limits<double>::min();
			arrSmallestRange[ci][ii] = std::numeric_limits<double>::max();
		}
	}

	// Calculating the range vectors
	for (unsigned int ci = 0; ci < amountOfClasses; ci++) {
		for (unsigned int ii = 0; ii < featureVectorSize; ii++) {
			for (unsigned int fvi = 0; fvi < featureVectorsPerClass; fvi++) {
				double item = arrClasses[ci][fvi][ii];
				arrLargestRange[ci][ii] = arrLargestRange[ci][ii] < item ? item : arrLargestRange[ci][ii];
				arrSmallestRange[ci][ii] = arrSmallestRange[ci][ii] > item ? item : arrSmallestRange[ci][ii];
			}
		}

		// Finding alpha
		for (unsigned int si = 0; si < featureVectorSize; ++si) {
			temp += 1 - (arrLargestRange[ci][si] - arrSmallestRange[ci][si]);
		}

		temp /= featureVectorSize;
		alpha = alpha > temp ? temp : alpha;
		temp = 0;
	}

	// clear the range vectors
	for (unsigned int ci = 0; ci < amountOfClasses; ci++) {
		delete[] arrLargestRange[ci];
		delete[] arrSmallestRange[ci];
	}

	delete[] arrLargestRange;
	delete[] arrSmallestRange;

	return alpha;
}

double calculateBeta(unsigned int amountOfClasses, unsigned int featureVectorsPerClass, unsigned int featureVectorSize, std::map<std::string, std::vector<std::vector<double>>> &arrClasses) {
	std::map<std::string, std::vector<double>> arrLargestItens;
	std::map<std::string, std::vector<double>> arrSmallestItems;

	// ci = class index
	// ii = item index
	// fvi = featureVectorIndex

	// initializes the range vectors
	for (std::pair<std::string, std::vector<std::vector<double>>> clazz : arrClasses) {
		// creates sub vector
		arrLargestItens[clazz.first].resize(featureVectorSize, std::numeric_limits<double>::min());
		arrSmallestItems[clazz.first].resize(featureVectorSize, std::numeric_limits<double>::max());
	}

	// Calculating the range vectors
	for (std::pair<std::string, std::vector<std::vector<double>>> clazz : arrClasses) {
		for (unsigned int ii = 0; ii < featureVectorSize; ii++) {
			for (unsigned int fvi = 0; fvi < featureVectorsPerClass; fvi++) {
				double item = arrClasses[clazz.first][fvi][ii];
				arrLargestItens[clazz.first][ii] = arrLargestItens[clazz.first][ii] < item ? item : arrLargestItens[clazz.first][ii];
				arrSmallestItems[clazz.first][ii] = arrSmallestItems[clazz.first][ii] > item ? item : arrSmallestItems[clazz.first][ii];
			}
		}
	}

	// Calculating the R factor (amount of times that a feature
	// vector item overlaps an range vector item
	unsigned long int R = 0;

	// comparing all featureVector elements from a class
	// with all range vectors from another classes
	for (std::pair<std::string, std::vector<std::vector<double>>> clazz : arrClasses) {
		for (unsigned int fvi = 0; fvi < featureVectorsPerClass; fvi++) {

			// We got in to a class (ci index), now we have to compare
			// it with all the range vectors form another classes (ci2 index)
			for (std::pair<std::string, std::vector<std::vector<double>>> clazz2 : arrClasses) {

				// do not compare with the range vector from the same class
				if (clazz2.first.compare(clazz.first) == 0) continue;

				for (unsigned int ii = 0; ii < featureVectorSize; ii++) {
					R += arrLargestItens[clazz2.first][ii] == arrClasses[clazz.first][fvi][ii] ? 1 : 0;
					R += arrSmallestItems[clazz2.first][ii] == arrClasses[clazz.first][fvi][ii] ? 1 : 0;
				}
			}
		}
	}

	// Return betha
	return R / (double) (amountOfClasses * (amountOfClasses - 1.0) * featureVectorsPerClass * featureVectorSize);
}

double calculateBeta(unsigned int amountOfClasses, unsigned int featureVectorsPerClass, unsigned int featureVectorSize, double ***&arrClasses) {
	double **arrLargestItens = new double*[amountOfClasses];
	double **arrSmallestItems = new double*[amountOfClasses];

	// ci = class index
	// ii = item index
	// fvi = featureVectorIndex

	// initializes the range vectors
	for (unsigned int ci = 0; ci < amountOfClasses; ci++) {
		// creates sub vector
		arrLargestItens[ci] = new double[featureVectorSize];
		arrSmallestItems[ci] = new double[featureVectorSize];
		for (unsigned int ii = 0; ii < featureVectorSize; ii++) {
			// cleans undesirable values
			arrLargestItens[ci][ii] = std::numeric_limits<double>::min();
			arrSmallestItems[ci][ii] = std::numeric_limits<double>::max();
		}
	}

	// Calculating the range vectors
	for (unsigned int ci = 0; ci < amountOfClasses; ci++) {
		for (unsigned int ii = 0; ii < featureVectorSize; ii++) {
			for (unsigned int fvi = 0; fvi < featureVectorsPerClass; fvi++) {
				double item = arrClasses[ci][fvi][ii];
				arrLargestItens[ci][ii] = arrLargestItens[ci][ii] < item ? item : arrLargestItens[ci][ii];
				arrSmallestItems[ci][ii] = arrSmallestItems[ci][ii] > item ? item : arrSmallestItems[ci][ii];
			}
		}
	}

	// Calculating the R factor (amount of times that a feature
	// vector item overlaps an range vector item
	unsigned long int R = 0;

	// comparing all featureVector elements from a class
	// with all range vectors from another classes
	for (unsigned int ci = 0; ci < amountOfClasses; ci++) {
		for (unsigned int fvi = 0; fvi < featureVectorsPerClass; fvi++) {

			// We got in to a class (ci index), now we have to compare
			// it with all the range vectors form another classes (ci2 index)
			for (unsigned int ci2 = 0; ci2 < amountOfClasses; ci2++) {

				// do not compare with the range vector from the same class
				if (ci2 == ci) continue;

				for (unsigned int ii = 0; ii < featureVectorSize; ii++) {
					R += arrLargestItens[ci2][ii] == arrClasses[ci][fvi][ii] ? 1 : 0;
					R += arrSmallestItems[ci2][ii] == arrClasses[ci][fvi][ii] ? 1 : 0;
				}
			}
		}
	}

	// The betha
	double betha = R / (double) (amountOfClasses * (amountOfClasses - 1.0) * featureVectorsPerClass * featureVectorSize);

	// cleans the range vectors
	for (unsigned int ci = 0; ci < amountOfClasses; ci++) {
		delete[] arrLargestItens[ci];
		delete[] arrSmallestItems[ci];
	}

	delete[] arrLargestItens;
	delete[] arrSmallestItems;

	return betha;
}
