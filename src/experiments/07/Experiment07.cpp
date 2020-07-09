/**
 * @author André Furlan
 * @email ensismoebius@gmail.com
 * This whole project are under GPLv3, for
 * more information read the license file
 *
 * 8 de jul. de 2020
 *
 */

#ifndef SRC_WAVELETEXPERIMENTS_07_EXPERIMENT07_CPP_
#define SRC_WAVELETEXPERIMENTS_07_EXPERIMENT07_CPP_

#include <cmath>
#include <string>
#include <vector>
#include <iomanip>
#include <iostream>
#include <algorithm>

#include "../../lib/wave/Wav.cpp"
#include "../../lib/wavelet/Types.h"
#include "../../lib/vector/vectorUtils.h"
#include "../../lib/gnuplot/gnuplotCalls.h"
#include "../../lib/wavelet/waveletOperations.h"
#include "../../lib/statistics/confusionMatrix.h"
#include "../../lib/linearAlgebra/linearAlgebra.h"
#include "../../lib/matplotlib-cpp/matplotlibcpp.h"
#include "../../lib/paraconsistent/paraconsistent.h"
#include "../../lib/classifiers/featureVectorsUtils.h"
#include "../../lib/classifiers/DistanceClassifier.cpp"
#include "../../lib/wavelet/WaveletTransformResults.cpp"

namespace waveletExperiments {

	/**
	 * Contains the code for experiment 07.
	 */
	class Experiment07 {
		private:

			/**
			 * Used to define when MEL or BARK is used
			 */
			enum BARK_MEL {
				BARK, MEL
			};

			/**
			 * Contains the BARK ranges values
			 */
			static inline std::vector<double> barkRanges;

			/**
			 * Wavelet waveform function
			 */
			static inline std::vector<double> wavelet;

		public:

