///**
// * @author André Furlan
// * @email ensismoebius@gmail.com
// * This whole project are under GPLv3, for
// * more information read the license file
// *
// * 04 de mai de 2020
// * QUEM SABE DEPOIS DA QUALIFICAÇÃO!!!!
// */
//
//#ifndef SRC_WAVELETEXPERIMENTS_02_EXPERIMENT02_CPP_
//#define SRC_WAVELETEXPERIMENTS_02_EXPERIMENT02_CPP_
//
//#include <cmath>
//#include <string>
//#include <vector>
//#include <iomanip>
//#include <iostream>
//
//#include "../../lib/wave/Wav.cpp"
//#include "../../lib/wavelet/Types.h"
//#include "../../lib/vector/vectorUtils.h"
//#include "../../lib/gnuplot/gnuplotCalls.h"
//#include "../../lib/wavelet/waveletOperations.h"
//#include "../../lib/linearAlgebra/linearAlgebra.h"
//#include "../../lib/matplotlib-cpp/matplotlibcpp.h"
//#include "../../lib/paraconsistent/paraconsistent.h"
//#include "../../lib/wavelet/WaveletTransformResults.cpp"
//
//namespace waveletExperiments {
//
//	/**
//	 * Contains the code for experiment 01.
//	 */
//	class Experiment02 {
//		private:
//
//			/**
//			 * Wavelet waveform function
//			 */
//			static inline std::vector<double> wavelet;
//
//			/**
//			 * Wavelet type
//			 */
//			static inline wavelets::TransformMode mode;
//
//			/**
//			 * Contains the final results
//			 */
//			static inline std::map<std::string, std::map<wavelets::TransformMode, std::vector<std::vector<double>>>> finalResults;
//
//			/**
//			 * Max decomposition level for wavelets
//			 */
//			static inline unsigned int level;
//
//		public:
//
//			/**
//			 * Initialises the experiment
//			 * @param wavelet
//			 * @param mode
//			 */
//			static void init(std::vector<double> wavelet, wavelets::TransformMode mode, unsigned int level) {
//				Experiment02::mode = mode;
//				Experiment02::level = level;
//				Experiment02::wavelet = wavelet;
//			}
//
//			/**
//			 * Analityc function which performs an wavelet transform
//			 * of the value
//			 * @param signal
//			 * @param signalLength
//			 * @param samplingRate
//			 * @param path
//			 */
//			static void waveletAnaliticFunction(std::vector<double> &signal, int &signalLength, unsigned int samplingRate, std::string path) {
//
//				///////////////////////
//				/// Wavelet section ///
//				///////////////////////
//
//				std::vector<double> featureVector;
//				std::vector<double> chunks;
//
//				// Expands the signal length to optimize the wavelet transform
//				signalLength = wavelets::getNextPowerOfTwo(signal.size());
//				signal.resize(signalLength, 0);
//
//				// Does the transformations
//				wavelets::WaveletTransformResults transformedSignal = wavelets::malat(signal, Experiment02::wavelet, Experiment02::mode, Experiment02::level);
//
//				if (Experiment02::mode == wavelets::PACKET_WAVELET) {
//					featureVector.resize(transformedSignal.getWaveletPacketAmountOfParts());
//
//					for (unsigned int i = 0; i < featureVector.size(); ++i) {
//						chunks = transformedSignal.getWaveletPacketTransforms(i);
//
//						for (double v : chunks) {
//							featureVector[i] += std::pow(v, 2);
//						}
//
//					}
//
//				} else if (Experiment02::mode == wavelets::REGULAR_WAVELET) {
//
//					featureVector.resize(Experiment02::level + 1);
//
//					for (unsigned int i = 0; i < featureVector.size(); ++i) {
//						chunks = transformedSignal.getWaveletTransforms(i);
//
//						for (double v : chunks) {
//							featureVector[i] += std::pow(v, 2);
//						}
//
//					}
//
//				}
//
//				linearAlgebra::normalizeVector(featureVector);
//
//				signal = featureVector;
//			}
//
//			/**
//			 * Plot the results on a paraconsistent plane
//			 * @param results
//			 */
//			static void plotResults(std::map<std::string, std::map<wavelets::TransformMode, std::vector<std::vector<double>>>> results) {
//
//				// Alias for a easier use of matplotlib
//				namespace plt = matplotlibcpp;
//
//				// The values of contradiction and certaint ever goes around 0 and 1
//				plt::xlim(-1, 1);
//				plt::ylim(-1, 1);
//
//				plt::grid(true);
//
//				// Preparing the paraconsistent plane
//				std::vector<int> x = { 1, 0 };
//				std::vector<int> y = { 0, 1 };
//
//				x = { 0, 1 };
//				y = { 1, 0 };
//				plt::plot(x, y);
//				plt::text(1, 0, "Truth");
//
//				x = { 1, 0 };
//				y = { 0, -1 };
//				plt::plot(x, y);
//				plt::text(0, -1, "Indefinition");
//
//				x = { 0, -1 };
//				y = { -1, 0 };
//				plt::plot(x, y);
//				plt::text(-1, 0, "Falsehood");
//
//				x = { -1, 0 };
//				y = { 0, 1 };
//				plt::plot(x, y);
//				plt::text(0, 1, "Ambiguity");
//				// Paraconsistent plane ready!
//
//				plt::xlabel("Certaint");
//				plt::ylabel("Contradiction");
//
//				std::string annotation;
//
//				// Iterates over all wavelets
//				for (std::pair<std::string, std::map<wavelets::TransformMode, std::vector<std::vector<double>>>> wavelet : results) {
//
//					// Iterates over REGULAR and PAKET wavelets (yes, just two values)
//					for (std::pair<wavelets::TransformMode, std::vector<std::vector<double>>> data : wavelet.second) {
//						annotation = std::string("←") + (data.first == wavelets::REGULAR_WAVELET ? "R-" : "P-") + wavelet.first;
//
//						plt::title("Regular (R) and Packet(P) wavelets on paraconsistent measures");
//						plt::scatter(data.second[0], data.second[1], 100.0);
//						plt::annotate(annotation, data.second[0][0], data.second[1][0]);
//					}
//				}
//
//				plt::show();
//				plt::pause(.1);
//			}
//
//			/**
//			 * Save the results to file on /tmp/results2.csv
//			 * @param data
//			 */
//			static void saveDataToFile(std::map<std::string, std::map<wavelets::TransformMode, std::vector<std::vector<double>>>> data) {
//
//				// Open the file
//				std::string filePath = "/tmp/results2.csv";
//				std::ofstream ofs(filePath, std::ios::app | std::ios::out);
//				if (!ofs.is_open()) {
//					std::cout << "Cannot open file: " << filePath;
//					throw std::runtime_error("Impossible to open the file!");
//					return;
//				}
//
//				// A misplaced variable used to store the distance
//				// from the point 1,0 at the paraconsistent plane
//				double distanceTo1_0 = 0;
//
//				for (std::pair<std::string, std::map<wavelets::TransformMode, std::vector<std::vector<double>>>> wavelet : data) {
//					for (std::pair<wavelets::TransformMode, std::vector<std::vector<double>>> regularOrPacketAndData : wavelet.second) {
//
//						// Calculating the distance
//						distanceTo1_0 = std::sqrt(std::pow(regularOrPacketAndData.second[0][0] - 1, 2) + std::pow(regularOrPacketAndData.second[0][1], 2));
//
//						// Writing in to the file
//						ofs << (regularOrPacketAndData.first == wavelets::REGULAR_WAVELET ? "Regular" : "Packet") << '\t' << wavelet.first << '\t';
//						ofs << regularOrPacketAndData.second[0][0] << '\t' << regularOrPacketAndData.second[0][1] << '\t' << distanceTo1_0 << '\t';
//						ofs << std::endl;
//					}
//				}
//				ofs.close();
//			}
//
//			/**
//			 * Perform the experiment
//			 * @param args - A list of wavefiles of the same class (ignore the first one)
//			 * @param argCount - The amount of these files
//			 */
//			static void perform(char *args[], int argCount) {
//				std::cout << std::fixed;
//				std::cout << std::setprecision(20);
//
//				// set the callback function in the Experiment02 class
//				Wav w;
//				w.setCallbackFunction(Experiment02::waveletAnaliticFunction);
//
//				// store the file path to be processed
//				std::string line;
//
//				// Max levels of decomposition
//				unsigned int maxLevels = 10;
//
//				/**
//				 * A data estructure witch will hold the wavelet transformed signals
//				 * wavelet1
//				 * 	REGULAR
//				 * 		Class1
//				 * 			data1
//				 * 			data2
//				 * 			data3
//				 * 			etc.
//				 * 		Class2
//				 * 			data1
//				 * 			data2
//				 * 			data3
//				 * 			etc.
//				 * 		Etc.
//				 * 	PACKET
//				 * 		Class1
//				 * 			data1
//				 * 			data2
//				 * 			data3
//				 * 			etc.
//				 * 		Class2
//				 * 			data1
//				 * 			data2
//				 * 			data3
//				 * 			etc.
//				 * 		Etc.
//				 * 	ETC.
//				 * wavelet2
//				 * 	...
//				 */
//				std::map<std::string, std::map<wavelets::TransformMode, std::map<std::string, std::vector<std::vector<std::vector<double>>>>>> results;
//				std::map<std::string,double> res2;
//
//				// Used to compute
//				double cycles = 0;
//				double totalCycles = 0;
//
//				// Computes the cicles needed to compute all signals
//				for (int i = 1; i < argCount; i++) {
//					std::ifstream fileListStream;
//					fileListStream.open(args[i], std::ios::out);
//
//					while (std::getline(fileListStream, line))
//						totalCycles++;
//
//					fileListStream.clear();
//					fileListStream.close();
//				}
//				totalCycles = wavelets::all().size() * (argCount - 1) * totalCycles * maxLevels;
//
//				// Iterates over all files, this files
//				// should represent our data classes
//				for (int i = 1; i < argCount; i++) {
//
//					// file reader
//					std::ifstream fileListStream;
//					fileListStream.open(args[i], std::ios::in);
//
//					// iterates over all wavelets types
//					for (std::pair<std::string, std::vector<double>> v : wavelets::all()) {
//
//						// Iterates over all Regular or Packet wavelets
//						for (int pr = wavelets::PACKET_WAVELET; pr <= wavelets::REGULAR_WAVELET; pr++) {
//
//							for (unsigned int level = 1; level <= maxLevels; ++level) {
//								// clear fail and eof bits
//								fileListStream.clear();
//								// back to the start!
//								fileListStream.seekg(0, std::ios::beg);
//
//								// gets the file path to process
//								while (std::getline(fileListStream, line)) {
//
//									// Status
//									cycles++;
//									std::cout << "\rCompletion " << (cycles / totalCycles) * 100 << "%" << std::flush;
//
//									// set current wavelet and barkOrMel to the experiment
//									Experiment02::init(v.second, static_cast<wavelets::TransformMode>(pr), level);
//
//									// lines that begins with # are going to be ignored
//									if (line.find("#") == 0) continue;
//
//									w.read(line.data());
//									w.process();
//
//									results[v.first][static_cast<wavelets::TransformMode>(pr)][args[i]].resize(level);
//									results[v.first][static_cast<wavelets::TransformMode>(pr)][args[i]].at(level - 1).push_back(w.getData());
//								}
//							}
//						}
//					}
//
//					fileListStream.clear();
//					fileListStream.close();
//				}
//
//				//////////////////////////////
//				/// Paraconsistent section ///
//				//////////////////////////////
//
//				// iterates over all wavelets types
//				for (std::pair<std::string, std::vector<double>> v : wavelets::all()) {
//					// Iterates over all barkOrMel
//					for (int pr = wavelets::PACKET_WAVELET; pr <= wavelets::REGULAR_WAVELET; pr++) {
//						for (unsigned int level = 1; level < maxLevels; ++level) {
//
//							unsigned int featureVectorsPerClass = results[v.first][static_cast<wavelets::TransformMode>(pr)][args[1]].size();
//							unsigned int featureVectorSize = results[v.first][static_cast<wavelets::TransformMode>(pr)][args[1]][level - 1].size();
//
////							std::map<std::string, std::vector<std::vector<double>>> arrClasses = results[v.first][static_cast<wavelets::TransformMode>(pr)][args[1]][level];
////
////							double alpha = calculateAlpha(argCount, featureVectorsPerClass, featureVectorSize, arrClasses);
////							double betha = calculateBeta(argCount, featureVectorsPerClass, featureVectorSize, arrClasses);
////
////							double certaintyDegree_G1 = calcCertaintyDegree_G1(alpha, betha);
////							double contradictionDegree_G2 = calcContradictionDegree_G2(alpha, betha);
////
////							// Calculating the position at the paraconsistent plane
////							std::cout << "Certainty degree     :" << certaintyDegree_G1 << std::endl;
////							std::cout << "Contradiction degree :" << contradictionDegree_G2 << std::endl;
////
////							// Storing the final results
////							finalResults[v.first][static_cast<wavelets::TransformMode>(pr)].resize(2);
////							finalResults[v.first][static_cast<wavelets::TransformMode>(pr)].at(0) = { certaintyDegree_G1 };
////							finalResults[v.first][static_cast<wavelets::TransformMode>(pr)].at(1) = { contradictionDegree_G2 };
//						}
//					}
//				}
//
//				saveDataToFile(finalResults);
//				plotResults(finalResults);
//			}
//	};
//}
//
//#endif /* SRC_WAVELETEXPERIMENTS_02_EXPERIMENT02_CPP_ */
//
