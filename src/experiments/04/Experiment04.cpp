/**
 * @author André Furlan
 * @email ensismoebius@gmail.com
 * This whole project are under GPLv3, for
 * more information read the license file
 *
 * 04 de mai de 2020
 *
 * This experiment visually demonstrates the wavelet packet
 * transformation given a repeated dummy signal. This was
 * made in order to justify why the wavelet haar is better
 * for create features vectors for classifications
 */

#ifndef SRC_WAVELETEXPERIMENTS_04_EXPERIMENT04_CPP_
#define SRC_WAVELETEXPERIMENTS_04_EXPERIMENT04_CPP_

#include <string>
#include <vector>
#include <iomanip>
#include <iostream>

#include "../../lib/wavelet/Types.h"
#include "../../lib/wavelet/waveletOperations.h"
#include "../../lib/linearAlgebra/linearAlgebra.h"
#include "../../lib/matplotlib-cpp/matplotlibcpp.h"

namespace waveletExperiments
{

	/**
	 * Contains the code for experiment 04.
	 */
	class Experiment04
	{
		private:

			static inline std::map<std::string, std::vector<double>> finalResults;

		public:

			static std::vector<double> waveletAnaliticFunction(std::vector<double> &signal, int signalLength, std::vector<double> wavelet, unsigned int level)
			{

				// Expands the signal length to optimize the wavelet transform
				signalLength = wavelets::getNextPowerOfTwo(signal.size());
				signal.resize(signalLength, 0);

				// Does the transformations
				wavelets::WaveletTransformResults transformedSignal = wavelets::malat(signal, wavelet, wavelets::PACKET_WAVELET, level);

				return transformedSignal.transformedSignal;
			}

			/**
			 * Plot the results
			 * @param results
			 */
			static void plotResults(std::map<std::string, std::vector<double>> results, int level, int size)
			{
				namespace plt = matplotlibcpp;

				// Calculates the limits of the graphic
				double ylimMax = -std::numeric_limits<double>().max(); // @suppress("Ambiguous problem")
				double ylimMin = +std::numeric_limits<double>().max(); // @suppress("Ambiguous problem")
				for (auto v : results)
				{
					for (auto number : v.second)
					{
						ylimMax = ylimMax < number ? number : ylimMax;
						ylimMin = ylimMin > number ? number : ylimMin;
					}
				}

				// Plots all results
				int plot = 1;
				for (auto v : results)
				{
					plt::subplot(results.size(), 1, plot++);
					plt::ylim(ylimMin, ylimMax);
					plt::named_plot(v.first, v.second);
					plt::title("name: " + v.first + " lvl:" + std::to_string(level) + " size:" + std::to_string(size));
				}

				plt::grid(true);
				plt::legend();
				plt::show();
			}

			/**
			 * Perform the experiment
			 */
			static void perform()
			{
				std::cout << std::fixed;
				std::cout << std::setprecision(20);

				// Dummy signal
				std::vector<double> signal = { 32, 10, 20, 38, 37, 28, 38, 34, 18, 24, 24, 9, 23, 24, 28, 34, 32, 10, 20, 38, 37, 28, 38, 34, 18, 24, 24, 9, 23, 24, 28, 34, 32, 10, 20, 38, 37, 28, 38, 34, 18, 24, 24, 9, 23, 24, 28, 34, 32, 10, 20, 38, 37, 28, 38, 34, 18, 24, 24, 9, 23, 24, 28, 34, 32, 10, 20, 38, 37, 28, 38, 34, 18, 24, 24, 9, 23, 24, 28, 34, 32, 10, 20, 38, 37, 28, 38, 34, 18, 24, 24, 9, 23, 24, 28, 34, 32, 10, 20, 38, 37, 28, 38, 34, 18, 24, 24, 9, 23, 24, 28, 34, 32, 10, 20, 38, 37, 28, 38, 34, 18, 24, 24, 9, 23, 24, 28, 34, 32, 10, 20, 38, 37, 28, 38, 34, 18, 24, 24, 9, 23, 24, 28, 34, 32, 10, 20, 38, 37, 28, 38, 34, 18, 24, 24, 9, 23, 24, 28, 34, 32, 10, 20, 38, 37, 28, 38, 34, 18, 24, 24, 9, 23, 24, 28, 34, 32, 10, 20, 38, 37, 28, 38, 34, 18, 24, 24, 9, 23, 24, 28, 34, 32, 10, 20, 38, 37, 28, 38, 34, 18, 24, 24, 9, 23, 24, 28, 34, 32, 10, 20, 38, 37, 28, 38, 34, 18, 24, 24, 9, 23, 24, 28, 34, 32, 10, 20, 38, 37, 28, 38, 34, 18, 24, 24, 9, 23, 24, 28, 34, 32, 10, 20, 38, 37, 28, 38, 34, 18, 24, 24, 9, 23, 24, 28, 34, 32, 10, 20, 38, 37, 28, 38, 34, 18, 24, 24, 9, 23, 24, 28, 34, 32, 10, 20, 38, 37, 28, 38, 34, 18, 24, 24, 9, 23, 24, 28, 34, 32, 10, 20, 38, 37, 28, 38, 34, 18, 24, 24, 9, 23, 24, 28, 34, 32, 10, 20, 38, 37, 28, 38, 34, 18, 24, 24, 9, 23, 24, 28, 34, 32, 10, 20, 38, 37, 28, 38, 34, 18, 24, 24, 9, 23, 24, 28, 34, 32, 10, 20, 38, 37, 28, 38, 34, 18, 24, 24, 9, 23, 24, 28, 34, 32, 10, 20, 38, 37, 28, 38, 34, 18, 24, 24, 9, 23, 24, 28, 34, 32, 10, 20, 38, 37, 28, 38, 34, 18, 24, 24, 9, 23, 24, 28, 34, 32, 10, 20, 38, 37, 28, 38, 34, 18, 24, 24, 9, 23, 24, 28, 34, 32, 10, 20, 38, 37, 28, 38, 34, 18, 24, 24, 9, 23, 24, 28, 34, 32, 10, 20, 38, 37, 28, 38, 34, 18, 24, 24, 9, 23, 24, 28, 34, 32, 10, 20, 38, 37, 28, 38, 34, 18, 24, 24, 9, 23, 24, 28, 34, 32, 10, 20, 38, 37, 28, 38, 34, 18, 24, 24, 9, 23, 24, 28, 34, 32, 10, 20, 38, 37, 28, 38, 34, 18, 24, 24, 9, 23, 24, 28, 34 };

				// Store the original signal
				finalResults["Original"] = signal;

				// Select the wavelets are going to be used
				wavelets::init( { "haar", "daub42", "daub54" });

				// Calculate the maximum levels of decomposition
				int level = std::log2(signal.size());

				// iterates over all wavelets types
				for (std::pair<std::string, std::vector<double>> v : wavelets::all())
				{
					finalResults[v.first] = waveletAnaliticFunction(signal, signal.size(), v.second, level);
					finalResults[v.first][0] = 0;
					finalResults[v.first][1] = 0;
				}

				// Plot the results
				plotResults(finalResults, level, signal.size());
			}
	};
}

#endif /* SRC_WAVELETEXPERIMENTS_04_EXPERIMENT04_CPP_ */

