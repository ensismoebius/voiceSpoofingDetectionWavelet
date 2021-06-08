/**
 * @author André Furlan
 * @email ensismoebius@gmail.com
 * This whole project are under GPLv3, for
 * more information read the license file
 *
 * 29 de mar de 2020
 *
 */
#ifndef SRC_LIB_LINEARALGEBRA_LINEARALGEBRA_H_
#define SRC_LIB_LINEARALGEBRA_LINEARALGEBRA_H_
#include <vector>

namespace linearAlgebra
{

	/**
	 * Calculates the derivative of a vector
	 * and change the given vector
	 * @param vector - A reference to the vector to be derived
	 * @param level - Amount of derivatives over the same vector
	 * @return resulting vector (optional)
	 */
	std::vector<long double> derivative(std::vector<long double> &vector, unsigned int level = 1);

	/**
	 * Function that return dot product of two vectors.
	 * @param a
	 * @param b
	 * @return
	 */
	long double dotProduct(std::vector<long double> a, std::vector<long double> b);

	/**
	 * Create an orthogonal vector given another one
	 * @param originalVector
	 * @param vectorSize
	 * @return
	 */
	long double* calcOrthogonalVector(long double *originalVector, int vectorSize);

	/**
	 * Given a vector calculates the corresponding orthogonal vector
	 * @param vector - source vector
	 * @return orthogonal vector
	 */
	std::vector<long double> calcOrthogonalVector(std::vector<long double> &vector);

	/**
	 * Normalize the vector
	 * @param signal
	 * @param signalLength
	 * @param lowerLimit
	 * @param upperLimit
	 */
	void normalizeVectorToRange(long double *signal, int signalLength, long double lowerLimit, long double upperLimit);

	/**
	 * Normalize the vector
	 * @param signal
	 * @param lowerLimit
	 * @param upperLimit
	 */
	void normalizeVectorToRange(std::vector<long double> &signal, long double lowerLimit, long double upperLimit);

	/**
	 * Normalize the vector to sum 1
	 * @param signal
	 * @param signalLength
	 */
	void normalizeVectorToSum1(long double *signal, int signalLength);

	/**
	 * Normalize the vector to sum 1
	 * @param signal
	 */
	void normalizeVectorToSum1(std::vector<long double> &signal);

	/**
	 * Convolute a signal with a filter (kernel)
	 * @param data
	 * @param dataLength
	 * @param kernel
	 * @param kernelSize
	 * @return
	 */
	bool convolution(long double *data, int dataLength, long double *kernel, int kernelSize);

	/**
	 * Performs a DCT on vector
	 * @param vector
	 * @param vectorLength
	 */
	void discreteCosineTransform(long double *vector, long vectorLength);

	/**
	 * Performs a DCT on vector
	 * @param vector
	 */
	void discreteCosineTransform(std::vector<long double> &vector);

	/**
	 * Scales a given matrix
	 * @param matrix
	 */
	void scaleMatrix(std::vector<std::vector<long double>> &matrix);

	/**
	 * Solves the linear system represented by
	 * the matrix and return the results.
	 * The matrix MUST be scaled before!!!
	 * @see scaleMatrix
	 * @param matrix - A scaled matrix
	 * @return a vetor with results
	 */
	std::vector<long double> solveMatrix(std::vector<std::vector<long double>> &matrix);

	/**
	 * Normalize the vector to sum 1 and guarantees
	 * that all values are positives
	 * @param signal
	 */
	void normalizeVectorToSum1AllPositive(std::vector<long double> &signal);

	/**
	 * Normalize the vector to sum 1 and guarantees
	 * that all values are positives
	 * @param signal
	 * @param signalLength
	 */
	void normalizeVectorToSum1AllPositive(long double *signal, int signalLength);

	/**
	 * Resizes a vector in a centered way
	 * @example std::vector<long double> vec{ 1, 2, 3, 4, 5 };
	 * resizeCentered(vec, 9);
	 * we get: { 0, 0, 1, 2, 3, 4, 5, 0, 0 }
	 * resizeCentered(vec, 3);
	 * we get: { 2, 3, 4 }
	 * @param vector
	 * @param newSize
	 */
	void resizeCentered(std::vector<long double> &vector, int newSize, long double defaultValue = 0);
}
#endif /* SRC_LIB_LINEARALGEBRA_LINEARALGEBRA_H_ */
