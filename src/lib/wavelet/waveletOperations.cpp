/*
 * waveletCoeficients.cpp
 *
 *  Created on: 29 de mar de 2020
 *      Author: ensismoebius
 */

#include <vector>

#include "waveletOperations.h"
#include "WaveletTransformResults.cpp"
#include "../linearAlgebra/linearAlgebra.h"

namespace wavelets {

	WaveletTransformResults malat(std::vector<double> signal, std::vector<double> lowpassfilter, TransformMode mode, unsigned int level, unsigned int maxItens, bool highPassBranch) {

		//If maxitens is not informed then get the full signal size
		if (maxItens == 0) {
			maxItens = signal.size();
		} else {
			// The number of itens must be equal or less than the signal length
			if (maxItens > signal.size()) {
				throw std::runtime_error("The number of itens must be equal or less than the signal length");
			}

			// The number of itens must be equal or less than the half of signal length
			// when in the high pass branch of the signal (used in the wavelet packet
			// starting in the second transformation level)
			if (highPassBranch && (maxItens > signal.size() / 2)) {
				throw std::runtime_error("The number of itens must be equal or less than the half of signal length when in the high pass branch of the signal");
			}
		}

		// Get the highpass filter based on lowpass filter
		std::vector<double> highpassfilter = linearAlgebra::calcOrthogonalVector(lowpassfilter);

		// Create the storage for the final results
		WaveletTransformResults results(maxItens);

		double lowPassSum = 0;
		double highPassSum = 0;
		unsigned int signalIndex = 0;

		/*
		 * The way we apply the filters and store the results for the
		 * highpass and lowpass portions of the signal is different
		 */
		if (highPassBranch) {

			//Translate the filters over the signal
			for (unsigned int translation = 0; translation < maxItens; translation += 2) {

				lowPassSum = 0;
				highPassSum = 0;

				// Make the sums for lowpass and highpass (i.e. apply the filters)
				for (unsigned int filterIndex = 0; filterIndex < lowpassfilter.size(); ++filterIndex) {

					// This part cooresponds to the "wrap around" part of Mallat's algorithm
					signalIndex = (translation + filterIndex) % maxItens;

					/*
					 * When in highpass branch of the signal we just want the
					 * second half of the signal (signalIndex + maxItens). This
					 * is used only with wavelet packet transformations
					 */
					lowPassSum += signal.at(signalIndex + maxItens) * lowpassfilter.at(filterIndex);
					highPassSum += signal.at(signalIndex + maxItens) * highpassfilter.at(filterIndex);
				}

				// Stores the values according to Malat's algorithm
				/*
				 * If we are decomposing the highpass branch then we need to swap the
				 * high pass and low pass filtered signals in order to maintain the
				 * signal order in the frequency domain. This is used only with
				 * wavelet packet transformations
				 */
				results.transformedSignal.at(translation / 2) = highPassSum;
				results.transformedSignal.at((translation / 2) + (maxItens / 2)) = lowPassSum;
			}
		} else {

			//Translate the filters over the signal
			for (unsigned int translation = 0; translation < maxItens; translation += 2) {

				lowPassSum = 0;
				highPassSum = 0;

				// Make the sums for lowpass and highpass (i.e. apply the filters)
				for (unsigned int filterIndex = 0; filterIndex < lowpassfilter.size(); ++filterIndex) {

					// This part cooresponds to the "wrap around" part of Mallat's algorithm
					signalIndex = (translation + filterIndex) % maxItens;

					/* When in lowpass branch of the signal we just want the
					 * first half of the signal (signalIndex)
					 */
					lowPassSum += signal.at(signalIndex) * lowpassfilter.at(filterIndex);
					highPassSum += signal.at(signalIndex) * highpassfilter.at(filterIndex);
				}

				// Stores the values according to Malat's algorithm
				/*
				 * If we are at the lowpass portion of the signal
				 * then just store the values as the regular wavelet transform
				 */
				results.transformedSignal.at(translation / 2) = lowPassSum;
				results.transformedSignal.at((translation / 2) + (maxItens / 2)) = highPassSum;
			}
		}

		// If there is more levels to made the transform do it!
		if (maxItens > 2 && level > 1) {

			/*
			 * The lowpass signal decomposition is made in both modes: PACKET_WAVELET
			 * and REGULAR_WAVELET.
			 * The next level uses only half of the resulting transfomed signal
			 * that why the "maxItens / 2"
			 */
			WaveletTransformResults lowpassBranchFiltered = malat(results.transformedSignal, lowpassfilter, mode, level - 1, maxItens / 2, false);

			// Used only when in wavelet packet transform
			if (mode == PACKET_WAVELET) {

				// The next level uses only half of the resulting transfomed signal
				// that why the "maxItens / 2"
				WaveletTransformResults highPassBranchFiltered = malat(results.transformedSignal, lowpassfilter, mode, level - 1, maxItens / 2, true);

				// Write the result
				for (unsigned int i = 0; i < highPassBranchFiltered.transformedSignal.size(); ++i) {
					results.transformedSignal.at(i + results.transformedSignal.size() / 2) = highPassBranchFiltered.transformedSignal.at(i);
				}
			}

			// Write the result
			for (unsigned int i = 0; i < lowpassBranchFiltered.transformedSignal.size(); ++i) {
				results.transformedSignal.at(i) = lowpassBranchFiltered.transformedSignal.at(i);
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
