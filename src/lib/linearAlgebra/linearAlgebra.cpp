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

namespace linearAlgebra
{

	std::vector<double> derivative(std::vector<double> &vector, unsigned int level)
	{

		if (vector.size() < 2)
		{
			return std::vector<double>( { vector.at(0) });
		}

		for (unsigned int i = 0; i < vector.size() - 1; ++i)
		{
			vector.at(i) = vector.at(i + 1) - vector.at(i);
		}

		level--;

		vector.resize(vector.size() - 1);

		if (level > 0)
		{
			vector = derivative(vector, level);
		}
		return vector;
	}

	long double dotProduct(std::vector<double> a, std::vector<double> b)
	{

		long double product = 0;

		// Loop for calculate cot product
		for (unsigned int i = 0; i < a.size(); i++)
		{
			product = product + a.at(i) * b.at(i);
		}
		return product;
	}

	double* calcOrthogonalVector(double *originalVector, int vectorSize)
	{

		double *finalResult = new double[vectorSize];

		int middleSignalIndex = vectorSize / 2;
		double tempVar;
		double inverter = 1.0;

		for (int i = middleSignalIndex; i < vectorSize; ++i)
		{
			tempVar = originalVector[i];

			finalResult[i] = originalVector[vectorSize - i - 1] * (-inverter);
			finalResult[vectorSize - i - 1] = tempVar * inverter;
			inverter *= -1.0;
		}

		return finalResult;
	}

	std::vector<double> calcOrthogonalVector(std::vector<double> &vector)
	{
		std::vector<double> result(vector.size());
		double multiplier = 1;

		for (int index = vector.size() - 1; index >= 0; index--)
		{
			result.at(vector.size() - (index + 1)) = vector.at(index) * multiplier;
			multiplier *= -1;
		}
		return result;
	}

	void normalizeVectorToSum1(double *signal, int signalLength)
	{
		double sum = 0;

		for (int i = 0; i < signalLength; ++i)
			sum += signal[i];

		for (int i = 0; i < signalLength; ++i)
			signal[i] /= sum;
	}

	void normalizeVectorToSum1(std::vector<double> &signal)
	{
		double sum = 0;

		for (double &v : signal)
			sum += v;

		for (double &v : signal)
			v /= sum;
	}

	void normalizeVectorToSum1AllPositive(std::vector<double> &signal)
	{
		long double min = signal[0];
		long double max = min;
		long double sum = 0;

		for (double &v : signal)
		{
			if (v < min) min = v;
			if (v > max) max = v;
			sum += v;
		}

		if (min < 0)
		{
			sum = 0;
			for (double &v : signal)
				sum += v += std::abs(min) + (long double) 1;
		}

		for (double &v : signal)
			v /= sum;
	}

	void normalizeVectorToSum1AllPositive(double *signal, int signalLength)
	{
		long double min = signal[0];
		long double max = min;
		long double sum = 0;

		for (int i = 0; i < signalLength; ++i)
		{
			if (signal[i] < min) min = signal[i];
			if (signal[i] > max) max = signal[i];
			sum += signal[i];
		}

		if (min < 0)
		{
			sum = 0;
			for (int i = 0; i < signalLength; ++i)
				sum += signal[i] += std::abs(min) + (long double) 1;
		}

		for (int i = 0; i < signalLength; ++i)
			signal[i] /= sum;
	}

