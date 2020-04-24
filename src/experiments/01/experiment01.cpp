/**
 * @author André Furlan
 * @email ensismoebius@gmail.com
 * This whole project are under GPLv3, for
 * more information read the license file
 *
 * 24 de abr de 2020
 *
 */

#include <cmath>
#include <string>
#include <vector>
#include <iomanip>
#include <iostream>

#include "../../lib/wave/Wav.cpp"
#include "../../lib/wavelet/waveletOperations.h"
#include "../../lib/wavelet/waveletCoeficients.h"
#include "../../lib/linearAlgebra/linearAlgebra.h"
#include "../../lib/matplotlib-cpp/matplotlibcpp.h"
#include "../../lib/wavelet/WaveletTransformResults.cpp"

namespace experiment01 {

	/**
	 * Analityc function which performs an wavelet transform
	 * of the value and calculate the energies based on MEL
	 * or BARK intervals
	 * @param signal
	 * @param signalLength
	 * @param samplingRate
	 * @param path
	 */
	void waveletAnaliticFunction(std::vector<double> &signal, int &signalLength, unsigned int samplingRate, std::string path) {

		///////////////////////
		/// Wavelet section ///
		///////////////////////

		// Expands the signal length to optimize the wavelet transform
		signalLength = wavelets::getNextPowerOfTwo(signal.size());
		signal.resize(signalLength, 0);

		// Calculate the max levels of decompositions
		// i.e. until the coeficients are formed by
		// just single numbers.
		// This is needed because at the end of the
		// transformation we will perform a MEL and
		// BARK decomposition
		int level = std::log2(signal.size());

		// Wavelet waveform function
		std::vector<double> wavelet = wavelets::altHaar;

		// Wavelet type
		wavelets::TransformMode mode = wavelets::PACKET_WAVELET;

		// Does the transformations
		wavelets::WaveletTransformResults transformedSignal = wavelets::malat(signal, wavelet, mode, level);

		///////////////////////////
		/// MEL or BARK section ///
		///////////////////////////

		// Ranges for MEL scale
		std::vector<double> scaleRanges = { 20, 160, 394, 670, 1000, 1420, 1900, 2450, 3120, 4000, 5100, 6600, 9000, 14000 };

		// feature vector has the amount of values minus 1 than ranges
		// because we are summing up intervals
		std::vector<double> featureVector(scaleRanges.size() - 1);

		// We need to known the max frequency supported
		// by the signal in order to find the values in
		// which the sums of the BARK and MEL scales
		// will be performed
		double maxFrequency = samplingRate / 2;

		// Calculate the minimum frequency range which
		// will enable the correct interval sums to
		// be performed
		double frequencyChunckSize = maxFrequency / transformedSignal.getWaveletPacketAmountOfParts();

		// Used to retrieve the interval for the sums
		double rangeScaleEnd = 0;
		double rangeScaleStart = 0;

		// Loop over all the ranges and calculate the energies inside it
		for (unsigned int i = 0; i < scaleRanges.size() - 1; i++) {

			// Retrieve the interval for the sums
			rangeScaleStart = scaleRanges.at(i);
			rangeScaleEnd = scaleRanges.at(i + 1);

			// Calculate the interval indexes inside the transformed signal
			int startIndex = rangeScaleStart / frequencyChunckSize;
			int endIndex = rangeScaleEnd / frequencyChunckSize;

			// Sum the values from selected range
			for (int j = startIndex; j < endIndex; ++j) {

				// Retrieve the values
				std::vector<double> sig1 = transformedSignal.getWaveletPacketTransforms(startIndex);

				// Sum them all!! (i.e. calculaate the energies)
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

		// Replaces the original signal
		signal = featureVector;
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

	void perform(char *args[]) {
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
	}
}
