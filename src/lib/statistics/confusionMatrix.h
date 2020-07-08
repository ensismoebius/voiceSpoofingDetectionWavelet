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
		TP, FP, FN, TN
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
	double falsePositiveRate(std::map<CONFUSION_POS, int> matrix);

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
	double falseNegativeRate(std::map<CONFUSION_POS, int> matrix);

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
	double truePositiveRate(std::map<CONFUSION_POS, int> matrix);

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
	double accuracyRate(std::map<CONFUSION_POS, int> matrix);

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
	double precision(std::map<CONFUSION_POS, int> matrix);

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
	double recall(std::map<CONFUSION_POS, int> matrix);

}

#endif /* SRC_LIB_STATISTICS_CONFUSIONMATRIX_H_ */