	void normalizeVectorToRange(double *signal, int signalLength, double lowerLimit, double upperLimit)
	{

		if (lowerLimit >= upperLimit)
		{
			throw std::runtime_error("lowerLimit MUST be bigger than upperLimit");
		}

		/*
		 * Keep this comment!
		 *
		 *	double min, max;
		 *	min = max = signal[0];
		 *
		 *	for (auto &v : signal)
		 *	{
		 *		// Minimum
		 *		if (v < min) min = v;
		 *		// Maximum
		 *		if (v > max) max = v;
		 *	}
		 *
		 *	// Normalize between 0 and 1
		 *	double rangeVal = max - min;
		 *	for (auto &v : signal)
		 *		v = (v - min) / rangeVal;
		 *
		 *	// Then scale to [lowerLimit,upperLimit]:
		 *	double rangeLim = upperLimit - lowerLimit;
		 *	for (auto &v : signal)
		 *		v = (v * rangeLim) + lowerLimit;
		 */

		// The code bellow do the same as the commented code above
		double min, max;
		min = max = signal[0];

		for (int i = 0; i < signalLength; ++i)
		{
			// Minimum
			if (signal[i] < min) min = signal[i];
			// Maximum
			if (signal[i] > max) max = signal[i];
		}

		double rangeVal = max - min;
		double rangeLim = upperLimit - lowerLimit;
		for (int i = 0; i < signalLength; ++i)
			signal[i] = (((signal[i] - min) / rangeVal) * rangeLim) + lowerLimit;
	}

	void normalizeVectorToRange(std::vector<double> &signal, double lowerLimit, double upperLimit)
	{
		if (lowerLimit >= upperLimit)
		{
			throw std::runtime_error("lowerLimit MUST be bigger than upperLimit");
		}
		/*
		 * Keep this comment!
		 *
		 *	double min, max;
		 *	min = max = signal[0];
		 *
		 *	for (auto &v : signal)
		 *	{
		 *		// Minimum
		 *		if (v < min) min = v;
		 *		// Maximum
		 *		if (v > max) max = v;
		 *	}
		 *
		 *	// Normalize between 0 and 1
		 *	double rangeVal = max == min ? 1 : max - min;
		 *	for (auto &v : signal)
		 *		v = (v - min) / rangeVal;
		 *
		 *	// Then scale to [lowerLimit,upperLimit]:
		 *	double rangeLim = upperLimit - lowerLimit;
		 *	for (auto &v : signal)
		 *		v = (v * rangeLim) + lowerLimit;
		 */

		// The code bellow do the same as the commented code above
		double min, max;
		min = max = signal[0];

		for (auto &v : signal)
		{
			// Minimum
			if (v < min) min = v;
			// Maximum
			if (v > max) max = v;
		}

		double rangeVal = max == min ? 1 : max - min;
		double rangeLim = upperLimit - lowerLimit;
		for (auto &v : signal)
			v = (((v - min) / rangeVal) * rangeLim) + lowerLimit;
	}

	bool convolution(double *data, int dataLength, double *kernel, int kernelSize)
	{
		int i, j, k;

		double *convolutedSignal = new double[dataLength];

		// check validity of params
		if (!data || !convolutedSignal || !kernel) return false;
		if (dataLength <= 0 || kernelSize <= 0) return false;

		// start convolution from out[kernelSize-1] to out[dataSize-1] (last)
		for (i = kernelSize - 1; i < dataLength; ++i)
		{
			convolutedSignal[i] = 0;                             // init to 0 before accumulate

			for (j = i, k = 0; k < kernelSize; --j, ++k)
			{
				convolutedSignal[i] += data[j] * kernel[k];
			}
		}

		// convolution from out[0] to out[kernelSize-2]
		for (i = 0; i < kernelSize - 1; ++i)
		{
			convolutedSignal[i] = 0;                             // init to 0 before sum

			for (j = i, k = 0; j >= 0; --j, ++k)
			{
				convolutedSignal[i] += data[j] * kernel[k];
			}
		}

		std::copy(convolutedSignal, convolutedSignal + dataLength, data);

		delete[] convolutedSignal;

		return true;
	}

	inline double getAlphaK(unsigned int k, unsigned int N)
	{
		return k == 0 ? std::sqrt(1.0 / N) : std::sqrt(2.0 / N);
	}

