/**
 * @author André Furlan
 * @email ensismoebius@gmail.com
 * This whole project are under GPLv3, for
 * more information read the license file
 *
 * 12 de mai de 2020
 *
 */

#ifndef SRC_LIB_CLASSIFIERS_SUPPORTVECTORMACHINE_CPP_
#define SRC_LIB_CLASSIFIERS_SUPPORTVECTORMACHINE_CPP_

#include <cmath>
#include <vector>
#include "../vector/vectorUtils.h"

class SupportVectorMachine {
	public:
		enum LABEL {
			POSITIVE = 1, NEGATIVE = -1
		};
	private:

		std::vector<std::vector<double>> inputLayer;
		std::vector<std::vector<double>> hiddenLayer;

		std::vector<double> outputLayer;
		std::vector<LABEL> labels;

		inline double radialDistance(std::vector<double> currentInputVector, std::vector<double> v2) {
			return std::exp(-euclidianDistance(currentInputVector, v2));
		}

	public:

		void addTrainningCases(std::vector<std::vector<double>> input, LABEL label) {
			this->labels.resize(this->labels.size() + input.size(), label);
			this->inputLayer.insert(this->inputLayer.end(), input.begin(), input.end());
			this->hiddenLayer.resize(this->inputLayer.size(), std::vector<double>(input[0].size()));
		}

		void train() {
			for (unsigned int fi = 0; fi < this->inputLayer.size(); fi++) {
				for (unsigned int ii = 0; ii < this->inputLayer.size(); ii++) {
					this->hiddenLayer[fi][ii] = this->radialDistance(this->inputLayer[fi], this->inputLayer[ii]);
				}
			}
		}
};

#endif /* SRC_LIB_CLASSIFIERS_SUPPORTVECTORMACHINE_CPP_ */
