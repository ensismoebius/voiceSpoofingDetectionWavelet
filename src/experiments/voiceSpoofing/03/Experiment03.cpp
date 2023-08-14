/**
 * @author André Furlan
 * @email ensismoebius@gmail.com
 * This whole project are under GPLv3, for
 * more information read the license file
 *
 * 14 de mai de 2020
 *
 * Based on the results of experiment 01, which has selected
 * wavelet haar and BARK scale as the best features vector
 * generators, this experiment do some classifications using
 * SVM classifier generating the respectives confusion
 * matrices, standard deviations and accuracy measurements.
 *
 */

#ifndef SRC_WAVELETEXPERIMENTS_03_EXPERIMENT03_CPP_
#define SRC_WAVELETEXPERIMENTS_03_EXPERIMENT03_CPP_

#include <cmath>
#include <string>
#include <vector>
#include <iomanip>
#include <iostream>

#include "../../lib/wave/Wav.h"
#include "../../lib/wavelet/Types.h"
#include "../../lib/vector/vectorUtils.h"
#include "../../lib/gnuplot/gnuplotCalls.h"
#include "../../lib/statistics/statistics.h"
#include "../../lib/wavelet/waveletOperations.h"
#include "../../lib/statistics/confusionMatrix.h"
#include "../../lib/linearAlgebra/linearAlgebra.h"
#include "../../lib/matplotlib-cpp/matplotlibcpp.h"
#include "../../lib/paraconsistent/paraconsistent.h"
#include "../../lib/classifiers/featureVectorsUtils.h"
#include "../../lib/wavelet/WaveletTransformResults.h"
#include "../../lib/classifiers/SupportVectorMachine.h"

namespace waveletExperiments
{

	/**
	 * Contains the code for experiment 03.
	 */
	class Experiment03
	{
		private:

			/**
			 * Used to define when MEL or BARK is used
			 */
			enum MEL_BARK
			{
				MEL, BARK
			};

			/**
			 * Contains the BARK ranges values
			 */
			static inline std::vector<double> BARKRanges;

			/**
			 * Wavelet waveform function
			 */
			static inline std::vector<long double> wavelet;

		public:

			/**
			 * Initializes the experiment
			 */
			static void init()
			{
				wavelets::init( { "haar" });
				Experiment03::wavelet = wavelets::get("haar");
				Experiment03::BARKRanges = { 20, 100, 200, 300, 400, 510, 630, 770, 920, 1080, 1270, 1480, 1720, 2000, 2320, 2700, 3150, 3700, 4400, 5300, 6400, 7700, 9500, 12000, 15500 };
			}

			/**
			 * Analytic function which performs an wavelet transform
			 * of the value and calculate the energies based on BARK intervals
			 * @param signal
			 * @param signalLength
			 * @param samplingRate
			 * @param path
			 */
			static void waveletAnaliticFunction(std::vector<long double> &signal, int &signalLength, unsigned int samplingRate, std::string path)
			{

				///////////////////////
				/// Wavelet section ///
				///////////////////////

				// Normalizes the signal between -1 and 1
				linearAlgebra::normalizeVectorToRange(signal, -1, 1);

				// Expands the signal length to optimize the wavelet transform
				signalLength = wavelets::getNextPowerOfTwo(signal.size());
				linearAlgebra::resizeCentered(signal, signalLength);

				// Calculates the maximum levels of decompositions
				// i.e. until the coefficients are formed by
				// just single numbers.
				// This is needed because at the end of the
				// transformation we will perform a BARK composition
				int level = std::log2(signal.size());

				// Does the transformations
				wavelets::WaveletTransformResults transformedSignal = wavelets::malat(signal, Experiment03::wavelet, wavelets::PACKET_WAVELET, level);

				////////////////////
				/// BARK section ///
				////////////////////

				// features vector has the amount of values equals to amount of the ranges minus 1 
				// because we are summing up intervals
				std::vector<long double> featureVector(BARKRanges.size() - 1);

				// We need to known the max frequency supported
				// by the signal in order to find the values in
				// which the sums of the BARK scale will be
				// performed
				double maxFrequency = samplingRate / 2;

				// Used to retrieve the interval for the sums
				double rangeScaleEnd = 0;
				double rangeScaleStart = 0;

				// Loop over all the ranges and calculate the energies inside it
				for (unsigned int i = 0; i < BARKRanges.size() - 1; i++)
				{

					// Retrieves the interval for the sums
					rangeScaleStart = BARKRanges.at(i);
					rangeScaleEnd = BARKRanges.at(i + 1);

					// Retrieve the values
					std::vector<long double> sig1 = transformedSignal.getWaveletPacketTransforms(rangeScaleStart, rangeScaleEnd, maxFrequency);

					// Sum the power of 2 of them all!!! (i.e. calculate the energies)
					featureVector.at(i) = 0;
					for (double v : sig1)
					{
						featureVector.at(i) += std::pow(v, 2);
					}

				}

				// Normalizes the resulting features vector
				linearAlgebra::normalizeVectorToSum1AllPositive(featureVector);

				// Replaces the original signal
				signal = featureVector;
			}

