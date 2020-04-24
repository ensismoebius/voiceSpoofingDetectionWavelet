/**
 * @author André Furlan
 * @email ensismoebius@gmail.com
 * This whole project are under GPLv3, for
 * more information read the license file
 *
 * 23 de abr de 2020
 *
 */
#include <cmath>
#include <string>
#include <sstream>

#include "lib/wave/filtersOperations.h"
#include "lib/linearAlgebra/linearAlgebra.h"

double* createFeatureVector(double *signal, int signalLength, unsigned int samplingRate, int filterOrder, std::string path, bool logSmooth = false) {

	// size of the range
	int rangesSize = 14;

	// Ranges for MEL scale
	double ranges[14] = { 20, 160, 394, 670, 1000, 1420, 1900, 2450, 3120, 4000, 5100, 6600, 9000, 14000 };

	double *window = createTriangularWindow(filterOrder);

	// feature vector has the amount of values minus 1 than ranges
	// because we are summing up intervals
	double *featureVector = new double[rangesSize - 1];

	// Initializing the vector
	for (int i = 0; i < rangesSize - 1; i++) {
		featureVector[i] = 0;
	}

	/** Calculating the signal strength for each interval **/
	double rangeEnd = 0;
	double rangeStart = 0;

	// for every pair of ranges we need to copy the original signal
	double *copiedSignal = new double[signalLength];

	for (int i = 0; i < rangesSize - 1; i++) {

		// Select start and end ranges
		rangeStart = ranges[i];
		rangeEnd = ranges[i + 1];

		// Create the signal filter
		double *filter = createBandPassFilter(filterOrder, samplingRate, rangeStart, rangeEnd);

		// Apply window
		applyWindow(filter, window, filterOrder);

		// Create a copy of the signal
		std::copy(signal, signal + signalLength, copiedSignal);

		// Apply the filter
		linearAlgebra::convolution(copiedSignal, signalLength, filter, filterOrder);

		// dispose filter
		delete[] filter;

		// normalize signal
		linearAlgebra::normalizeVector(copiedSignal, signalLength);

		// Calculating the energies
		double energy = 0;
		for (int j = 0; j < signalLength; j++) {

			// Calculate the energies for each energy interval
			energy = std::pow(copiedSignal[j], 2);

			if (logSmooth) {
				// apply log to it.
				energy = energy == 0 ? 0 : log(energy);
			}

			// Calculate the sum of all energies for this range
			featureVector[i] += energy;
		}

	}

	// Normalize the resulting feature vector
	linearAlgebra::normalizeVector(featureVector, rangesSize - 1);

	// Apply a DCT (Discrete Cosine Transform)
	//linearAlgebra::discreteCosineTransform(featureVector, rangesSize - 1);

	delete[] window;
	delete[] copiedSignal;
	return featureVector;
}

unsigned int resultIndex = 0;
std::string **results = 0;

void analiticFunction(double *signal, int signalLength, unsigned int samplingRate, std::string path) {

	unsigned int filterOrder = 27;

	std::stringstream dataColumn;

	dataColumn << "Digit:" << path.substr(path.find_last_of('/') - 1, 1) << "-";
	dataColumn << "Signal:" << path.substr(path.find_last_of('/') + 1, 255);

	results[resultIndex] = new std::string[14];
	results[resultIndex][0] = dataColumn.str();

	double *fv = createFeatureVector(signal, signalLength, samplingRate, filterOrder, path);
	for (int i = 0; i < 13; i++) {
		results[resultIndex][i + 1] = std::to_string(fv[i]);
	}
	delete[] fv;

	resultIndex++;
}
