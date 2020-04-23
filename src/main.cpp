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

/*
 **************************
 **************************
 * Experiment parameters **
 **************************
 **************************
 */
// Transformation level
int level = 1;
// Wavelet function
std::vector<double> wavelet = wavelets::altHaar;
// Wavelet type
wavelets::TransformMode mode = wavelets::PACKET_WAVELET;

/**
 * Analityc function
 * @param signal
 * @param signalLength
 * @param samplingRate
 * @param path
 */
void waveletAnaliticFunction(std::vector<double> &signal, int &signalLength, unsigned int samplingRate, std::string path) {

//	// Transformation level
//	int level = 8;

// Expands the signal length to optimize the wavelet transform
	signal.resize(wavelets::getNextPowerOfTwo(signal.size()), 0);

	// Does the transformations
	// wavelets::WaveletTransformResults res = wavelets::malat(signal, wavelet, wavelets::PACKET_WAVELET, level);
	wavelets::WaveletTransformResults res = wavelets::malat(signal, wavelet, mode, level);

	// Modify the signal with the results
	for (unsigned int i = 0; i < signal.size(); ++i) {
		signal.at(i) = res.transformedSignal.at(i);
	}
}

/**
 * Just for testing purposes
 */
void waveletTransformTestFunction() {
	std::vector<double> signal = { 32, 10, 20, 38, 37, 28, 38, 34, 18, 24, 24, 9, 23, 24, 28, 34 };
	int length = 16;
	waveletAnaliticFunction(signal, length, 44100, "ssss");
}

void plotResults(std::vector<double> data) {

	// Alias for a easier use of matplotlib
	namespace plt = matplotlibcpp;

	plt::xlim(0, int(data.size()));
	plt::title("Signal");

	plt::named_plot("Signal", data, "r-");
	plt::show();
	plt::pause(.1);
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

		// does not need to write a new file: We will just analise it
		//w.write("/tmp/teste.wav");

		int parts = wavelets::WaveletTransformResults::getWaveletPacketAmountOfParts(level);

		for (int i = 0; i < parts; ++i) {
			std::vector<double> data = wavelets::WaveletTransformResults::getWaveletPacketTransforms(w.getData(), i, level);
			plotResults(data);
		}

	}

	return 0;
}