			/**
			 * Plot the results
			 *
			 * @param numberOfTests
			 * @param bestTestAccuracy
			 * @param worseTestAccuracy
			 * @param stdDeviation
			 * @param pencentageSizeOfModel
			 * @param resultsDestiny
			 * @param yrange
			 */
			static void savePlotResults(std::vector<double> &numberOfTests, std::vector<double> &bestTestAccuracy, std::vector<double> &worseTestAccuracy, double stdDeviation, double pencentageSizeOfModel, std::string &resultsDestiny, double yrange[2])
			{

				// Alias for a easier use of matplotlib
				namespace plt = matplotlibcpp;

				plt::xlabel("Amount of tests");
				plt::ylabel("Accuracy");

				plt::ylim(yrange[0], yrange[1]);

				plt::title("Accuracy of BARK over haar wavelet using SVM classifier.\n Model size: " + std::to_string(int(pencentageSizeOfModel * 100)) + "% of total data. Standard deviation: " + std::to_string(stdDeviation));

				plt::named_plot("Best accuracy", numberOfTests, bestTestAccuracy, "-");
				plt::named_plot("Worst accuracy", numberOfTests, worseTestAccuracy, "--");

				plt::text(numberOfTests[numberOfTests.size() - 1], bestTestAccuracy[bestTestAccuracy.size() - 1], std::to_string(bestTestAccuracy[bestTestAccuracy.size() - 1]));
				plt::text(numberOfTests[numberOfTests.size() - 1], worseTestAccuracy[worseTestAccuracy.size() - 1], std::to_string(worseTestAccuracy[worseTestAccuracy.size() - 1]));

				plt::grid(true);
				plt::legend();
				plt::save(resultsDestiny + "/classifier_SVM_" + std::to_string(int(pencentageSizeOfModel * 100)) + ".pdf");
				plt::clf();
			}

