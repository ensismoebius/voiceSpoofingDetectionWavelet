/**
 * @author André Furlan
 * @email ensismoebius@gmail.com
 * This whole project are under GPLv3, for
 * more information read the license file
 *
 * 8 de jul. de 2020
 *
 */

#ifndef SRC_LIB_STATISTICS_CONFUSIONMATRIX_H_
#define SRC_LIB_STATISTICS_CONFUSIONMATRIX_H_

#include <map>
#include <string>
#include <vector>
#include <algorithm>

namespace statistics {

	enum CONFUSION_POS {
		TruePositive, FalsePositive, FalseNegative, TrueNegative
	};

	struct ConfusionMatrix {
			int truePositive;
			int falsePositive;
			int falseNegative;
			int trueNegative;
	};

	/**
	 * False positive rate
	 * @param fp
	 * @param tn
	 * @return
	 */
	double falsePositiveRate(double fp, double tn);

	/**
	 * False positive rate
	 * @param matrix
	 * @return
	 */
	double falsePositiveRate(ConfusionMatrix matrix);

	/**
	 * False negative rate
	 * @param fn
	 * @param tn
	 * @return
	 */
	double falseNegativeRate(double tp, double fn);

	/**
	 * False negative rate
	 * @param matrix
	 * @return
	 */
	double falseNegativeRate(ConfusionMatrix matrix);

	/**
	 * True positive rate
	 * @param tp
	 * @param fn
	 * @return
	 */
	double truePositiveRate(double tp, double fn);

	/**
	 * True positive rate
	 * @param matrix
	 * @return
	 */
	double truePositiveRate(ConfusionMatrix matrix);

	/**
	 * Accuracy rate
	 * @param tp
	 * @param tn
	 * @param fp
	 * @param fn
	 * @return
	 */
	double accuracyRate(double tp, double tn, double fp, double fn);

	/**
	 * Accuracy rate
	 * @param matrix
	 * @return
	 */
	double accuracyRate(ConfusionMatrix matrix);

	/**
	 * Precision
	 * @param tp
	 * @param fp
	 * @return
	 */
	double precision(double tp, double fp);

	/**
	 * Precision
	 * @param matrix
	 * @return
	 */
	double precision(ConfusionMatrix matrix);

	/**
	 * Recall
	 * @param tp
	 * @param fn
	 * @return
	 */
	double recall(double tp, double fn);

	/**
	 * Recall
	 * @param matrix
	 * @return
	 */
	double recall(ConfusionMatrix matrix);

}

#endif /* SRC_LIB_STATISTICS_CONFUSIONMATRIX_H_ */
