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
	std::vector<double> derivative(std::vector<double> &vector, unsigned int level = 1);

	/**
	 * Function that return dot product of two vectors.
	 * @param a
	 * @param b
	 * @return
	 */
	long double dotProduct(std::vector<double> a, std::vector<double> b);

	/**
	 * Create an orthogonal vector given another one
	 * @param originalVector
	 * @param vectorSize
	 * @return
	 */
	double* calcOrthogonalVector(double *originalVector, int vectorSize);

	/**
	 * Given a vector calculates the corresponding orthogonal vector
	 * @param vector - source vector
	 * @return orthogonal vector
	 */
	std::vector<double> calcOrthogonalVector(std::vector<double> &vector);

	/**
	 * Normalize the vector
	 * @param signal
	 * @param signalLength
	 */
	void normalizeVector(double *signal, int signalLength);

	/**
	 * Normalize the vector
	 * @param signal
	 */
	void normalizeVector(std::vector<double> &signal);

	/**
	 * Convolute a signal with a filter (kernel)
	 * @param data
	 * @param dataLength
	 * @param kernel
	 * @param kernelSize
	 * @return
	 */
	bool convolution(double *data, int dataLength, double *kernel, int kernelSize);

	/**
	 * Perform a DCT on vector
	 * @param vector
	 * @param vectorLength
	 */
	void discreteCosineTransform(double *vector, long vectorLength);

	/**
	 * Perform a DCT on vector
	 * @param vector
	 */
	void discreteCosineTransform(std::vector<double> vector);

	/**
	 * Scales a given matrix
	 * @param matrix
	 */
	void scaleMatrix(std::vector<std::vector<double>> &matrix);

	/**
	 * Solves the linear system represented by
	 * the matrix and return the results.
	 * The matrix MUST be scaled before!!!
	 * @see scaleMatrix
	 * @param matrix - A scaled matrix
	 * @return a vetor with results
	 */
	std::vector<double> solveMatrix(std::vector<std::vector<double>> &matrix);
}
#endif /* SRC_LIB_LINEARALGEBRA_LINEARALGEBRA_H_ */