			/**
			 * Save confusion matrices in @resultsDestiny
			 * @param bestMatrix
			 * @param worstMatrix
			 * @param pencentageSizeOfModel
			 * @param resultsDestiny
			 */
			static void saveConfusionMatrices(statistics::ConfusionMatrix &bestMatrix, statistics::ConfusionMatrix &worstMatrix, double pencentageSizeOfModel, std::string &resultsDestiny)
			{

				// Open the file
				std::string filePath = resultsDestiny + "/classifier_SVM_" + std::to_string(int(pencentageSizeOfModel * 100)) + ".tex";
				std::ofstream ofs(filePath, std::ios::app | std::ios::out);
				if (!ofs.is_open())
				{
					std::cout << "Cannot open file: " << filePath;
					throw std::runtime_error("Impossible to open the file!");
					return;
				}

				// Generate the latex code for confusion matrices
				ofs << "\\begin{table}[h] \
					\\newcommand{\\mc}[3]{\\multicolumn{#1}{#2}{#3}} \
					\\definecolor{tcB}{rgb}{0.447059,0.74902,0.266667} \
					\\definecolor{tcC}{rgb}{0,0,0} \
					\\definecolor{tcD}{rgb}{0,0.5,1} \
					\\definecolor{tcA}{rgb}{0.65098,0.65098,0.65098} \
					\\begin{center} \
						\\subfloat[Best confusion matrix]{ \
							\\begin{tabular}{ccc} \
								\\mc{1}{l}{} & \\mc{1}{>{\\columncolor{tcA}}c}{\\textbf{genuine}} & \\mc{1}{>{\\columncolor{tcA}}c}{\\textbf{spoofed}}\\\\ \
								\\mc{1}{>{\\columncolor{tcA}}r}{\\textbf{genuine}} & \\mc{1}{>{\\columncolor{tcB}}c}{\\textcolor{tcC}{" << std::to_string(bestMatrix.truePositive) << "}} & \\mc{1}{>{\\columncolor{tcD}}c}{\\textcolor{tcC}{" << std::to_string(bestMatrix.falsePositive) << "}}\\\\ \
								\\mc{1}{>{\\columncolor{tcA}}r}{\\textbf{spoofed}} & \\mc{1}{>{\\columncolor{tcD}}c}{\\textcolor{tcC}{" << std::to_string(bestMatrix.falseNegative) << "}} & \\mc{1}{>{\\columncolor{tcB}}c}{\\textcolor{tcC}{" << std::to_string(bestMatrix.trueNegative) << "}} \
							\\end{tabular} \
							\\label{tab:classifier_SVM_" << std::to_string(int(pencentageSizeOfModel * 100)) << "_best} \
						} \
						\\qquad \
						\\subfloat[Worst confusion matrix]{ \
							\\begin{tabular}{ccc} \
								\\mc{1}{l}{} & \\mc{1}{>{\\columncolor{tcA}}c}{\\textbf{genuine}} & \\mc{1}{>{\\columncolor{tcA}}c}{\\textbf{spoofed}}\\\\ \
								\\mc{1}{>{\\columncolor{tcA}}r}{\\textbf{genuine}} & \\mc{1}{>{\\columncolor{tcB}}c}{\\textcolor{tcC}{" << std::to_string(worstMatrix.truePositive) << "}} & \\mc{1}{>{\\columncolor{tcD}}c}{\\textcolor{tcC}{" << std::to_string(worstMatrix.falsePositive) << "}}\\\\ \
								\\mc{1}{>{\\columncolor{tcA}}r}{\\textbf{spoofed}} & \\mc{1}{>{\\columncolor{tcD}}c}{\\textcolor{tcC}{" << std::to_string(worstMatrix.falseNegative) << "}} & \\mc{1}{>{\\columncolor{tcB}}c}{\\textcolor{tcC}{" << std::to_string(worstMatrix.trueNegative) << "}} \
							\\end{tabular} \
							\\label{tab:classifier_SVM_" << std::to_string(int(pencentageSizeOfModel * 100)) << "_worse} \
						} \
					\\end{center} \
					\\caption{Confusion matrices for SVM distance classifier at " << std::to_string(int(pencentageSizeOfModel * 100)) << "\\% model} \
				\\end{table}";

				ofs.close();
			}

