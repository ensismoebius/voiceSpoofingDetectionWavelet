/**
 * @author André Furlan
 * @email ensismoebius@gmail.com
 * This whole project are under GPLv3, for
 * more information read the license file
 *
 * 8 de jul. de 2020
 *
 */

#include <string>
#include <vector>

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
	double falsePositiveRate(std::map<CONFUSION_POS, int> matrix) {
		return falsePositiveRate(matrix[FP], matrix[TN]);
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
	double falseNegativeRate(std::map<CONFUSION_POS, int> matrix) {
		return falseNegativeRate(matrix[TP], matrix[FN]);
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
	double truePositiveRate(std::map<CONFUSION_POS, int> matrix) {
		return truePositiveRate(matrix[FN], matrix[TP]);
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
	double accuracyRate(std::map<CONFUSION_POS, int> matrix) {
		return accuracyRate(matrix[TP], matrix[TN], matrix[FP], matrix[FN]);
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
	double precision(std::map<CONFUSION_POS, int> matrix) {
		return precision(matrix[TP], matrix[FP]);
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
	double recall(std::map<CONFUSION_POS, int> matrix) {
		return recall(matrix[TP], matrix[FN]);
	}
}
