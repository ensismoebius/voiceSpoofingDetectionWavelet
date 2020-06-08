/**
 * @author André Furlan
 * @email ensismoebius@gmail.com
 * This whole project are under GPLv3, for
 * more information read the license file
 *
 * 5 de mai de 2020
 *
 */

#ifndef SRC_WAVELETEXPERIMENTS_06_EXPERIMENT06_CPP_
#define SRC_WAVELETEXPERIMENTS_06_EXPERIMENT06_CPP_

#include <cmath>
#include <string>
#include <vector>
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
#include "../../lib/classifiers/NeuralNetwork.cpp"
#include "../../lib/classifiers/featureVectorsUtils.h"
#include "../../lib/wavelet/WaveletTransformResults.cpp"

namespace waveletExperiments {

	/**
	 * Contains the code for experiment 06.
	 */
	class Experiment06 {
		private:

			/**
			 * Used to define when MEL or BARK is used
			 */
			enum BARK_MEL {
				BARK, MEL
			};

			enum CONFUSION_POS {
				TP, FP, FN, TN
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
				Experiment06::wavelet = wavelets::get("haar");
				Experiment06::barkRanges = { 20, 100, 200, 300, 400, 510, 630, 770, 920, 1080, 1270, 1480, 1720, 2000, 2320, 2700, 3150, 3700, 4400, 5300, 6400, 7700, 9500, 12000, 15500 };
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
				// transformation we will perform a BARK composition
				int level = std::log2(signal.size());

				// Does the transformations
				wavelets::WaveletTransformResults transformedSignal = wavelets::malat(signal, Experiment06::wavelet, wavelets::PACKET_WAVELET, level);

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