			/**
			 * Initialises the experiment
			 */
			static void init() {
				wavelets::init( { "haar" });
				Experiment07::wavelet = wavelets::get("haar");
				Experiment07::barkRanges = { 20, 100, 200, 300, 400, 510, 630, 770, 920, 1080, 1270, 1480, 1720, 2000, 2320, 2700, 3150, 3700, 4400, 5300, 6400, 7700, 9500, 12000, 15500 };
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

				// Calculate the max levels of decomposi				std::sort(far);tions
				// i.e. until the coeficients are formed by
				// just single numbers.
				// This is needed because at the end of the
				// transformation we will perform a BARK composition
				int level = std::log2(signal.size());

				// Does the transformations
				wavelets::WaveletTransformResults transformedSignal = wavelets::malat(signal, Experiment07::wavelet, wavelets::PACKET_WAVELET, level);

				////////////////////
				/// BARK section ///
				////////////////////

				// feature vector has the amount of values minus 1 than ranges
				// because we are summing up intervals
				std::vector<double> featureVector(barkRanges.size() - 1);

				// We need to known the max frequency supported
				// by the signal in order to find the values in
				// which the sums of the BARK scale will be
				// performed
				double maxFrequency = samplingRate / 2;

				// Calculate the minimum frequency range which
				// will enable the correct interval sums to
				// be performed
				double frequencyChunckSize = maxFrequency / transformedSignal.getWaveletPacketAmountOfParts();

				// Used to retrieve the interval for the sums
				double rangeScaleEnd = 0;
				double rangeScaleStart = 0;

				// Loop over all the ranges and calculate the energies inside it
				for (unsigned int i = 0; i < barkRanges.size() - 1; i++) {

					// Retrieve the interval for the sums
					rangeScaleStart = barkRanges.at(i);
					rangeScaleEnd = barkRanges.at(i + 1);

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

				// Replaces the original signal
				signal = featureVector;
			}

			static void savePlotResults(std::vector<statistics::ConfusionMatrix> confusionMatrices, double percentage, classifiers::DistanceClassifier::DISTANCE_TYPE distanceType, std::string destiny) {

				// Alias for a easier use of matplotlib
				namespace plt = matplotlibcpp;

				std::string distType = distanceType == classifiers::DistanceClassifier::MANHATTAN ? "Manhattan" : "Euclidian";

				plt::xlabel("Amount of tests");
				plt::ylabel("Accuracy");

				plt::title("Detection Error Tradeoff curve and EER for " + distType + " and " + std::to_string(percentage));

				std::vector<double> far;
				std::vector<double> frr;
				std::vector<double> x;

				int xi = 0;
				for (auto confusionMatrix : confusionMatrices) {
					far.push_back(statistics::falsePositiveRate(confusionMatrix));
					frr.push_back(statistics::falseNegativeRate(confusionMatrix));
					x.push_back(xi++);
				}

				std::sort(far.rbegin(), far.rend());
				std::sort(frr.begin(), frr.end());

//				plt::named_plot("far", far);
//				plt::named_plot("frr", frr);
				plt::named_plot("det", far, frr);
				plt::plot(x, x);

				plt::xlim(0.0, 0.5);
				plt::ylim(0.0, 0.5);

				plt::grid(true);
				plt::legend();
				plt::show();
			}

			/**
			 * Save the results to file on /tmp/results.csv
			 * @param data
			 */

			static void saveDataToFile(std::map<std::string, std::map<BARK_MEL, std::map<std::string, std::vector<std::vector<double>>>>> data) {

				// Open the file
				std::string filePath = "/tmp/results.csv";
				std::ofstream ofs(filePath, std::ios::app | std::ios::out);
				if (!ofs.is_open()) {
					std::cout << "Cannot open file: " << filePath;
					throw std::runtime_error("Impossible to open the file!");
					return;
				}

				for (auto clazz : data["haar"][BARK]) {
					for (std::vector<double> featureVector : clazz.second) {
						ofs << clazz.first << '\t';
						for (double value : featureVector) {
							ofs << value << '\t';
						}
						ofs << std::endl;
					}
				}
				ofs.close();
			}

			/**
			 * Perform the experiment
			 * @param args - A list of wavefiles of the same class (ignore the first one)
			 * @param argCount - The amount of these files
			 */
			static void perform(std::vector<std::string> classFilesList, std::string resultsDestiny, unsigned int amountOfTestsToPerfom, double minModel, double maxModel) {
				std::cout << std::fixed;
				std::cout << std::setprecision(4);

				// set the callback function in the Experiment07 class
				Wav w;
				w.setCallbackFunction(Experiment07::waveletAnaliticFunction);

				// store the file path to be processed
				std::string line;

				/**
				 * A data estructure witch will hold the wavelet transformed signals
				 * haar
				 * 	BARK
				 * 		Class1
				 * 			featureVector01
				 * 			featureVector02
				 * 			featureVector03
				 * 			etc.
				 * 		Class2
				 * 			featureVector01
				 * 			featureVector02
				 * 			featureVector03
				 * 			etc.
				 * 		Etc.
				 */
				std::map<std::string, std::map<BARK_MEL, std::map<std::string, std::vector<std::vector<double>>>>> results;

				////////////////////////////////////////
				/// Preparing to compute the progress //
				////////////////////////////////////////
				double cycles = 0;
				double totalCycles = 0;

				// Computes the cicles needed to compute all signals
				for (unsigned int i = 0; i < classFilesList.size(); i++) {
					std::ifstream fileListStream;
					fileListStream.open(classFilesList[i], std::ios::out);

					while (std::getline(fileListStream, line))
						totalCycles++;

					fileListStream.clear();
					fileListStream.close();
				}
				totalCycles = (classFilesList.size() - 1) * totalCycles;

				//////////////////////////////////////////////////
				/// Processing data with wavelet haar and BARK ///
				//////////////////////////////////////////////////

				// Iterates over all files, this files
				// have to represent our data classes
				for (unsigned int i = 0; i < classFilesList.size(); i++) {

					// file reader
					std::ifstream fileListStream;
					fileListStream.open(classFilesList[i], std::ios::in);

					// gets the file path to process
					while (std::getline(fileListStream, line)) {

						// Status
						cycles++;
						std::cout << "\rPreparing feature vectors... " << (cycles / totalCycles) * 100 << "%" << std::flush;

						// Initializes the experiment
						Experiment07::init();

						// lines that begins with # are going to be ignored
						if (line.find("#") == 0) continue;

						// Read the files and process it
						w.read(line.data());
						w.process();

						// Store the parcial results
						results["haar"][BARK][classFilesList[i]].push_back(w.getData());
					}

					fileListStream.clear();
					fileListStream.close();
				}

				////////////////////////////
				/// Classification phase ///
				////////////////////////////

				// Start a new line to give space
				// to the next reports
				std::cout << std::endl;

				// Creating the confusion matrix structure
				statistics::ConfusionMatrix confusionMatrix;

				std::map<double, std::vector<statistics::ConfusionMatrix>> confusionMatricesForEachPercentage;

				// Holds the tests features vectors for live signals
				std::vector<std::vector<double>> testLive;

				// Holds the models features vectors for live signals
				std::vector<std::vector<double>> modelLive;

				// Holds the tests features vectors for spoofing signals
				std::vector<std::vector<double>> testSpoofing;

				// Holds the models features vectors for spoofing signals
				std::vector<std::vector<double>> modelSpoofing;

				// Creating the classifier
				classifiers::DistanceClassifier c;

				// Changes the type of distance classifier used
				for (int distClassifierType = classifiers::DistanceClassifier::EUCLICIDIAN; distClassifierType <= classifiers::DistanceClassifier::MANHATTAN; distClassifierType++) {

					// Changes the percentage of the feature vectors used as models for the classifier
					for (double modelPercentage = maxModel; modelPercentage >= minModel; modelPercentage -= .1) {

						// Changes the amount of tests done against the dataset
						confusionMatricesForEachPercentage[modelPercentage].resize(amountOfTestsToPerfom);
						for (unsigned int testIndex = 0; testIndex < amountOfTestsToPerfom; testIndex++) {

							// Sampling the live signals
							classifiers::raflleFeaturesVectors(results["haar"][BARK][classFilesList[0]], modelLive, testLive, modelPercentage);
							// Sampling the spoofing signals
							classifiers::raflleFeaturesVectors(results["haar"][BARK][classFilesList[1]], modelSpoofing, testSpoofing, modelPercentage);

							// Setting up the classifier
							c.setDistanceType(static_cast<classifiers::DistanceClassifier::DISTANCE_TYPE>(distClassifierType));
							c.addReferenceModels("live", modelLive);
							c.addReferenceModels("spoofing", modelSpoofing);

							// Preparing confusion matrix
							confusionMatrix.truePositive = 0;
							confusionMatrix.falsePositive = 0;
							confusionMatrix.trueNegative = 0;
							confusionMatrix.falseNegative = 0;

							// Test it out!!
							for (auto test : testLive) {
								if (c.classify(test).compare("live") == 0) {
									confusionMatrix.truePositive += 1;
								} else {
									confusionMatrix.falseNegative += 1;
								}
							}

							for (auto test : testSpoofing) {
								if (c.classify(test).compare("spoofing") == 0) {
									confusionMatrix.trueNegative += 1;
								} else {
									confusionMatrix.falsePositive += 1;
								}
							}

							confusionMatricesForEachPercentage[modelPercentage][testIndex] = confusionMatrix;

							std::cout << "\rPreparing DET curve... " << 100 * testIndex / amountOfTestsToPerfom << "%" << std::flush;
						}
						// Report for this amount of tests
						std::cout << std::endl << (static_cast<classifiers::DistanceClassifier::DISTANCE_TYPE>(distClassifierType) == classifiers::DistanceClassifier::MANHATTAN ? "Manhattan: " : "Euclidian: ") << modelPercentage * 100 << "%\t" << std::endl;
					}

					// Plot everything
					for (auto confusionMatrices : confusionMatricesForEachPercentage) {
						savePlotResults(confusionMatrices.second, confusionMatrices.first, static_cast<classifiers::DistanceClassifier::DISTANCE_TYPE>(distClassifierType), resultsDestiny);
					}
				}

			}
	};

}
#endif /* SRC_WAVELETEXPERIMENTS_07_EXPERIMENT02_CPP_ */
