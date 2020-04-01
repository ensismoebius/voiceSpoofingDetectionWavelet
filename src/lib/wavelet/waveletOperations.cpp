/*
 * waveletCoeficients.cpp
 *
 *  Created on: 29 de mar de 2020
 *      Author: ensismoebius
 */

#include <vector>

namespace wavelets {

	std::vector<long double> calcOrthogonal(std::vector<long double> &vector) {
		std::vector<long double> result(vector.size());
		double multiplier = 1;

		for (int index = vector.size() - 1; index >= 0; index--) {
			result.at(vector.size() - (index + 1)) = vector.at(index) * multiplier;
			multiplier *= -1;
		}
		return result;
	}

	std::vector<long double> malat(std::vector<long double> signal, std::vector<long double> lowpassfilter, unsigned int level = 1, unsigned int maxItens = 0) {

		if (maxItens == 0) maxItens = signal.size();

		std::vector<long double> highpassfilter = wavelets::calcOrthogonal(lowpassfilter);

		std::vector<long double> results(maxItens);

		long double lowPassSum = 0;
		long double highPassSum = 0;
		unsigned int signalIndex = 0;

		for (unsigned int translation = 0; translation < maxItens; translation += 2) {

			lowPassSum = 0;
			highPassSum = 0;

			// Make the sums for lowpass and highpass
			for (unsigned int filterIndex = 0; filterIndex < lowpassfilter.size(); ++filterIndex) {

				signalIndex = (translation + filterIndex) % maxItens;

				lowPassSum += signal.at(signalIndex) * lowpassfilter.at(filterIndex);
				highPassSum += signal.at(signalIndex) * highpassfilter.at(filterIndex);
			}

			results.at(translation / 2) = lowPassSum;
			results.at((translation / 2) + (maxItens / 2)) = highPassSum;

		}

		if (maxItens > 2 && level > 1) {
			std::vector<long double> tmp = malat(results, lowpassfilter, level - 1, maxItens / 2);

			for (unsigned int i = 0; i < tmp.size(); ++i) {
				results.at(i) = tmp.at(i);
			}
		}

		return results;
	}
}
