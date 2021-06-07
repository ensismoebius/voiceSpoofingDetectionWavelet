/**
 * @author André Furlan
 * @email ensismoebius@gmail.com
 * This whole project are under GPLv3, for
 * more information read the license file
 *
 * 12 de abr de 2020
 */

#ifndef SRC_LIB_WAVELET_WAVELETTRANSFORMRESULTS_H_
#define SRC_LIB_WAVELET_WAVELETTRANSFORMRESULTS_H_

#include <cmath>
#include <vector>
#include <stdexcept>

namespace wavelets
{
	class WaveletTransformResults
	{

		public:
			/**
			 * If true the results comes from a
			 * packet wavelet transform
			 */
			bool packet = false;

			/**
			 * The levels of transformations
			 * we have done to the signal
			 */
			unsigned int levelsOfTransformation = 0;

			/**
			 * The transformed signal
			 */
			std::vector<long double> transformedSignal;

			WaveletTransformResults(unsigned int maxItens = 0);

			/**
			 * Extracts the approximation and details from the wavelet transformation
			 * @param detailIndex -1: Return the whole transformed signal
			 * @param detailIndex 0: Extracts the aproximation
			 * @param detailIndex 1 or more: Extracts the corresponding detail
			 * @return Whole transformed signal, aproximation or details
			 */
			std::vector<long double> getWaveletTransforms(int detailIndex = -1);

			/**
			 * Extracts the values of a wavelet packet transformation
			 * differently from @getWaveletTransforms it DO NOT returns
			 * the details of transformation, otherwise, returns the
			 * generated chunks of the transformed signal
			 * @param partIndex : A value from 0 up to @getWaveletPacketAmountOfParts
			 * @return the requested chunk
			 */
			std::vector<long double> getWaveletPacketTransforms(unsigned int partIndex);

			/**
			 * Calculate the maximum number of generated
			 * parts in a packet wavelet transform
			 * @return maximum number of generated parts
			 */
			unsigned int getWaveletPacketAmountOfParts();

			/**
			 * Static version of @getWaveletPacketTransforms(unsigned int partIndex)
			 * Extracts the values of a wavelet packet transformation
			 * differently from @getWaveletTransforms it DO NOT returns
			 * the details of transformation, otherwise, returns the
			 * generated chunks of the transformed signal
			 * USE ONLY WITH PACKET WAVELETS!!
			 * @param transformedSignal : vector with transformed signal
			 * @param partIndex : A value from 0 up to @getWaveletPacketAmountOfParts
			 * @param levelsOfTransformation : levels of transformation of the signal
			 * @return the requested chunk
			 */
			static std::vector<long double> getWaveletPacketTransforms(std::vector<long double> transformedSignal, unsigned int partIndex, unsigned int levelsOfTransformation);

			/**
			 * Static version of @getWaveletPacketAmountOfParts()
			 * Calculate the maximum number of generated
			 * parts in a packet wavelet transform given
			 * the levels of transformations performed
			 * USE ONLY WITH PACKET WAVELETS!!
			 * @param levelsOfTransformation
			 * @return maximum number of generated parts
			 */
			static unsigned int getWaveletPacketAmountOfParts(unsigned int levelsOfTransformation);
	};
}

#endif /* SRC_LIB_WAVELET_WAVELETTRANSFORMRESULTS_H_ */

