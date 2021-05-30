/**
 * @author André Furlan
 * @email ensismoebius@gmail.com
 * This whole project are under GPLv3, for
 * more information read the license file
 *
 * 24 de abr de 2020
 *
 * The experiment 01 consists in take the Wavelet transforms
 * of the given signal until each coefficient is just a number,
 * after this, a sum of the transformed signal squares is taken
 * for each range defined in MEL or BARK scales and following
 * the respectives rules for BARK or MEL scales to build a
 * features vector witch will be analyzed by a paraconsistent
 * algorithm. The results will be plotted and saved.
 * It will be done for all files and wavelets.
 */

#ifndef SRC_WAVELETEXPERIMENTS_01_EXPERIMENT01_CPP_
#define SRC_WAVELETEXPERIMENTS_01_EXPERIMENT01_CPP_

#include <cmath>
#include <string>
#include <vector>
#include <future>
#include <iomanip>
#include <iostream>

#include "../../lib/wave/Wav.h"
#include "../../lib/wavelet/Types.h"
#include "../../lib/vector/vectorUtils.h"
#include "../../lib/gnuplot/gnuplotCalls.h"
#include "../../lib/wavelet/waveletOperations.h"
#include "../../lib/linearAlgebra/linearAlgebra.h"
#include "../../lib/matplotlib-cpp/matplotlibcpp.h"
#include "../../lib/paraconsistent/paraconsistent.h"
#include "../../lib/wavelet/WaveletTransformResults.cpp"

namespace waveletExperiments
{

	/**
	 * Contains the code for experiment 01.
	 */
	class Experiment01
	{
		private:

			/**
			 * Used to define when MEL or BARK is used
			 */
			enum BARK_MEL
			{
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
			 * Initializes the experiment
			 * @param wavelet
			 * @param mode
			 * @param barkOrMel
			 */
			static void init(std::vector<double> wavelet, wavelets::TransformMode mode, BARK_MEL barkOrMel)
			{
				Experiment01::mode = mode;
				Experiment01::wavelet = wavelet;
				Experiment01::barkOrMel = barkOrMel;
				Experiment01::MELRanges = { 20, 160, 394, 670, 1000, 1420, 1900, 2450, 3120, 4000, 5100, 6600, 9000, 14000 };
				Experiment01::BARKRanges = { 20, 100, 200, 300, 400, 510, 630, 770, 920, 1080, 1270, 1480, 1720, 2000, 2320, 2700, 3150, 3700, 4400, 5300, 6400, 7700, 9500, 12000, 15500 };
			}

			/**
			 * Analytic function which performs an wavelet transform
			 * of the value and calculate the energies based on MEL
			 * or BARK intervals
			 *
			 * @param signal
			 * @param signalLength
			 * @param samplingRate
			 * @param path
			 */
			static void waveletAnaliticFunction(std::vector<double> &signal, int &signalLength, unsigned int samplingRate, std::string path)
			{

				///////////////////////
				/// Wavelet section ///
				///////////////////////

				// Normalizes the signal between -1 and 1
				linearAlgebra::normalizeVectorToRange(signal, -1, 1);

				// Expands the signal length to optimize the wavelet transform
				signalLength = wavelets::getNextPowerOfTwo(signal.size());
				signal.resize(signalLength, 0);

				// Calculates the maximum levels of decompositions
				// i.e. until the coefficients are formed by
				// just single numbers.
				// This is needed because at the end of the
				// transformation we will perform a MEL and
				// BARK composition
				int level = std::log2(signal.size());

				// Does the transformations
				wavelets::WaveletTransformResults transformedSignal = wavelets::malat(signal, Experiment01::wavelet, Experiment01::mode, level);

				///////////////////////////
				/// MEL or BARK section ///
				///////////////////////////

				// Contains the values of the MEL or BARK ranges
				std::vector<double> scaleRanges;

				if (Experiment01::barkOrMel == MEL)
				{
					// Ranges for MEL scale
					scaleRanges = MELRanges;
				} else
				{
					// Ranges for BARK scale
					scaleRanges = BARKRanges;
				}

				// Features vector has the amount of values equals to amount of the ranges minus 1
				// because we are summing up intervals
				std::vector<double> featureVector(scaleRanges.size() - 1);

				// We need to known the max frequency supported
				// by the signal in order to find the values in
				// which the sums of the BARK and MEL scales
				// will be performed
				double maxFrequency = samplingRate / 2;

				// Calculates the minimum frequency range which
				// will enable the correct interval sums to
				// be performed
				double frequencyChunckSize = maxFrequency / transformedSignal.getWaveletPacketAmountOfParts();

				// Used to retrieve the interval for the sums
				double rangeScaleEnd = 0;
				double rangeScaleStart = 0;

				// Loop over all the ranges and calculate the energies inside it
				for (unsigned int i = 0; i < scaleRanges.size() - 1; i++)
				{

					// Retrieves the interval for the sums
					rangeScaleStart = scaleRanges.at(i);
					rangeScaleEnd = scaleRanges.at(i + 1);

					// Calculates the interval indexes inside the transformed signal
					int startIndex = rangeScaleStart / frequencyChunckSize;
					int endIndex = rangeScaleEnd / frequencyChunckSize;

					// Sums the values from selected range
					for (int j = startIndex; j < endIndex; ++j)
					{

						// Retrieve the values
						std::vector<double> sig1 = transformedSignal.getWaveletPacketTransforms(startIndex);

						// Sum the power of 2 of them all!!! (i.e. calculate the energies)
						featureVector.at(i) = 0;
						for (double v : sig1)
						{
							featureVector.at(i) += std::pow(v, 2);
						}

						// Just for MEL
						if (Experiment01::barkOrMel == MEL)
						{
							featureVector.at(i) = featureVector.at(i) == 0 ? 0 : std::log10(featureVector.at(i));
						}
					}

				}

				// Just for MEL
				if (Experiment01::barkOrMel == MEL)
				{
					// Applies a DCT (Discrete Cosine Transform)
					linearAlgebra::discreteCosineTransform(featureVector);

					// Applies the double derivative of the features vector
					linearAlgebra::derivative(featureVector, 2);
				}

				// Normalizes the resulting features vector
				linearAlgebra::normalizeVectorToSum1(featureVector);

				// Replaces the original signal
				signal = featureVector;
			}

