/**
 * @author André Furlan
 * @email ensismoebius@gmail.com
 * This whole project are under GPLv3, for
 * more information read the license file
 *
 * 29 de mar de 2020
 *
 * @see http://wavelets.pybytes.com/
 *
 */

#ifndef SRC_LIB_WAVELET_TYPES_H_
#define SRC_LIB_WAVELET_TYPES_H_

#include <map>
#include <vector>
#include <string>
namespace wavelets
{

	void init(std::vector<std::string> chosenWavelets = { });
	void resetInitialization();
	std::vector<double> get(std::string waveletName);
	std::map<std::string, std::vector<double>> all();
}

#endif /* SRC_LIB_WAVELET_TYPES_H_ */
