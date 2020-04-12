/*
 * waveletCoeficients.cpp
 *
 *  Created on: 29 de mar de 2020
 *      Author: ensismoebius
 */

#include <vector>

#include "WaveletTransformResults.cpp"
#include "../linearAlgebra/linearAlgebra.h"

namespace wavelets {

	WaveletTransformResults malat(std::vector<double> signal, std::vector<double> lowpassfilter, unsigned int level = 1, unsigned int maxItens = 0, bool packet = false) {

		//If maxitens is not informed then get the full signal size
		if (maxItens == 0) maxItens = signal.size();

		// Get the highpass filter based on lowṕass filter
		std::vector<double> highpassfilter = linearAlgebra::calcOrthogonalVector(lowpassfilter);

		// Store the final results
		WaveletTransformResults results(maxItens);

		double lowPassSum = 0;
		double highPassSum = 0;
		unsigned int signalIndex = 0;

		//Translate de filters over the signal
		for (unsigned int translation = 0; translation < maxItens; translation += 2) {

			lowPassSum = 0;
			highPassSum = 0;

			// Make the sums for lowpass and highpass (i.e. apply the filters)
			for (unsigned int filterIndex = 0; filterIndex < lowpassfilter.size(); ++filterIndex) {

				signalIndex = (translation + filterIndex) % maxItens;

				lowPassSum += signal.at(signalIndex) * lowpassfilter.at(filterIndex);
				highPassSum += signal.at(signalIndex) * highpassfilter.at(filterIndex);
			}

			// Stores the values according to Malat's algorithm
			results.transformedSignal.at(translation / 2) = lowPassSum;
			results.transformedSignal.at((translation / 2) + (maxItens / 2)) = highPassSum;

		}

		// If there is more levels to made the transform do it!
		if (maxItens > 2 && level > 1) {

			// The next level uses only half of the resulting transfomed signal
			// that why the "maxItens / 2"
			WaveletTransformResults tmp = malat(results.transformedSignal, lowpassfilter, level - 1, maxItens / 2, packet);

			// Write the result
			for (unsigned int i = 0; i < tmp.transformedSignal.size(); ++i) {
				results.transformedSignal.at(i) = tmp.transformedSignal.at(i);
			}
		}

		// Return the whole thing
		return results;
	}
}