	void discreteCosineTransform(std::vector<double> &signal)
	{
		std::vector<double> res(signal.size());
		unsigned int N = signal.size();
		double sum = 0;

		for (unsigned int k = 0; k < N; ++k)
		{

			sum = 0;
			for (unsigned int n = 0; n < N; ++n)
			{
				sum += signal[n] * std::cos(((2.0 * n + 1.0) * M_PIl * k) / (2.0 * N));
			}

			res[k] = getAlphaK(k, N) * sum;
		}

		signal = res;
	}

	void discreteCosineTransform(double *signal, long signalLength)
	{
		double *res = new double[signalLength];
		unsigned int N = signalLength;
		double sum = 0;

		for (unsigned int k = 0; k < N; ++k)
		{
			sum = 0;
			for (unsigned int n = 0; n < N; ++n)
			{
				sum += signal[n] * std::cos(((2.0 * n + 1.0) * M_PIl * k) / (2.0 * N));
			}

			res[k] = getAlphaK(k, N) * sum;
		}

		for (unsigned int i = 0; i < N; ++i)
			signal[i] = res[i];

		delete[] res;
	}

	void scaleMatrix(std::vector<std::vector<double>> &matrix)
	{

		// Points to the best line that can nullify our values
		unsigned int bestLineForSubtration = 0;

		// Selecting the line on which we have the value to nullify
		for (unsigned int lineIndex = 1; lineIndex < matrix.size(); lineIndex++)
		{

			// Selecting the column on which we have the value to nullify
			for (unsigned columnIndex = 0; columnIndex < lineIndex; columnIndex++)
			{

				// If this value is already zero then we are ok, move on
				if (matrix[lineIndex][columnIndex] == 0) continue;

				// Otherwise we must find the best line for subtraction
				bestLineForSubtration = 0;
				for (; bestLineForSubtration < matrix.size(); bestLineForSubtration++)
				{

					// The line must have an value different of
					// zero at the position we want nullify
					if (matrix[bestLineForSubtration][columnIndex] == 0) continue;

					// The line must have zeros BEFORE the
					// current position we want to nullify
					bool zeroedBefore = true;
					for (unsigned int ci = 0; ci < columnIndex; ci++)
					{
						if (matrix[bestLineForSubtration][ci] != 0)
						{
							zeroedBefore = false;
							break;
						}
					}
					if (!zeroedBefore) continue;

					// We got it! Stop now!
					break;
				}

				// Ready to calculate the coefficient
				double coef = matrix[lineIndex][columnIndex] / matrix[bestLineForSubtration][columnIndex];

				unsigned int ci = columnIndex;
				matrix[lineIndex][ci] = 0;
				ci++;

				for (; ci < matrix[lineIndex].size(); ci++)
				{
					matrix[lineIndex][ci] -= coef * matrix[bestLineForSubtration][ci];
				}
			}

		}

	}

	std::vector<double> solveMatrix(std::vector<std::vector<double>> &matrix)
	{
		// Used to make the substitutions
		double temp;

		// final result
		std::vector<double> result(matrix.size());

		// The amount of the matrix columns
		unsigned int colums = matrix[0].size();

		// loop over all matrix lines from bottom to up
		for (int li = matrix.size() - 1; li >= 0; li--)
		{

			// Make the substitutions:
			//	-Ignore the incognito variable (all values from main diagonal).-> "ci = li + 1"
			//	-Ignore the right side of the equation.-> "colums - 1"
			temp = 0;
			for (unsigned ci = li + 1; ci < colums - 1; ci++)
			{
				temp -= matrix[li][ci] * result[ci];
			}

			// The result is computed as follows:
			//	-Take the right side of the equation (the number) -> matrix[li][colums - 1]
			//	-Take the value multiplying the incognito variable -> matrix[li][li]
			//	-Make a substitution with previous result (the loop above)
			//	-Then sum the substitutions with the right side of equation and divide by incognito
			result[li] = (matrix[li][colums - 1] + temp) / matrix[li][li];
		}

		return result;
	}

}
