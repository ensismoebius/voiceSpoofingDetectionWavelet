/**
 * @author André Furlan
 * @email ensismoebius@gmail.com
 * This whole project are under GPLv3, for
 * more information read the license file
 *
 * 29 de mar de 2020
 *
 */

#include "linearAlgebra.h"

#include <algorithm>
#include <cmath>

namespace linearAlgebra {

	long double dotProduct(std::vector<double> a, std::vector<double> b) {

		long double product = 0;

		// Loop for calculate cot product
		for (unsigned int i = 0; i < a.size(); i++) {
			product = product + a.at(i) * b.at(i);
		}
		return product;
	}

	double* calcOrthogonalVector(double *originalVector, int vectorSize) {

		double *finalResult = new double[vectorSize];

		int middleSignalIndex = vectorSize / 2;
		double tempVar;
		double inverter = 1.0;

		for (int i = middleSignalIndex; i < vectorSize; ++i) {
			tempVar = originalVector[i];

			finalResult[i] = originalVector[vectorSize - i - 1] * (-inverter);
			finalResult[vectorSize - i - 1] = tempVar * inverter;
			inverter *= -1.0;
		}

		return finalResult;
	}

	std::vector<double> calcOrthogonalVector(std::vector<double> &vector) {
		std::vector<double> result(vector.size());
		double multiplier = 1;

		for (int index = vector.size() - 1; index >= 0; index--) {
			result.at(vector.size() - (index + 1)) = vector.at(index) * multiplier;
			multiplier *= -1;
		}
		return result;
	}

	void normalizeVector(double *signal, int signalLength) {

		double sum = 0;

		for (int i = 0; i < signalLength; ++i) {
			sum += signal[i];
		}

		for (int i = 0; i < signalLength; ++i) {
			signal[i] /= sum;
		}

	}

	void normalizeVector(std::vector<double> &signal) {

		double sum = 0;

		for (unsigned int i = 0; i < signal.size(); ++i) {
			sum += signal[i];
		}

		for (unsigned int i = 0; i < signal.size(); ++i) {
			signal[i] /= sum;
		}

	}

	bool convolution(double *data, int dataLength, double *kernel, int kernelSize) {
		int i, j, k;

		double *convolutedSignal = new double[dataLength];

		// check validity of params
		if (!data || !convolutedSignal || !kernel) return false;
		if (dataLength <= 0 || kernelSize <= 0) return false;

		// start convolution from out[kernelSize-1] to out[dataSize-1] (last)
		for (i = kernelSize - 1; i < dataLength; ++i) {
			convolutedSignal[i] = 0;                             // init to 0 before accumulate

			for (j = i, k = 0; k < kernelSize; --j, ++k) {
				convolutedSignal[i] += data[j] * kernel[k];
			}
		}

		// convolution from out[0] to out[kernelSize-2]
		for (i = 0; i < kernelSize - 1; ++i) {
			convolutedSignal[i] = 0;                             // init to 0 before sum

			for (j = i, k = 0; j >= 0; --j, ++k) {
				convolutedSignal[i] += data[j] * kernel[k];
			}
		}

		std::copy(convolutedSignal, convolutedSignal + dataLength, data);

		delete[] convolutedSignal;

		return true;
	}

	void discreteCosineTransform(double *vector, long vectorLength) {

		long N = vectorLength;
		double sum;
		double multi;
		double F[vectorLength];

		for (int u = 0; u < N; u++) {

			sum = 0.0;
			multi = u == 0 ? 1.0 / std::sqrt(2.0) : 1.0;

			for (int i = 0; i < N; i++) {
				sum += multi * std::cos(((M_PI * u) / (2.0 * N)) * (2.0 * i + 1)) * vector[u];
			}
			F[u] = sum;
		}

		double maior = 0;
		maior = F[1];

		for (int i = 2; i < N; i++) {

			if (F[i] > maior) {
				maior = F[i];
			}
		}

		for (int i = 1; i < N; i++) {
			F[i] /= maior;
		}

		for (int i = 0; i < N; i++) {
			vector[i] = F[i];
		}

	}
}
