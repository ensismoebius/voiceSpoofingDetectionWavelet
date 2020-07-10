/**
 * @author André Furlan
 * @email ensismoebius@gmail.com
 * This whole project are under GPLv3, for
 * more information read the license file
 *
 * 8 de jul. de 2020
 *
 */

#include <cmath>
#include <string>
#include <vector>
#include <algorithm>

#include "confusionMatrix.h"

namespace statistics {

	/**
	 * False positive rate
	 * @param fp
	 * @param tn
	 * @return
	 */
	double falsePositiveRate(double fp, double tn) {
		return fp / (tn + fp);
	}

	/**
	 * False positive rate
	 * @param matrix
	 * @return
	 */
	double falsePositiveRate(ConfusionMatrix &matrix) {
		return falsePositiveRate(matrix.falsePositive, matrix.trueNegative);
	}

	/**
	 * False negative rate
	 * @param fn
	 * @param tn
	 * @return
	 */
	double falseNegativeRate(double tp, double fn) {
		return fn / (fn + tp);
	}

	/**
	 * False negative rate
	 * @param matrix
	 * @return
	 */
	double falseNegativeRate(ConfusionMatrix &matrix) {
		return falseNegativeRate(matrix.truePositive, matrix.falseNegative);
	}

	/**
	 * True positive rate
	 * @param tp
	 * @param fn
	 * @return
	 */
	double truePositiveRate(double tp, double fn) {
		return tp / (fn + tp);
	}

	/**
	 * True positive rate
	 * @param matrix
	 * @return
	 */
	double truePositiveRate(ConfusionMatrix &matrix) {
		return truePositiveRate(matrix.falseNegative, matrix.truePositive);
	}

	/**
	 * Accuracy rate
	 * @param tp
	 * @param tn
	 * @param fp
	 * @param fn
	 * @return
	 */
	double accuracyRate(double tp, double tn, double fp, double fn) {
		return (tp + tn) / (tn + fp + fn + tp);
	}

	/**
	 * Accuracy rate
	 * @param matrix
	 * @return
	 */
	double accuracyRate(ConfusionMatrix &matrix) {
		return accuracyRate(matrix.truePositive, matrix.trueNegative, matrix.falsePositive, matrix.falseNegative);
	}

	/**
	 * Precision
	 * @param tp
	 * @param fp
	 * @return
	 */
	double precision(double tp, double fp) {
		return tp / (tp + fp);
	}

	/**
	 * Precision
	 * @param matrix
	 * @return
	 */
	double precision(ConfusionMatrix &matrix) {
		return precision(matrix.truePositive, matrix.falsePositive);
	}

	/**
	 * Recall
	 * @param tp
	 * @param fn
	 * @return
	 */
	double recall(double tp, double fn) {
		return tp / (tp + fn);
	}

	/**
	 * Recall
	 * @param matrix
	 * @return
	 */
	double recall(ConfusionMatrix &matrix) {
		return recall(matrix.truePositive, matrix.falseNegative);
	}

	/**
	 * Calculate the Equal Error Rate
	 * @param eer
	 * @param falsePositiveRates
	 * @param falseNegativeRates
	 */
	void calculateEER(double &eer, std::vector<double> &falsePositiveRates, std::vector<double> &falseNegativeRates) {
		double minorDistance = std::numeric_limits<double>::max();
		double tempDistance = -std::numeric_limits<double>::max();

		double pointAbove[2] = { 0, 0 };
		double pointBellow[2] = { 0, 0 };

		std::sort(falsePositiveRates.begin(), falsePositiveRates.end());
		std::sort(falseNegativeRates.rbegin(), falseNegativeRates.rend());

		for (unsigned int i = 0; i < falsePositiveRates.size(); i++) {

			// Calculate the distance between the coordinates and the x=y line in the graph
			tempDistance = std::abs((falsePositiveRates[i] - falseNegativeRates[i]) / std::sqrt(2));

			// Store the first nearest point ABOVE x=y line
			if (tempDistance <= minorDistance && falseNegativeRates[i] >= falsePositiveRates[i]) {
				minorDistance = tempDistance;
				pointAbove[0] = falsePositiveRates[i];
				pointAbove[1] = falseNegativeRates[i];
				continue;
			}

			// Second nearest point to x=y BELLOW line
			pointBellow[0] = falsePositiveRates[i];
			pointBellow[1] = falseNegativeRates[i];
			break;
		}

		// Calculating the line equation determined by
		// the points prev and next and its crossing
		// point with the line x=y witch is the EER
		double y = pointAbove[0] - pointBellow[0] - 1;
		double x = pointBellow[1] - pointAbove[1] + 1;
		double c = pointAbove[1] * pointBellow[0] - pointAbove[0] * pointBellow[1];

		eer = -(c / (x + y));
	}

	/**
	 * Calculate the Equal Error Rate
	 * @param confusionMatrices - A vector of @ConfusionMatrix
	 * @param eer
	 * @param falsePositiveRates
	 * @param falseNegativeRates
	 */
	void calculateEER(std::vector<ConfusionMatrix> &confusionMatrices, double &eer, std::vector<double> &falsePositiveRates, std::vector<double> &falseNegativeRates) {

		// Calculate all false positive and false negative rates
		for (auto confusionMatrix : confusionMatrices) {
			falsePositiveRates.push_back(falsePositiveRate(confusionMatrix));
			falseNegativeRates.push_back(falseNegativeRate(confusionMatrix));
		}

		calculateEER(eer, falsePositiveRates, falseNegativeRates);
	}
}
