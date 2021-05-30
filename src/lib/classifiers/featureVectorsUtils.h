/**
 * @author André Furlan
 * @email ensismoebius@gmail.com
 * This whole project are under GPLv3, for
 * more information read the license file
 *
 * 7 de mai de 2020
 *
 */
#ifndef SRC_LIB_CLASSIFIERS_FEATUREVECTORSUTILS_H_
#define SRC_LIB_CLASSIFIERS_FEATUREVECTORSUTILS_H_

#include <vector>

namespace classifiers
{
	/**
	 * Randomly chooses a certain amount of samples
	 * from @source and puts them in @output
	 * @param source - A list of features vectors
	 * @param model - A scrambled list of the features vectors for model
	 * @param teste - A scrambled list of the features vectors for tests
	 * @param modelSize - The percentage of features vectors we want, model size must be between 0 and 1 (Zero not included)
	 */
	void raflleFeaturesVectors(std::vector<std::vector<double>> &source, std::vector<std::vector<double>> &model, std::vector<std::vector<double>> &test, float modelSize);
}

#endif /* SRC_LIB_CLASSIFIERS_FEATUREVECTORSUTILS_H_ */