			/**
			 * Plot the results on a paraconsistent plane
			 * @param results
			 */
			static void savePlotResults(std::vector<double> &numberOfTests, std::vector<double> &bestTestAccuracy, std::vector<double> &worseTestAccuracy, double pencentageSizeOfModel, std::string &resultsDestiny, double yrange[2]) {

				// Alias for a easier use of matplotlib
				namespace plt = matplotlibcpp;

				plt::xlabel("Amount of tests");
				plt::ylabel("Accuracy");

				plt::ylim(yrange[0], yrange[1]);

				plt::title("Accuracy of BARK over Haar wavelet using 3 layer neural network classifier\n with model size of " + std::to_string(int(pencentageSizeOfModel * 100)) + "% of total data");

				plt::named_plot("Best accuracy", numberOfTests, bestTestAccuracy, "-");
				plt::named_plot("Worst accuracy", numberOfTests, worseTestAccuracy, "--");

				plt::text(numberOfTests[numberOfTests.size() - 1], bestTestAccuracy[bestTestAccuracy.size() - 1], std::to_string(bestTestAccuracy[bestTestAccuracy.size() - 1]));
				plt::text(numberOfTests[numberOfTests.size() - 1], worseTestAccuracy[worseTestAccuracy.size() - 1], std::to_string(worseTestAccuracy[worseTestAccuracy.size() - 1]));

				plt::grid(true);
				plt::legend();
				plt::save(resultsDestiny + "/classifier_NeuralNetwork_" + std::to_string(int(pencentageSizeOfModel * 100)) + ".png");
				plt::clf();
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

			static void saveConfusionMatrices(std::map<CONFUSION_POS, int> &bestMatrix, std::map<CONFUSION_POS, int> &worstMatrix, double pencentageSizeOfModel, std::string &resultsDestiny) {

				// Open the file
				std::string filePath = resultsDestiny + "/classifier_NeuralNetwork_" + std::to_string(int(pencentageSizeOfModel * 100)) + ".csv";
				std::ofstream ofs(filePath, std::ios::app | std::ios::out);
				if (!ofs.is_open()) {
					std::cout << "Cannot open file: " << filePath;
					throw std::runtime_error("Impossible to open the file!");
					return;
				}

				ofs << "Best confusion matrix" << "\n" << std::to_string(bestMatrix[TP]) << "\t" << std::to_string(bestMatrix[FP]) << "\n" << std::to_string(bestMatrix[FN]) << "\t" << std::to_string(bestMatrix[TN]) << std::endl;
				ofs << "Worst confusion matrix" << "\n" << std::to_string(worstMatrix[TP]) << "\t" << std::to_string(worstMatrix[FP]) << "\n" << std::to_string(worstMatrix[FN]) << "\t" << std::to_string(worstMatrix[TN]) << std::endl;
				ofs.close();
			}

			/**
			 * Perform the experiment
			 * @param args - A list of wavefiles of the same class (ignore the first one)
			 * @param argCount - The amount of these files
			 */
			static void perform(std::vector<std::string> classFilesList, std::string resultsDestiny, unsigned int amountOfTestsToPerfom, double minModel, double maxModel) {

				classifiers::NeuralNetwork nn(2, 1, 0.1, classifiers::NeuralNetwork::leakyRelu, classifiers::NeuralNetwork::dleakyRelu);

				nn.addHiddenLayer(3);

				std::vector<double> i = { 1, 0 };
				std::vector<double> t = { 1 };

				nn.train(i, t);

				std::vector<double> o = nn.feedForward(i);

				nn.showLayers();

//				std::cout << std::fixed;
//				std::cout << std::setprecision(4);
//
//				// set the callback function in the Experiment06 class
//				Wav w;
//				w.setCallbackFunction(Experiment06::waveletAnaliticFunction);
//
//				// store the file path to be processed
//				std::string line;
//
//				/**
//				 * A data estructure witch will hold the wavelet transformed signals
//				 * haar
//				 * 	BARK
//				 * 		Class1
//				 * 			featureVector01
//				 * 			featureVector02
//				 * 			featureVector03
//				 * 			etc.
//				 * 		Class2
//				 * 			featureVector01
//				 * 			featureVector02
//				 * 			featureVector03
//				 * 			etc.
//				 * 		Etc.
//				 */
//				std::map<std::string, std::map<BARK_MEL, std::map<std::string, std::vector<std::vector<double>>>>> results;
//
//				////////////////////////////////////////
//				/// Preparing to compute the progress //
//				////////////////////////////////////////
//				double cycles = 0;
//				double totalCycles = 0;
//
//				// Computes the cicles needed to compute all signals
//				for (unsigned int i = 0; i < classFilesList.size(); i++) {
//					std::ifstream fileListStream;
//					fileListStream.open(classFilesList[i], std::ios::out);
//
//					while (std::getline(fileListStream, line))
//						totalCycles++;
//
//					fileListStream.clear();
//					fileListStream.close();
//				}
//				totalCycles = (classFilesList.size() - 1) * totalCycles;
//
//				//////////////////////////////////////////////////
//				/// Processing data with wavelet haar and BARK ///
//				//////////////////////////////////////////////////
//
//				// Iterates over all files, this files
//				// have to represent our data classes
//				for (unsigned int i = 0; i < classFilesList.size(); i++) {
//
//					// file reader
//					std::ifstream fileListStream;
//					fileListStream.open(classFilesList[i], std::ios::in);
//
//					// gets the file path to process
//					while (std::getline(fileListStream, line)) {
//
//						// Status
//						cycles++;
//						std::cout << "\rPreparing feature vectors... " << (cycles / totalCycles) * 100 << "%" << std::flush;
//
//						// Initializes the experiment
//						Experiment06::init();
//
//						// lines that begins with # are going to be ignored
//						if (line.find("#") == 0) continue;
//
//						// Read the files and process it
//						w.read(line.data());
//						w.process();
//
//						// Store the parcial results
//						results["haar"][BARK][classFilesList[i]].push_back(w.getData());
//					}
//
//					fileListStream.clear();
//					fileListStream.close();
//				}
//
//				////////////////////////////
//				/// Classification phase ///
//				////////////////////////////
//
//				// Start a new line to give space
//				// to the next resports
//				std::cout << std::endl;
//
//				// Creating the confusion matrix structure
//				std::map<CONFUSION_POS, int> confusionMatrix;
//
//				// Holds the values for the graphic
//				// of accuracy versus the number of
//				// tests
//				std::map<double, std::vector<double>> numberOfTests;
//				std::map<double, std::vector<double>> bestTestAccuracy;
//				std::map<double, std::vector<double>> worseTestAccuracy;
//				std::map<double, std::map<CONFUSION_POS, int>> bestConfusionMatrix;
//				std::map<double, std::map<CONFUSION_POS, int>> worseConfusionMatrix;
//
//				// Holds the parcial user friendly reports
//				std::string partialReport;
//
//				// Used to calculate the accuracies
//				double temp;
//
//				// Used to calculate the worst accuracy of a percentage
//				double percentageWorstAccuracy;
//
//				// Used to calculate the best accuracy of a percentage
//				double percentageBestAccuracy;
//
//				// Stores the best and the worst confusion matrix
//				std::map<CONFUSION_POS, int> percentageBestConfusionMatrix;
//				std::map<CONFUSION_POS, int> percentageWorseConfusionMatrix;
//
//				// Holds the tests features vectors for live signals
//				std::vector<std::vector<double>> testLive;
//
//				// Holds the models features vectors for live signals
//				std::vector<std::vector<double>> modelLive;
//
//				// Holds the tests features vectors for spoofing signals
//				std::vector<std::vector<double>> testSpoofing;
//
//				// Holds the models features vectors for spoofing signals
//				std::vector<std::vector<double>> modelSpoofing;
//
//				// Creating the classifiers
//				classifiers::NeuralNetwork c(Experiment06::barkRanges.size() - 1, 2, 0.01, classifiers::NeuralNetwork::sigmoid, classifiers::NeuralNetwork::dsigmoid);
//
//				// Clearing the results for the next iteration
//				numberOfTests.clear();
//				bestTestAccuracy.clear();
//				worseTestAccuracy.clear();
//				bestConfusionMatrix.clear();
//				worseConfusionMatrix.clear();
//
//				// Changes the percentage of the feature vectors used as models for the classifier
//				for (double modelPercentage = maxModel; modelPercentage >= minModel; modelPercentage -= .1) {
//
//					c.resetNetwork();
//
//					// Preparing network for the next iteration
//					std::cout << "Next iteration!" << std::endl;
//
//					// Initializing the accuracies
//					percentageBestAccuracy = -std::numeric_limits<double>().max();
//					percentageWorstAccuracy = std::numeric_limits<double>().max();
//
//					// Used for trainning
//					std::vector<double> target(2);
//
//					// Used for classification
//					std::vector<double> guess(2);
//
//					// Changes the amount of tests done against the dataset
//					for (unsigned int amountOfTests = 1; amountOfTests < amountOfTestsToPerfom + 1; amountOfTests++) {
//
//						// Do the classification and populate the confusion matrix
//						for (unsigned int k = 0; k < amountOfTests; k++) {
//
//							// Sampling the live signals
//							classifiers::raflleFeaturesVectors(results["haar"][BARK][classFilesList[0]], modelLive, testLive, modelPercentage);
//							// Sampling the spoofing signals
//							classifiers::raflleFeaturesVectors(results["haar"][BARK][classFilesList[1]], modelSpoofing, testSpoofing, modelPercentage);
//
//							// Trainning
//							target[0] = 1;
//							target[1] = 0;
//							for (auto sample : modelLive) {
//								c.train(sample, target);
//							}
//
//							target[0] = 0;
//							target[1] = 1;
//							for (auto sample : modelSpoofing) {
//								c.train(sample, target);
//							}
//
//						}
//
//						// Preparing confusion matrix
//						confusionMatrix[TP] = 0;
//						confusionMatrix[FP] = 0;
//						confusionMatrix[TN] = 0;
//						confusionMatrix[FN] = 0;
//
//						// Test it out!!
//						for (auto test : testLive) {
//
//							guess = c.feedForward(test);
//
//							if (guess[0] > guess[1]) {
//								confusionMatrix[TP] += 1;
//							} else {
//								confusionMatrix[FN] += 1;
//							}
//						}
//
//						for (auto test : testSpoofing) {
//							guess = c.feedForward(test);
//
//							if (guess[0] < guess[1]) {
//								confusionMatrix[TN] += 1;
//							} else {
//								confusionMatrix[FP] += 1;
//							}
//						}
//
//						////////////////////////
//						/// Conclusion phase ///
//						////////////////////////
//
//						// calculate the best accuracy
//						temp = double(confusionMatrix[TP] + confusionMatrix[TN]) / double(testLive.size() + testSpoofing.size());
//
//						if (temp > percentageBestAccuracy) {
//							percentageBestAccuracy = temp;
//							percentageBestConfusionMatrix = confusionMatrix;
//							partialReport = std::to_string(percentageBestAccuracy) + "\t" + std::to_string(confusionMatrix[TP]) + "\t" + std::to_string(confusionMatrix[FP]) + "\t";
//							partialReport += std::to_string(confusionMatrix[FN]) + "\t" + std::to_string(confusionMatrix[TN]);
//						}
//
//						if (temp < percentageWorstAccuracy) {
//							percentageWorstAccuracy = temp;
//							percentageWorseConfusionMatrix = confusionMatrix;
//							partialReport = std::to_string(percentageBestAccuracy) + "\t" + std::to_string(confusionMatrix[TP]) + "\t" + std::to_string(confusionMatrix[FP]) + "\t";
//							partialReport += std::to_string(confusionMatrix[FN]) + "\t" + std::to_string(confusionMatrix[TN]);
//						}
//
//						// Store the results for the graphic
//						numberOfTests[modelPercentage].push_back(amountOfTests);
//						bestTestAccuracy[modelPercentage].push_back(percentageBestAccuracy);
//						worseTestAccuracy[modelPercentage].push_back(percentageWorstAccuracy);
//						bestConfusionMatrix[modelPercentage] = percentageBestConfusionMatrix;
//						worseConfusionMatrix[modelPercentage] = percentageWorseConfusionMatrix;
//
//						// Report for this amount of tests
//						std::cout << modelPercentage * 100 << "%\t" << amountOfTests << "\t" << partialReport << std::endl;
//					}
//				}
//
//				// Calculates the range of y axis
//				// for a more regular ploting
//				double yrange[2] = { 1, 0 };
//				for (auto test : numberOfTests) {
//
//					for (double v : worseTestAccuracy[test.first]) {
//						yrange[0] = v < yrange[0] ? v : yrange[0];
//					}
//					for (double v : bestTestAccuracy[test.first]) {
//						yrange[1] = v > yrange[1] ? v : yrange[1];
//					}
//
//					float temp = (yrange[1] - yrange[0]) / 30;
//					yrange[0] -= temp;
//					yrange[1] += temp;
//
//				}
//
//				// Plot everything
//				for (auto test : numberOfTests) {
//					saveConfusionMatrices(bestConfusionMatrix[test.first], worseConfusionMatrix[test.first], test.first, resultsDestiny);
//					savePlotResults(test.second, bestTestAccuracy[test.first], worseTestAccuracy[test.first], test.first, resultsDestiny, yrange);
//				}

			}
	};

}
#endif /* SRC_WAVELETEXPERIMENTS_06_EXPERIMENT06_CPP_ */
