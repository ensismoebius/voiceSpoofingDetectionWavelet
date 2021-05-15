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

	void normalizeVector(double *signal, int signalLength)
	{

		double sum = 0;

		for (int i = 0; i < signalLength; ++i)
		{
			sum += signal[i];
		}

		for (int i = 0; i < signalLength; ++i)
		{
			signal[i] /= sum;
		}

	}

	void normalizeVector(std::vector<double> &signal)
	{

		double sum = 0;

		for (unsigned int i = 0; i < signal.size(); ++i)
		{
			sum += signal[i];
		}

		for (unsigned int i = 0; i < signal.size(); ++i)
		{
			signal[i] /= sum;
		}

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

	void discreteCosineTransform(std::vector<double> vector)
	{

		double sum;
		double multi;
		long N = vector.size();
		std::vector<double> F(vector.size());

		for (int u = 0; u < N; u++)
		{

			sum = 0.0;
			multi = u == 0 ? 1.0 / std::sqrt(2.0) : 1.0;

			for (int i = 0; i < N; i++)
			{
				sum += multi * std::cos(((M_PI * u) / (2.0 * N)) * (2.0 * i + 1)) * vector.at(u);
			}
			F.at(u) = sum;
		}

		double maior = 0;
		maior = F.at(1);

		for (int i = 2; i < N; i++)
		{

			if (F.at(i) > maior)
			{
				maior = F.at(i);
			}
		}

		for (int i = 1; i < N; i++)
		{
			F.at(i) /= maior;
		}

		for (int i = 0; i < N; i++)
		{
			vector.at(i) = F.at(i);
		}

	}

	void scaleMatrix(std::vector<std::vector<double>> &matrix)
	{

		// Points to the best line that can nullify our values
		unsigned int bestLineForSubtration = 0;

		// Selecting the line on which we have the value to nullyfy
		for (unsigned int lineIndex = 1; lineIndex < matrix.size(); lineIndex++)
		{

			// Selecting the column on which we have the value to nullyfy
			for (unsigned columnIndex = 0; columnIndex < lineIndex; columnIndex++)
			{

				// If this value is already zero then we are ok, move on
				if (matrix[lineIndex][columnIndex] == 0) continue;

				// Otherwise we must find the best line for subtraction
				bestLineForSubtration = 0;
				for (; bestLineForSubtration < matrix.size(); bestLineForSubtration++)
				{

					// The line must have an value diferent of
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

		// loop over all matrix lines from botton to up
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
			//	-Take the rigth side of the equation (the number) -> matrix[li][colums - 1]
			//	-Take the value multipling the incognito variable -> matrix[li][li]
			//	-Make a substituition with previous result (the loop above)
			//	-Then sum the substitutions with the right side of equation and divide by incognito
			result[li] = (matrix[li][colums - 1] + temp) / matrix[li][li];
		}

		return result;
	}

}