			/**
			 * Plots the results
			 * @param results
			 */
			static void plotResults(std::map<std::string, std::map<BARK_MEL, std::vector<std::vector<double>>>> results)
			{

				// Alias for a easier use of matplotlib
				namespace plt = matplotlibcpp;

				double dist;
				int pos = 1;
				std::vector<double> posVect;
				std::vector<double> distancesFrom1_0;
				std::vector<std::string> annotations;

				plt::title("Wavelets on MEL(M) and BARK(B) on paraconsistent measures");

				// Iterates over all wavelets
				for (std::pair<std::string, std::map<BARK_MEL, std::vector<std::vector<double>>>> wavelet : results)
				{

					// Iterates over BARK and MEL (yes, just two values)
					for (std::pair<BARK_MEL, std::vector<std::vector<double>>> data : wavelet.second)
					{

						posVect.push_back(pos++);
						annotations.push_back((data.first == BARK ? "B-" : "M-") + wavelet.first);

						// Calculate the distance from point 1,0 (true) of the paraconsistent plane
						dist = std::sqrt(std::pow(data.second[0][0] - 1, 2) + std::pow(data.second[0][1], 2));

						// Represents the values in a log of 10 base
						distancesFrom1_0.push_back(std::log10(dist));
					}
				}

				plt::xlabel("Values in Log10(x) - The least the better");
				plt::barh(posVect, distancesFrom1_0);
				plt::yticks(posVect, annotations);
				plt::tight_layout();
				plt::show();
			}

			/**
			 * Saves the results to @resultsDestiny
			 * @param data
			 * @param resultsDestiny
			 */
			static void saveDataToFile(std::map<std::string, std::map<BARK_MEL, std::vector<std::vector<double>>>> data, std::string resultsDestiny)
			{

				// Open the file
				std::string filePath = resultsDestiny + "/results.csv";
				std::ofstream ofs(filePath, std::ios::trunc | std::ios::out);

				if (!ofs.is_open())
				{
					std::cout << "Cannot open file: " << filePath;
					throw std::runtime_error("Impossible to open the file!");
					return;
				}

				for (std::pair<std::string, std::map<BARK_MEL, std::vector<std::vector<double>>>> wavelet : data)
				{
					for (std::pair<BARK_MEL, std::vector<std::vector<double>>> barkOrMelSAndData : wavelet.second)
					{

						// Writing in to the file
						ofs << std::boolalpha
							<< std::fixed
							<< std::setprecision(10)
							<< (barkOrMelSAndData.first == BARK ? "BARK" : "MEL") << '\t'
							<< wavelet.first << '\t'
							<< std::noboolalpha
							<< std::scientific
							<< std::dec
							<< barkOrMelSAndData.second[0][0]
							<< std::boolalpha
							<< '\t'
							<< std::noboolalpha
							<< std::scientific
							<< std::dec
							<< barkOrMelSAndData.second[1][0]
							<< std::boolalpha
							<< '\t'
							<< std::noboolalpha
							<< std::scientific
							<< std::dec
							<< barkOrMelSAndData.second[2][0]
							<< std::boolalpha
							<< std::endl;
					}
				}
				ofs.close();
			}

