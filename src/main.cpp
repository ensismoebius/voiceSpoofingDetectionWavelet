#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
#include <cmath>

#include "lib/wave/Wav.cpp"
#include "lib/wave/filtersOperations.h"
#include "lib/wavelet/waveletOperations.h"
#include "lib/wavelet/waveletCoeficients.h"
#include "lib/linearAlgebra/linearAlgebra.h"
#include "lib/matplotlib-cpp/matplotlibcpp.h"

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
	linearAlgebra::discreteCosineTransform(featureVector, rangesSize - 1);

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

void waveletAnaliticFunction(double *signal, int signalLength, unsigned int samplingRate, std::string path) {

	namespace plt = matplotlibcpp;

	unsigned int level = 4;
	unsigned int plotResolution = 1; //the lower the better
	std::vector<double> wavelet = wavelets::daub76;
	std::vector<double> xdot(signal, signal + signalLength);

	//TODO Create an object witch returns all details and aproximations
	std::vector<double> res = wavelets::malat(xdot, wavelet, level);

	std::vector<double> energies(level + 1);

	for (unsigned int levelIndex = 0; levelIndex <= level; levelIndex++) {

		unsigned sstart = res.size() / std::pow(2, levelIndex + 1);
		unsigned send = res.size() / std::pow(2, levelIndex);

		//TODO VERY inefficient: Plot directlly from an object (see above)
		plt::subplot(level + 2, 1, levelIndex + 1);
		unsigned int xcounter = 0;
		std::vector<int> x;
		std::vector<double> y;

		for (unsigned int indexRange = sstart; indexRange < send; indexRange++) {

			if (indexRange % plotResolution == 0) {
				x.push_back(xcounter++);
				y.push_back(res.at(indexRange));
			}

			energies.at(levelIndex) += std::pow(res.at(indexRange), 2);
		}

		plt::title("Scale " + std::to_string(levelIndex + 1));
		plt::xlim(0, (int) xcounter);
		plt::plot(x, y);
		plt::pause(0.0000000000001);

		xcounter = 0;
		x.clear();
		y.clear();
	}

//	plt::subplot(3, 1, 1);
//	plt::title("Signal");
//	plt::xlim(0, (int) xdot.size());
//	plt::named_plot("Signal", xdot, "r-");

//	plt::subplot(2, 2, 2);
//	plt::title("Wavelet");
//	plt::named_plot("Wavelet", wavelet, "b-");

//	plt::subplot(3, 1, 2);
//	plt::title("Transformed");
//	plt::named_plot("Transformed", res, "g-");
//
	plt::subplot(level + 2, 1, level + 2);
	plt::title("Energies");
	plt::named_plot("Energy", energies, "y-");
	plt::show();
//
//	plt::xlim(0, (int) std::max(xdot.size(), wavelet.size()));
//	plt::legend();

}

int main(int i, char *args[]) {

	std::cout << std::fixed;
	std::cout << std::setprecision(20);

	Wav w;
	w.setCallbackFunction(waveletAnaliticFunction);

	std::ifstream fileListStream;
	fileListStream.open(args[1], std::ios::in);

	std::string line;
	while (std::getline(fileListStream, line)) {
		std::cout << resultIndex << ":" << line << std::endl;

		// lines that begins with # are going to be ignored
		if (line.find("#") == 0) continue;

		w.read(line.data());
		w.process();
		//	w.write("/tmp/teste.wav");
	}

	for (unsigned int columns = 0; columns < 14; columns++) {
		for (unsigned int files = 0; files < resultIndex; files++) {
			std::cout << results[files][columns] << "\t";
		}
		std::cout << std::endl;
	}

	return 0;
}
