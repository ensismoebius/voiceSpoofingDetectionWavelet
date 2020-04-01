/*
 * waveletCoeficients.cpp
 *
 *  Created on: 29 de mar de 2020
 *      Author: ensismoebius
 */

#include "waveletCoeficients.h"

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
}
