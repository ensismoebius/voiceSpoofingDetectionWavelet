/**
 * @author André Furlan
 * @email ensismoebius@gmail.com
 * This whole project are under GPLv3, for
 * more information read the license file
 *
 * 14 de mai de 2020
 *
 * This code create the EER(Equal Error Rate) plots
 * for SVM classifier
 */

#ifndef SRC_WAVELETEXPERIMENTS_08_EXPERIMENT08_CPP_
#define SRC_WAVELETEXPERIMENTS_08_EXPERIMENT08_CPP_

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
	 * Contains the code for experiment 08.
	 */
	class Experiment08
	{
		private:

			/**
			 * Used to define when MEL or MEL is used
			 */
			enum BARK_MEL
			{
				BARK, MEL
			};

			/**
			 * Contains the MEL ranges values
			 */
			static inline std::vector<double> melRanges;

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
				wavelets::init({"haar"});
				Experiment08::wavelet = wavelets::get("haar");
				Experiment08::melRanges = { 20, 160, 394, 670, 1000, 1420, 1900, 2450, 3120, 4000, 5100, 6600, 9000, 14000 };
			}

			/**
			 * Analytic function which performs an wavelet transform
			 * of signal and calculate the energies based on MEL intervals
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
				// transformation we will perform a MEL composition
				int level = std::log2(signal.size());

				// Does the transformations
				wavelets::WaveletTransformResults transformedSignal = wavelets::malat(signal, Experiment08::wavelet, wavelets::PACKET_WAVELET, level);

				////////////////////
				/// MEL section ///
				////////////////////

				// features vector has the amount of values equals to amount of the ranges minus 1 
				// because we are summing up intervals
				std::vector<long double> featureVector(melRanges.size() - 1);

				// We need to known the max frequency supported
				// by the signal in order to find the values in
				// which the sums of the MEL scale will be
				// performed
				double maxFrequency = samplingRate / 2;

				// Used to retrieve the interval for the sums
				double rangeScaleEnd = 0;
				double rangeScaleStart = 0;

				// Loop over all the ranges and calculate the energies inside it
				for (unsigned int i = 0; i < melRanges.size() - 1; i++)
				{

					// Retrieves the interval for the sums
					rangeScaleStart = melRanges.at(i);
					rangeScaleEnd = melRanges.at(i + 1);

					// Retrieve the values
					std::vector<long double> sig1 = transformedSignal.getWaveletPacketTransforms(rangeScaleStart, rangeScaleEnd, maxFrequency);

					// Sum the power of 2 of them all!!! (i.e. calculate the energies)
					featureVector.at(i) = 0;
					for (double v : sig1)
					{
						featureVector.at(i) += std::pow(v, 2);
					}

					featureVector.at(i) = featureVector.at(i) == 0 ? 0 : std::log10(featureVector.at(i));
				}

				// Applies a DCT (Discrete Cosine Transform)
				linearAlgebra::discreteCosineTransform(featureVector);

				// Applies the double derivative of the features vector
				linearAlgebra::derivative(featureVector, 2);

				// Normalizes the resulting features vector
				linearAlgebra::normalizeVectorToSum1(featureVector);

				// Replaces the original signal
				signal = featureVector;
			}

			/**
			 * Save confusion matrices in @destiny
			 * @param confusionMatrices
			 * @param percentage
			 * @param destiny
			 */
			static void savePlotResults(std::vector<statistics::ConfusionMatrix> confusionMatrices, double percentage, std::string destiny)
			{

				// Alias for a easier use of matplotlib
				namespace plt = matplotlibcpp;

				// Preparing data for plotting
				std::vector<double> fpr;
				std::vector<double> fnr;
				double eer;

				statistics::calculateEER(confusionMatrices, eer, fpr, fnr);

				// Plotting data
				plt::text(eer, eer, "EER:" + std::to_string(eer));
				plt::plot( { 0, 1 });
				plt::plot(fpr, fnr);

				plt::title("Detection Error Tradeoff curve and EER using SVM classifier.\n Model size: " + std::to_string(int(percentage * 100)) + "%");
				plt::xlabel("False Acceptance Rate");
				plt::ylabel("False Rejection Rate");
				plt::xlim(0.0, .5);
				plt::ylim(0.0, .5);

				plt::grid(true);
				plt::save(destiny + "/DET_for_classifier_SVM_" + std::to_string(int(percentage * 100)) + ".pdf");
				plt::clf();
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

				// set the callback function in the Experiment08 class
				Wav w;
				w.setCallbackFunction(Experiment08::waveletAnaliticFunction);

				// store the file path to be processed
				std::string line;

				/**
				 * A data structure witch will hold the wavelet transformed signals
				 * haar
				 * 	MEL
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
				std::map<std::string, std::map<BARK_MEL, std::map<std::string, std::vector<std::vector<long double>>>>> results;

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
				/// Processing data with wavelet haar and MEL ///
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
						Experiment08::init();

						// lines that begins with # are going to be ignored
						if (line.find("#") == 0) continue;

						// Read the files and process it
						w.read(line.data());
						w.process();

						// Store the partial results
						results["haar"][MEL][classFilesList[i]].push_back(w.getData());
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

					// Changes the amount of tests done against the dataset
					confusionMatricesForEachPercentage[modelPercentage].resize(amountOfTestsToPerfom);
					for (unsigned int testIndex = 0; testIndex < amountOfTestsToPerfom; testIndex++)
					{

						// Sampling the live signals
						classifiers::raflleFeaturesVectors(results["haar"][MEL][classFilesList[0]], modelLive, testLive, modelPercentage);
						// Sampling the spoofed signals
						classifiers::raflleFeaturesVectors(results["haar"][MEL][classFilesList[1]], modelSpoofing, testSpoofing, modelPercentage);

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

						confusionMatricesForEachPercentage[modelPercentage][testIndex] = confusionMatrix;

						std::cout << "\rPreparing DET curve... " << 100 * testIndex / amountOfTestsToPerfom << "%" << std::flush;
					}

					// Report for this amount of tests
					std::cout << std::endl << "SVM " << modelPercentage * 100 << "%\t" << std::endl;
				}

				// Plot everything
				for (auto confusionMatrices : confusionMatricesForEachPercentage)
				{
					savePlotResults(confusionMatrices.second, confusionMatrices.first, resultsDestiny);
				}
			}
	};
}
#endif /* SRC_WAVELETEXPERIMENTS_08_EXPERIMENT08_CPP_ */
