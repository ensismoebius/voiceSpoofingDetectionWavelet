/*
 * waveletCoeficients.cpp
 *
 *  Created on: 29 de mar de 2020
 *      Author: ensismoebius
 */

#include "waveletCoeficients.h"

namespace wavelets {

	std::vector<double> calcOrthogonal(std::vector<double> &vector) {
		std::vector<double> result(vector.size());
		double multiplier = -1;

		for (int index = vector.size() - 1; index >= 0; index--) {
			result.at(vector.size() - (index + 1)) = vector.at(index) * multiplier;
			multiplier *= -1;
		}
		return result;
	}
}
