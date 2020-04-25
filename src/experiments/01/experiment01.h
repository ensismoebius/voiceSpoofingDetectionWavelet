/**
 * @author André Furlan
 * @email ensismoebius@gmail.com
 * This whole project are under GPLv3, for
 * more information read the license file
 *
 * 24 de abr de 2020
 *
 */

#ifndef SRC_EXPERIMENTS_01_EXPERIMENTS01_CPP_
#define SRC_EXPERIMENTS_01_EXPERIMENTS01_CPP_

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

			static void init(std::vector<double> wavelet, wavelets::TransformMode mode, BARK_MEL barkOrMel);

			/**
			 * Analityc function which performs an wavelet transform
			 * of the value and calculate the energies based on MEL
			 * or BARK intervals
			 * @param signal
			 * @param signalLength
			 * @param samplingRate
			 * @param path
			 */
			static void waveletAnaliticFunction(std::vector<double> &signal, int &signalLength, unsigned int samplingRate, std::string path);
	};

	/**
	 * Plot the results
	 * @param data
	 */
	void plotResults(std::vector<double> data);

	void perform(char *args[]);

}
#endif /* SRC_EXPERIMENTS_01_EXPERIMENTS01_CPP_ */
