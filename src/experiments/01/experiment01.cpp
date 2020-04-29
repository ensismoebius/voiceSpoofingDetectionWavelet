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
#include "../../lib/wavelet/Types.h"
#include "../../lib/wavelet/waveletOperations.h"
#include "../../lib/linearAlgebra/linearAlgebra.h"
#include "../../lib/matplotlib-cpp/matplotlibcpp.h"
#include "../../lib/wavelet/WaveletTransformResults.cpp"

namespace waveletExperiments {

	enum BARK_MEL {
		BARK, MEL
	};

	class Experiment01 {

		public:
			// Use bark or mel?
			static inline BARK_MEL barkOrMel;

			// Wavelet waveform function
			static inline std::vector<double> wavelet;

			// Wavelet type
			static inline wavelets::TransformMode mode;

			static void init(std::vector<double> wavelet, wavelets::TransformMode mode, BARK_MEL barkOrMel) {
				Experiment01::mode = mode;
				Experiment01::wavelet = wavelet;
				Experiment01::barkOrMel = barkOrMel;
			}

			/**
			 * Analityc function which performs an wavelet transform
			 * of the value and calculate the energies based on MEL
			 * or BARK intervals
			 * @param signal
			 * @param signalLength
			 * @param samplingRate
			 * @param path
			 */
			static void waveletAnaliticFunction(std::vector<double> &signal, int &signalLength, unsigned int samplingRate, std::string path) {

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

				// Does the transformations
				wavelets::WaveletTransformResults transformedSignal = wavelets::malat(signal, Experiment01::wavelet, Experiment01::mode, level);

				///////////////////////////
				/// MEL or BARK section ///
				///////////////////////////

				std::vector<double> scaleRanges;

				if (Experiment01::barkOrMel == MEL) {
					// Ranges for MEL scale
					scaleRanges = { 20, 160, 394, 670, 1000, 1420, 1900, 2450, 3120, 4000, 5100, 6600, 9000, 14000 };
				} else if (Experiment01::barkOrMel == BARK) {
					// Ranges for BARK scale
					scaleRanges = { 20, 100, 200, 300, 400, 510, 630, 770, 920, 1080, 1270, 1480, 1720, 2000, 2320, 2700, 3150, 3700, 4400, 5300, 6400, 7700, 9500, 12000, 15500 };
				}

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
	};

	std::vector<std::string> explode(std::string str, std::string delimiter) {
		std::vector<std::string> res;

		size_t pos = 0;
		std::string token;
		while ((pos = str.find(delimiter)) != std::string::npos) {
			token = str.substr(0, pos);
			if (token.compare("") != 0) {
				res.push_back(token);
			}
			str.erase(0, pos + delimiter.length());
		}

		res.push_back(str);

		return res;
	}

	/**
	 * Plot the results
	 * @param data
	 */
	void plotFeatureVector(std::vector<double> signal, BARK_MEL bm, std::string waveletName, std::string filePath) {

		// Plot title
		std::string plotTitle;

		plotTitle = (bm == BARK ? "BARK <" : "MEL <") + waveletName + "> - ";

		std::vector<std::string> parts = explode(filePath, "/");
		plotTitle += parts.at(parts.size() - 4) + " - [" + parts.at(parts.size() - 2) + "] - " + parts.at(parts.size() - 1);

		// Alias for a easier use of matplotlib
		namespace plt = matplotlibcpp;

		plt::xlim(0, int(signal.size()));
		plt::title(plotTitle);

		plt::plot(signal, "r-");
		plt::show();
		plt::pause(.1);
	}

	/**
	 * Save the results
	 * @param data
	 */
	void saveDataToFile(std::vector<double> signal, BARK_MEL bm, std::string waveletName, std::string filePath) {

		std::vector<std::string> parts = explode(filePath, "/");
		std::string barOrMel = (bm == BARK ? "BARK" : "MEL");
		std::string fileName = parts.at(parts.size() - 1);
		std::string digit = parts.at(parts.size() - 2);
		std::string liveOrPlayback = parts.at(parts.size() - 4);

		filePath = "/tmp/" + barOrMel + "_results.csv";
		std::ofstream ofs(filePath, std::ios::app | std::ios::out);
		if (!ofs.is_open()) {
			std::cout << "Cannot open file: " << filePath;
			throw std::runtime_error("Impossible to open the file!");
			return;
		}
		ofs << waveletName + '\t' + barOrMel + '\t' + liveOrPlayback + '\t' + fileName + '\t' + digit + '\t';
		for (unsigned int i = 0; i < signal.size(); i++) {
			ofs << std::to_string(signal.at(i)) << '\t';
		}
		ofs << std::endl;
		ofs.close();
	}

	void perform(char *args[]) {
		std::cout << std::fixed;
		std::cout << std::setprecision(20);

		std::ifstream fileListStream;
		fileListStream.open(args[1], std::ios::in);

		// set the callback function in the Experiment01 class
		Wav w;
		w.setCallbackFunction(Experiment01::waveletAnaliticFunction);

		// store the file path to be processed
		std::string line;

		// iterates over all wavelets types
		for (std::pair<std::string, std::vector<double>> v : wavelets::all()) {

			// Iterates over all barkOrMel
			for (int bm = BARK; bm <= MEL; bm++) {

				// clear fail and eof bits
				fileListStream.clear();
				// back to the start!
				fileListStream.seekg(0, std::ios::beg);

				// gets the file path to process
				while (std::getline(fileListStream, line)) {

					// set current wavelet and barkOrMel to the experiment
					Experiment01::init(v.second, wavelets::PACKET_WAVELET, static_cast<BARK_MEL>(bm));

					// lines that begins with # are going to be ignored
					if (line.find("#") == 0) continue;

					w.read(line.data());
					w.process();

					saveDataToFile(w.getData(), static_cast<BARK_MEL>(bm), v.first, line);
					//plotFeatureVector(w.getData(), static_cast<BARK_MEL>(bm), v.first, line);
				}
			}
		}

	}

}
