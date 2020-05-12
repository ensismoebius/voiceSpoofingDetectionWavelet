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

#include <vector>

class SupportVectorMachine {

	private:
		std::vector<double> inputLayer;
		std::vector<double> hiddenLayer;
		std::vector<double> outputLayer;

	public:
		SupportVectorMachine(unsigned int inputSize, unsigned int hiddenSize, unsigned int outputSize) {
			this->inputLayer.resize(inputSize);
			this->hiddenLayer.resize(hiddenSize);
			this->outputLayer.resize(outputSize);
		}
};

#endif /* SRC_LIB_CLASSIFIERS_SUPPORTVECTORMACHINE_CPP_ */
