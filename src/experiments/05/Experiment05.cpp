/**
 * @author André Furlan
 * @email ensismoebius@gmail.com
 * This whole project are under GPLv3, for
 * more information read the license file
 *
 * 2 de jun de 2020
 *
 */

#ifndef SRC_WAVELETEXPERIMENTS_05_EXPERIMENT05_CPP_
#define SRC_WAVELETEXPERIMENTS_05_EXPERIMENT05_CPP_

#include <cmath>
#include <string>
#include <vector>
#include <future>
#include <iomanip>
#include <iostream>

#include "../../lib/wave/Wav.cpp"
#include "../../lib/wavelet/Types.h"
#include "../../lib/vector/vectorUtils.h"
#include "../../lib/gnuplot/gnuplotCalls.h"
#include "../../lib/wavelet/waveletOperations.h"
#include "../../lib/linearAlgebra/linearAlgebra.h"
#include "../../lib/matplotlib-cpp/matplotlibcpp.h"
#include "../../lib/paraconsistent/paraconsistent.h"
#include "../../lib/wavelet/WaveletTransformResults.cpp"

namespace waveletExperiments {

	/**
	 * Contains the code for experiment 05.
	 */
	class Experiment05 {
		private:

			/**
			 * Used to define when MEL or BARK is used
			 */
			enum BARK_MEL {
				BARK, MEL
			};

			/**
			 * Contains the MEL ranges values
			 */
			static inline std::vector<double> MELRanges;

			/**
			 * Contains the BARK ranges values
			 */
			static inline std::vector<double> BARKRanges;

			/**
			 * Contains the paths to the lists of files
			 */
			static inline std::vector<std::string> classFileList;

			/**
			 * Contains the final results
			 */
			static inline std::map<std::string, std::map<BARK_MEL, std::vector<std::vector<double>>>> finalResults;

			/**
			 * Flag indicating if we are processing bark or mel
			 */
			static inline BARK_MEL barkOrMel;

			/**
			 * Wavelet waveform function
			 */
			static inline std::vector<double> wavelet;

			/**
			 * Wavelet type
			 */
			static inline wavelets::TransformMode mode;
		public:

			/**
			 * Initialises the experiment
			 * @param wavelet
			 * @param mode
			 * @param barkOrMel
			 */
			static void init(std::vector<double> wavelet, wavelets::TransformMode mode, BARK_MEL barkOrMel) {
				Experiment05::mode = mode;
				Experiment05::wavelet = wavelet;
				Experiment05::barkOrMel = barkOrMel;
				Experiment05::MELRanges = { 20, 160, 394, 670, 1000, 1420, 1900, 2450, 3120, 4000, 5100, 6600, 9000, 14000 };
				Experiment05::BARKRanges = { 20, 100, 200, 300, 400, 510, 630, 770, 920, 1080, 1270, 1480, 1720, 2000, 2320, 2700, 3150, 3700, 4400, 5300, 6400, 7700, 9500, 12000, 15500 };
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
				// BARK composition
				int level = std::log2(signal.size());

				// Does the transformations
				wavelets::WaveletTransformResults transformedSignal = wavelets::malat(signal, Experiment05::wavelet, Experiment05::mode, level);

				///////////////////////////
				/// MEL or BARK section ///
				///////////////////////////

				// Contains the values of the MEL or BARK ranges
				std::vector<double> scaleRanges;

				if (Experiment05::barkOrMel == MEL) {
					// Ranges for MEL scale
					scaleRanges = MELRanges;
				} else {
					// Ranges for BARK scale
					scaleRanges = BARKRanges;
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

						featureVector.at(i) = featureVector.at(i) == 0 ? 0 : std::log(featureVector.at(i));
					}

				}

				// Normalize the resulting feature vector
				linearAlgebra::normalizeVector(featureVector);

				// Just for MEL
				if (Experiment05::barkOrMel == MEL) {
					// Apply a DCT (Discrete Cosine Transform)
					linearAlgebra::discreteCosineTransform(featureVector);

					// For the sake of this experiment we need to known MEL values without derivation
					// Takes the double derivative of the features vector
					// linearAlgebra::derivative(featureVector, 2);
				}

				// Replaces the original signal
				signal = featureVector;
			}

