/**
 * @author André Furlan
 * @email ensismoebius@gmail.com
 * This whole project are under GPLv3, for
 * more information read the license file
 *
 * 12 de abr de 2020
 */

#ifndef SRC_LIB_WAVELET_WAVELETTRANSFORMRESULTS_CPP_
#define SRC_LIB_WAVELET_WAVELETTRANSFORMRESULTS_CPP_

#include <cmath>
#include <vector>
#include <stdexcept>

class WaveletTransformResults {

	public:
		unsigned int levelsOfTransformation;

		std::vector<double> transformedSignal;

		std::vector<double> getTransformedSignal(unsigned int level = 1) {

			if (level > this->levelsOfTransformation) {
				throw std::runtime_error("There is not a transformations at this level");
			}

			if (level == 1) return this->transformedSignal;

			std::vector<double> levelTransformedSignal;

			unsigned sstart = this->transformedSignal.size() / std::pow(2, level + 1);
			unsigned send = this->transformedSignal.size() / std::pow(2, level);

			for (unsigned int indexRange = sstart; indexRange < send; indexRange++) {
				levelTransformedSignal.push_back(this->transformedSignal.at(indexRange));
			}

			return levelTransformedSignal;
		}
};

#endif /* SRC_LIB_WAVELET_WAVELETTRANSFORMRESULTS_CPP_ */

