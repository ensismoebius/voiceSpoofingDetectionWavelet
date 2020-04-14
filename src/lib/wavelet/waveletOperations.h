/**
 * @author André Furlan
 * @email ensismoebius@gmail.com
 * This whole project are under GPLv3, for
 * more information read the license file
 *
 * 1 de abr de 2020
 */
#ifndef SRC_LIB_WAVELET_WAVELETOPERATIONS_H_
#define SRC_LIB_WAVELET_WAVELETOPERATIONS_H_

#include <vector>

#include "WaveletTransformResults.cpp"

namespace wavelets {

	/**
	 * Indicates what a kind of
	 * transformation must be done
	 */
	enum TransformMode {
		PACKET_WAVELET, REGULAR_WAVELET
	};

	/**
	 * Applies a wavelets transform over a signal using the Mallat's algorithm
	 * @param signal - signal to be transformed
	 * @param lowpassfilter - the wavelet lowpass filter
	 * @param level - levels of the signal decomposition
	 * @param maxItens - the signal upper limit to be processed
	 * @param highPassBranch - true: Do the decomposition in the highpass portion of the signal
	 * (wavelet packet transform). false: Do a regular wavelet transform
	 * @param packet - PACKET_WAVELET: wavelet packet, REGULAR_WAVELET: regular wavelet
	 * @return transformed signal
	 */
	WaveletTransformResults malat(std::vector<double> &signal, std::vector<double> lowpassfilter, TransformMode mode = REGULAR_WAVELET, unsigned int level = 1, unsigned int maxItens = 0, bool highPassBranch = false);
}

#endif /* SRC_LIB_WAVELET_WAVELETOPERATIONS_H_ */