			/**
			 * Plot the results on a paraconsistent plane
			 * @param results
			 */
			static void plotResults(std::map<std::string, std::map<BARK_MEL, std::map<std::string, std::vector<std::vector<double>>>>> results, std::string resultsDestiny) {

				// Alias for a easier use of matplotlib
				namespace plt = matplotlibcpp;

				// Prepare the labels for MEL scale
				std::vector<double> melTicks(MELRanges.size() - 1);
				std::vector<std::string> melLabels(MELRanges.size() - 1);
				for (unsigned int i = 0; i < MELRanges.size() - 1; i++) {
					melLabels[i] = std::to_string(int(MELRanges[i])) + "-" + std::to_string(int(MELRanges[i + 1]));
					melTicks[i] = i;
				}

				// Prepare the labels for BARK scale
				std::vector<double> barkTicks(BARKRanges.size() - 1);
				std::vector<std::string> barkLabels(BARKRanges.size() - 1);
				for (unsigned int i = 0; i < BARKRanges.size() - 1; i++) {
					barkLabels[i] = std::to_string(int(BARKRanges[i])) + "-" + std::to_string(int(BARKRanges[i + 1]));
					barkTicks[i] = i;
				}

				// Labels configurations
				std::map<std::string, std::string> parans;
				parans["rotation"] = "40";
				parans["rotation_mode"] = "anchor";
				parans["horizontalalignment"] = "right";

				std::map<std::string, std::string> parans2;
				parans2["color"] = "red";

				for (auto wavelets : results) {
					for (auto barkMel : wavelets.second) {

						// For each class we have one plot
						for (auto clazz : barkMel.second) {

							// Building the title
							std::string title;
							if (clazz.first.compare(Experiment05::classFileList[0]) == 0) {
								title = "Non spoofing ";
							} else {
								title = "Spoofing ";
							}
							title += wavelets.first + "-" + (barkMel.first == BARK ? "Bark" : "Mel");

							// Setting up ticks and labels
							if (static_cast<BARK_MEL>(barkMel.first) == BARK) {
								plt::xticks(barkTicks, barkLabels, parans);
							} else {
								plt::xticks(melTicks, melLabels, parans);
							}

							// Setting up the width and heigth of plot
							if (static_cast<BARK_MEL>(barkMel.first) == BARK) {
								plt::xlim(0, int(BARKRanges.size()));
							} else {
								plt::xlim(0, int(MELRanges.size()));
							}
							// This range was figured out empirically
							plt::ylim(-0.11, 0.16);

							// Plotting the values
							plt::title(title);
							for (auto vector : clazz.second) {
								if (static_cast<BARK_MEL>(barkMel.first) == BARK) {
									plt::scatter(barkTicks, vector, 8, parans2);
								} else {
									plt::scatter(melTicks, vector, 8, parans2);
								}
							}
							plt::tight_layout();
							plt::grid(true);
							plt::show();
						}
					}
				}
			}

			/**
			 * Perform the experiment
			 * @param args - A list of wavefiles of the same class (ignore the first one)
			 * @param args.size() - The amount of these files
			 */
			static void perform(std::vector<std::string> classFileList, std::string resultsDestiny) {
				//std::cout << std::fixed;
				//std::cout << std::setprecision(20);

				Experiment05::classFileList = classFileList;

				// set the callback function in the Experiment05 class
				Wav w;
				w.setCallbackFunction(Experiment05::waveletAnaliticFunction);

				// store the file path to be processed
				std::string line;

				/**
				 * A data estructure witch will hold the wavelet transformed signals
				 * wavelet1
				 * 	BARK
				 * 		Class1
				 * 			data1
				 * 			data2
				 * 			data3
				 * 			etc.
				 * 		Class2
				 * 			data1
				 * 			data2
				 * 			data3
				 * 			etc.
				 * 		Etc.
				 * 	MEL
				 * 		Class1
				 * 			data1
				 * 			data2
				 * 			data3
				 * 			etc.
				 * 		Class2
				 * 			data1
				 * 			data2
				 * 			data3
				 * 			etc.
				 * 		Etc.
				 * 	ETC.
				 * wavelet2
				 * 	...
				 */
				std::map<std::string, std::map<BARK_MEL, std::map<std::string, std::vector<std::vector<double>>>>> results;

				// Used to compute the progress
				double cycles = 0;
				double totalCycles = 0;

				// For this experiment we wil use just three wavelets
				wavelets::init( { "haar", "daub42", "daub54" });

				// Computes the cicles needed to compute all signals
				for (unsigned int i = 0; i < classFileList.size(); i++) {
					std::ifstream fileListStream;
					fileListStream.open(classFileList[i], std::ios::out);

					while (std::getline(fileListStream, line)) {
						// lines that begins with # are going to be ignored
						if (line.find("#") == 0) continue;
						totalCycles++;
					}

					fileListStream.clear();
					fileListStream.close();
				}
				totalCycles = wavelets::all().size() * classFileList.size() * totalCycles;

				// Iterates over all files, this files
				// have to represent our data classes
				for (unsigned int i = 0; i < classFileList.size(); i++) {

					// file reader
					std::ifstream fileListStream;
					fileListStream.open(classFileList[i], std::ios::in);

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

								// lines that begins with # are going to be ignored
								if (line.find("#") == 0) continue;

								// Status
								cycles++;
								std::cout << "\rCompletion " << (cycles / totalCycles) * 100 << "%" << std::flush;

								// set current wavelet and barkOrMel to the experiment
								Experiment05::init(v.second, wavelets::PACKET_WAVELET, static_cast<BARK_MEL>(bm));

								w.read(line.data());
								w.process();
								results[v.first][static_cast<BARK_MEL>(bm)][classFileList[i]].push_back(w.getData());
							}
						}
					}

					fileListStream.clear();
					fileListStream.close();
				}

				plotResults(results, resultsDestiny);
			}
	};
}

#endif /* SRC_WAVELETEXPERIMENTS_05_EXPERIMENT05_CPP_ */