			/**
			 * Perform the experiment
			 *
			 * @param classFilesList - A list of wave files of the same class (ignore the first one)
			 * @param resultsDestiny
			 * @param amountOfTestsToPerfom
			 * @param minModel
			 * @param maxModel
			 */
			static void perform(std::vector<std::string> classFilesList, std::string resultsDestiny, unsigned int amountOfTestsToPerfom, double minModel, double maxModel)
			{
				std::cout << std::fixed;
				std::cout << std::setprecision(4);

				// set the callback function in the Experiment03 class
				Wav w;
				w.setCallbackFunction(Experiment03::waveletAnaliticFunction);

				// store the file path to be processed
				std::string line;

				/**
				 * A data structure witch will hold the wavelet transformed signals
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
				std::map<std::string, std::map<MEL_BARK, std::map<std::string, std::vector<std::vector<long double>>>>> results;

				////////////////////////////////////////
				/// Preparing to compute the progress //
				////////////////////////////////////////
				double cycles = 0;
				double totalCycles = 0;

				// Computes the cycles needed to compute all signals
				for (unsigned int i = 0; i < classFilesList.size(); i++)
				{
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
				for (unsigned int i = 0; i < classFilesList.size(); i++)
				{

					// file reader
					std::ifstream fileListStream;
					fileListStream.open(classFilesList[i], std::ios::in);

					// gets the file path to process
					while (std::getline(fileListStream, line))
					{

						// Status
						cycles++;
						std::cout << "\rPreparing features vectors... " << (cycles / totalCycles) * 100 << "%" << std::flush;

						// Initializes the experiment
						Experiment03::init();

						// lines that begins with # are going to be ignored
						if (line.find("#") == 0) continue;

						// Read the files and process it
						w.read(line.data());
						w.process();

						// Store the partial results
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

				// Holds the values for the graphic
				// of accuracy versus the number of
				// tests
				std::map<double, std::vector<double>> numberOfTestsForEachPercentage;

				std::map<double, double> stdDeviationForEachPercentage;
				std::map<double, std::vector<double>> allAccuracies;
				std::map<double, std::vector<double>> bestTestAccuracyForEachPercentage;
				std::map<double, std::vector<double>> worseTestAccuracyForEachPercentage;

				std::map<double, statistics::ConfusionMatrix> bestConfusionMatrixForEachPercentage;
				std::map<double, statistics::ConfusionMatrix> worseConfusionMatrixForEachPercentage;

				// Holds the partial user friendly reports
				std::string partialReport;

				// Used to calculate the accuracies
				double temp;

				// Used to calculate the worst accuracy of a percentage
				double percentageWorstAccuracy;

				// Used to calculate the best accuracy of a percentage
				double percentageBestAccuracy;

				// Stores the best and the worst confusion matrix
				statistics::ConfusionMatrix percentageBestConfusionMatrix;
				statistics::ConfusionMatrix percentageWorseConfusionMatrix;

				// Holds the tests features vectors for live signals
				std::vector<std::vector<long double>> testLive;

				// Holds the models features vectors for live signals
				std::vector<std::vector<long double>> modelLive;

				// Holds the tests features vectors for spoofed signals
				std::vector<std::vector<long double>> testSpoofing;

				// Holds the models features vectors for spoofed signals
				std::vector<std::vector<long double>> modelSpoofing;

				// Creating the classifier
				classifiers::SupportVectorMachine c;

				// Changes the percentage of the features vectors used as models for the classifier
				for (double modelPercentage = maxModel; modelPercentage >= minModel; modelPercentage -= .1)
				{

					// Initializing the accuracies
					percentageBestAccuracy = -std::numeric_limits<long double>().max(); // @suppress("Ambiguous problem")
					percentageWorstAccuracy = std::numeric_limits<long double>().max(); // @suppress("Ambiguous problem")

					// Changes the amount of tests done against the dataset
					for (unsigned int amountOfTests = 1; amountOfTests < amountOfTestsToPerfom + 1; amountOfTests++)
					{

						// Do the classification and populate the confusion matrix
						for (unsigned int k = 0; k < amountOfTests; k++)
						{

							// Sampling the live signals
							classifiers::raflleFeaturesVectors(results["haar"][BARK][classFilesList[0]], modelLive, testLive, modelPercentage);
							// Sampling the spoofed signals
							classifiers::raflleFeaturesVectors(results["haar"][BARK][classFilesList[1]], modelSpoofing, testSpoofing, modelPercentage);

							// Setting up the classifier
							c.clearTrain();
							c.addTrainingCases(modelLive, classifiers::SupportVectorMachine::POSITIVE);
							c.addTrainingCases(modelSpoofing, classifiers::SupportVectorMachine::NEGATIVE);
							c.train();

							// Preparing confusion matrix
							confusionMatrix.truePositive = 0;
							confusionMatrix.falsePositive = 0;
							confusionMatrix.trueNegative = 0;
							confusionMatrix.falseNegative = 0;

							/////////////////////////////////////////
							/// Populating the confusion matrices ///
							/////////////////////////////////////////

							for (auto test : testLive)
							{
								if (c.evaluate(test) == classifiers::SupportVectorMachine::POSITIVE)
								{
									confusionMatrix.truePositive += 1;
								} else
								{
									confusionMatrix.falseNegative += 1;
								}
							}

							for (auto test : testSpoofing)
							{
								if (c.evaluate(test) == classifiers::SupportVectorMachine::NEGATIVE)
								{
									confusionMatrix.trueNegative += 1;
								} else
								{
									confusionMatrix.falsePositive += 1;
								}
							}

							////////////////////////
							/// Conclusion phase ///
							////////////////////////

							temp = double(confusionMatrix.truePositive + confusionMatrix.trueNegative) / double(testLive.size() + testSpoofing.size());

							// store the best accuracy yet
							if (temp > percentageBestAccuracy)
							{
								percentageBestAccuracy = temp;
								percentageBestConfusionMatrix = confusionMatrix;
								partialReport = std::to_string(percentageBestAccuracy) + "\t" + std::to_string(confusionMatrix.truePositive) + "\t" + std::to_string(confusionMatrix.falsePositive) + "\t";
								partialReport += std::to_string(confusionMatrix.falseNegative) + "\t" + std::to_string(confusionMatrix.trueNegative);
							}

							// store the worst accuracy yet
							if (temp < percentageWorstAccuracy)
							{
								percentageWorstAccuracy = temp;
								percentageWorseConfusionMatrix = confusionMatrix;
								partialReport = std::to_string(percentageBestAccuracy) + "\t" + std::to_string(confusionMatrix.truePositive) + "\t" + std::to_string(confusionMatrix.falsePositive) + "\t";
								partialReport += std::to_string(confusionMatrix.falseNegative) + "\t" + std::to_string(confusionMatrix.trueNegative);
							}

							// store the current accuracy (will be use to calculate the standard deviation)
							allAccuracies[modelPercentage].push_back(temp);
						}

						// Store the results for the graphic
						numberOfTestsForEachPercentage[modelPercentage].push_back(amountOfTests);
						bestTestAccuracyForEachPercentage[modelPercentage].push_back(percentageBestAccuracy);
						worseTestAccuracyForEachPercentage[modelPercentage].push_back(percentageWorstAccuracy);
						bestConfusionMatrixForEachPercentage[modelPercentage] = percentageBestConfusionMatrix;
						worseConfusionMatrixForEachPercentage[modelPercentage] = percentageWorseConfusionMatrix;

						// Report for this amount of tests
						std::cout << "SVM: " << modelPercentage * 100 << "%\t" << amountOfTests << "\t" << partialReport << std::endl;
					}
				}

				//Calculates the standard deviation for each percentage
				for (auto test : numberOfTestsForEachPercentage)
				{
					stdDeviationForEachPercentage[test.first] = statistics::standardDeviation(allAccuracies[test.first]);
				}

				// Calculates the range of y axis
				// for a more regular plotting
				double yrange[2] = { 1, 0 };
				for (auto test : numberOfTestsForEachPercentage)
				{

					for (double v : worseTestAccuracyForEachPercentage[test.first])
					{
						yrange[0] = v < yrange[0] ? v : yrange[0];
					}
					for (double v : bestTestAccuracyForEachPercentage[test.first])
					{
						yrange[1] = v > yrange[1] ? v : yrange[1];
					}

					float temp = (yrange[1] - yrange[0]) / 30;
					yrange[0] -= temp;
					yrange[1] += temp;

				}

				// save plots and results
				for (auto test : numberOfTestsForEachPercentage)
				{
					saveConfusionMatrices(bestConfusionMatrixForEachPercentage[test.first], worseConfusionMatrixForEachPercentage[test.first], test.first, resultsDestiny);
					savePlotResults(test.second, bestTestAccuracyForEachPercentage[test.first], worseTestAccuracyForEachPercentage[test.first], stdDeviationForEachPercentage[test.first], test.first, resultsDestiny, yrange);
				}

			}
	};

}
#endif /* SRC_WAVELETEXPERIMENTS_03_EXPERIMENT03_CPP_ */
