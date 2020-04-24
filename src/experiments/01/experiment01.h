/**
 * @author André Furlan
 * @email ensismoebius@gmail.com
 * This whole project are under GPLv3, for
 * more information read the license file
 *
 * 24 de abr de 2020
 *
 */

#ifndef SRC_EXPERIMENTS_EXPERIMENT01_CPP_
#define SRC_EXPERIMENTS_EXPERIMENT01_CPP_

#include <string>
#include <vector>

namespace experiment01 {

	/**
	 * Perform the experiment
	 * @param args
	 */
	void perform(char *args[]);

	/**
	 * Plot the results
	 * @param data
	 */
	void plotResults(std::vector<double> data);

	/**
	 * Analityc function which performs an wavelet transform
	 * of the value and calculate the energies based on MEL
	 * or BARK intervals
	 * @param signal
	 * @param signalLength
	 * @param samplingRate
	 * @param path
	 */
	void waveletAnaliticFunction(std::vector<double> &signal, int &signalLength, unsigned int samplingRate, std::string path);
}
#endif /* SRC_EXPERIMENTS_EXPERIMENT01_CPP_ */
