/**
 * @author André Furlan
 * @email ensismoebius@gmail.com
 * This whole project are under GPLv3, for
 * more information read the license file
 *
 * 3 de jul de 2020
 *
 */
#ifndef SRC_LIB_STATISTICS_STATISTICS_H_
#define SRC_LIB_STATISTICS_STATISTICS_H_

#include <vector>

namespace statistics {
	double variance(std::vector<double> data);
	double variance(double *data, unsigned int length);

	double standardDeviation(std::vector<double> data);
	double standardDeviation(double *data, unsigned int length);
}
#endif /* SRC_LIB_STATISTICS_STATISTICS_H_ */
