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

	WaveletTransformResults malat(std::vector<double> signal, std::vector<double> lowpassfilter, unsigned int level = 1, unsigned int maxItens = 0, bool highPassBranch = false, bool packet = false) {

		//If maxitens is not informed then get the full signal size
		if (maxItens == 0) maxItens = signal.size();

		// Get the highpass filter based on lowpass filter
		std::vector<double> highpassfilter = linearAlgebra::calcOrthogonalVector(lowpassfilter);

		// Stores the final results
		WaveletTransformResults results(maxItens);

		double lowPassSum = 0;
		double highPassSum = 0;
		unsigned int signalIndex = 0;

		//Translate the filters over the signal
		for (unsigned int translation = 0; translation < maxItens; translation += 2) {

			lowPassSum = 0;
			highPassSum = 0;

			// Make the sums for lowpass and highpass (i.e. apply the filters)
			for (unsigned int filterIndex = 0; filterIndex < lowpassfilter.size(); ++filterIndex) {

				signalIndex = (translation + filterIndex) % maxItens;

				lowPassSum += signal.at(signalIndex) * lowpassfilter.at(filterIndex);
				highPassSum += signal.at(signalIndex) * highpassfilter.at(filterIndex);
			}

			// TODO for the recursion we must create two vectors: One for lowpass filtered signal and another for highpass filtered signal
			// TODO AND do the recursion BEFORE alter the results, i think that is it... 

			// Stores the values according to Malat's algorithm
			if (highPassBranch) {
				/*
				 * If we are decomposing the highpass branch then we need to swap the
				 * high pass and low pass filtered signals in order to maintain the
				 * signal order in the frequency domain
				 */
				results.transformedSignal.at(translation / 2) = highPassSum;
				results.transformedSignal.at((translation / 2) + (maxItens / 2)) = lowPassSum;
			} else {
				/*
				 * If we are at the lowpass portion of the signal
				 * then just perform the regular wavelet transform
				 */
				results.transformedSignal.at(translation / 2) = lowPassSum;
				results.transformedSignal.at((translation / 2) + (maxItens / 2)) = highPassSum;
			}

		}

		// If there is more levels to made the transform do it!
		if (maxItens > 2 && level > 1) {

			// The next level uses only half of the resulting transfomed signal
			// that why the "maxItens / 2"
			WaveletTransformResults lowpassBranchFiltered = malat(results.transformedSignal, lowpassfilter, level - 1, maxItens / 2, false, packet);

			// Write the result
			for (unsigned int i = 0; i < lowpassBranchFiltered.transformedSignal.size(); ++i) {
				results.transformedSignal.at(i) = lowpassBranchFiltered.transformedSignal.at(i);
			}

			// Used only when in wavelet packet transform
			if (packet) {
				// The next level uses only half of the resulting transfomed signal
				// that why the "maxItens / 2"
				WaveletTransformResults highPassBranchFiltered = malat(results.transformedSignal, lowpassfilter, level - 1, maxItens / 2, true, packet);

				// Write the result
				for (unsigned int i = 0; i < highPassBranchFiltered.transformedSignal.size(); ++i) {
					results.transformedSignal.at(i + results.transformedSignal.size() / 2) = highPassBranchFiltered.transformedSignal.at(i);
				}
			}

			// Add the transformation levels done in recursion to current level
			// Even when we do an wavelet transform the way we count it remains
			results.levelsOfTransformation += lowpassBranchFiltered.levelsOfTransformation;
		}

		// Increase the levels of trasformation
		results.levelsOfTransformation++;

		// Return the whole thing
		return results;
	}
}
