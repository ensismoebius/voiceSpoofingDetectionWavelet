#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "lib/matplotlib-cpp/matplotlibcpp.h"
#include "lib/wave/Wav.cpp"
#include "lib/wavelet/waveletCoeficients.h"
#include "lib/wavelet/waveletOperations.h"
#include "lib/wavelet/WaveletTransformResults.cpp"

#include "lib/linearAlgebra/linearAlgebra.h"

///**
// * Just for testing purposes
// */
//void waveletTransformTestFunction() {
//	std::vector<double> signal = { 32, 10, 20, 38, 37, 28, 38, 34, 18, 24, 24, 9, 23, 24, 28, 34 };
//	int length = 16;
//	waveletAnaliticFunction(signal, length, 44100, "ssss");
//}

void plotResults(std::vector<double> data) {

	// Alias for a easier use of matplotlib
	namespace plt = matplotlibcpp;

	plt::xlim(0, int(data.size()));
	plt::title("Signal");

	plt::named_plot("Signal", data, "r-");
	plt::show();
	plt::pause(.1);
}

/*
 **************************
 **************************
 * Experiment parameters **
 **************************
 **************************
 */
// Transformation level
int level = 12;
// Wavelet function
std::vector<double> wavelet = wavelets::altHaar;
// Wavelet type
wavelets::TransformMode mode = wavelets::PACKET_WAVELET;

std::vector<double> createFeatureVector(wavelets::WaveletTransformResults transformedSignal, unsigned int samplingRate, bool logSmooth = false) {

	// Ranges for MEL scale
	std::vector<double> ranges = { 20, 160, 394, 670, 1000, 1420, 1900, 2450, 3120, 4000, 5100, 6600, 9000, 14000 };

	// feature vector has the amount of values minus 1 than ranges
	// because we are summing up intervals
	std::vector<double> featureVector(ranges.size() - 1);

	int maxFrequency = samplingRate / 2;
	int frequecyChunckSize = maxFrequency / transformedSignal.getWaveletPacketAmountOfParts();

	/** Calculating the signal strength for each interval **/
	double rangeEnd = 0;
	double rangeStart = 0;

	// for every pair of ranges we need to copy the original signal
	std::vector<double> copiedSignal(transformedSignal.transformedSignal);
	//std::vector<double> copiedSignal(signal.size());

	for (unsigned int i = 0; i < ranges.size() - 1; i++) {

		// Select start and end ranges
		rangeStart = ranges.at(i);
		rangeEnd = ranges.at(i + 1);

		int startIndex = rangeStart / frequecyChunckSize;
		int endIndex = rangeEnd / frequecyChunckSize;

		for (int j = startIndex; j < endIndex; ++j) {
			std::vector<double> sig1 = transformedSignal.getWaveletPacketTransforms(startIndex);

			featureVector.at(i) = 0;
			for (double v : sig1) {
				featureVector.at(i) += std::pow(v, 2);
			}

		}

	}

	// Normalize the resulting feature vector
	linearAlgebra::normalizeVector(featureVector);

	// Apply a DCT (Discrete Cosine Transform)
	linearAlgebra::discreteCosineTransform(featureVector);

	return featureVector;
}

/**
 * Analityc function
 * @param signal
 * @param signalLength
 * @param samplingRate
 * @param path
 */
void waveletAnaliticFunction(std::vector<double> &signal, int &signalLength, unsigned int samplingRate, std::string path) {

	// Expands the signal length to optimize the wavelet transform
	signal.resize(wavelets::getNextPowerOfTwo(signal.size()), 0);

	// Does the transformations
	// wavelets::WaveletTransformResults res = wavelets::malat(signal, wavelet, wavelets::PACKET_WAVELET, level);
	wavelets::WaveletTransformResults res = wavelets::malat(signal, wavelet, mode, level);

	signal = createFeatureVector(res, samplingRate, true);
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

		// lines that begins with # are going to be ignored
		if (line.find("#") == 0) continue;

		w.read(line.data());
		w.process();

		plotResults(w.getData());
	}

	return 0;
}