			/**
			 *
			 * Perform the experiment
			 * @param classFileList - A list of wave files of the same class (ignore the first one)
			 * @param resultsDestiny
			 */
			static void perform(std::vector<std::string> classFileList, std::string resultsDestiny)
			{
				std::cout << std::fixed;
				std::cout << std::setprecision(10);

				// set the callback function in the Experiment01 class
				Wav w;
				w.setCallbackFunction(Experiment01::waveletAnaliticFunction);

				// store the file path to be processed
				std::string line;

				/**
				 * A data structure witch will hold the wavelet transformed signals
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

				// Computes the cycles needed to compute all signals
				for (unsigned int i = 0; i < classFileList.size(); i++)
				{
					std::ifstream fileListStream;
					fileListStream.open(classFileList[i], std::ios::out);

					while (std::getline(fileListStream, line))
					{
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
				for (unsigned int i = 0; i < classFileList.size(); i++)
				{
					// file reader
					std::ifstream fileListStream;
					fileListStream.open(classFileList[i], std::ios::in);

					// iterates over all wavelets types
					for (std::pair<std::string, std::vector<double>> v : wavelets::all())
					{

						// Iterates over all barkOrMel
						for (int bm = BARK; bm <= MEL; bm++)
						{

							// clear fail and eof bits
							fileListStream.clear();
							// back to the start!
							fileListStream.seekg(0, std::ios::beg);

							// gets the file path to process
							while (std::getline(fileListStream, line))
							{

								// lines that begins with # are going to be ignored
								if (line.find("#") == 0) continue;

								// Status
								cycles++;
								std::cout << "\rCompletion " << (cycles / totalCycles) * 100 << "%" << std::flush;

								// set current wavelet and barkOrMel to the experiment
								Experiment01::init(v.second, wavelets::PACKET_WAVELET, static_cast<BARK_MEL>(bm));

								w.read(line.data());
								w.process();
								results[v.first][static_cast<BARK_MEL>(bm)][classFileList[i]].push_back(w.getData());
							}
						}
					}

					fileListStream.clear();
					fileListStream.close();
				}

				//////////////////////////////
				/// Paraconsistent section ///
				//////////////////////////////

				std::cout << std::endl << "----------------------" << std::endl;

				// iterates over all wavelets types
				for (std::pair<std::string, std::vector<double>> v : wavelets::all())
				{
					// Iterates over all barkOrMel
					for (int bm = BARK; bm <= MEL; bm++)
					{
						unsigned int featureVectorsPerClass = results.at(v.first).at(static_cast<BARK_MEL>(bm)).at(classFileList[0]).size();
						unsigned int featureVectorSize = results.at(v.first).at(static_cast<BARK_MEL>(bm)).at(classFileList[0]).at(0).size();

						std::map<std::string, std::vector<std::vector<double>>> arrClasses = results.at(v.first).at(static_cast<BARK_MEL>(bm));

						double alpha = calculateAlpha(classFileList.size(), featureVectorsPerClass, featureVectorSize, arrClasses);
						double betha = calculateBeta(classFileList.size(), featureVectorsPerClass, featureVectorSize, arrClasses);

						double certaintyDegree_G1 = calcCertaintyDegree_G1(alpha, betha);
						double contradictionDegree_G2 = calcContradictionDegree_G2(alpha, betha);
						double distanceTo1_0 = std::sqrt(std::pow(certaintyDegree_G1 - 1, 2) + std::pow(contradictionDegree_G2, 2));

						// Calculating the position at the paraconsistent plane
						std::cout << "Certainty degree     :" << certaintyDegree_G1 << std::endl;
						std::cout << "Contradiction degree :" << contradictionDegree_G2 << std::endl;
						std::cout << "Dist from point(1,0) :" << distanceTo1_0 << std::endl;
						std::cout << "----------------------" << std::endl;

						// Storing the final results
						finalResults[v.first][static_cast<BARK_MEL>(bm)].resize(3);
						finalResults[v.first][static_cast<BARK_MEL>(bm)].at(0) = { certaintyDegree_G1 };
						finalResults[v.first][static_cast<BARK_MEL>(bm)].at(1) = { contradictionDegree_G2 };
						finalResults[v.first][static_cast<BARK_MEL>(bm)].at(2) = { distanceTo1_0 };
					}
				}

				saveDataToFile(finalResults, resultsDestiny);
				plotResults(finalResults);
			}
	};
}

#endif /* SRC_WAVELETEXPERIMENTS_01_EXPERIMENT01_CPP_ */

