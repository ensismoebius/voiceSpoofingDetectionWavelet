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

#include <iostream>
#include <cmath>
#include <vector>
#include "../vector/vectorUtils.h"
#include "../linearAlgebra/linearAlgebra.h"

class SupportVectorMachine {
	public:
		enum LABEL {
			POSITIVE = 1, NEGATIVE = -1
		};
	private:

		std::vector<std::vector<double>> trainningModels;
		std::vector<LABEL> trainningLabels;

		std::vector<double> hiddenToOutputWeights;
		std::vector<double> outputLayer;

		inline double radialDistance(std::vector<double> currentInputVector, std::vector<double> v2) {
			return std::exp(-euclidianDistance(currentInputVector, v2));
		}

	public:

		void addTrainningCases(std::vector<std::vector<double>> input, LABEL label) {
			this->trainningLabels.resize(this->trainningLabels.size() + input.size(), label);
			this->trainningModels.insert(this->trainningModels.end(), input.begin(), input.end());
		}

		LABEL evaluate(std::vector<double> input) {

			std::vector<double> distances(this->trainningModels.size());

			// creating the layer values
			for (unsigned int fi = 0; fi < this->trainningModels.size(); fi++) {
				distances[fi] = this->radialDistance(input, this->trainningModels[fi]) * this->hiddenToOutputWeights[fi];
			}

			double result = 0;

			for (double v : distances)
				result += v;

			// If the distance from NEGATIVE is greater
			// than the distance from POSITIVE return
			// POSITIVE otherwise return NEGATIVE
			if ((result - NEGATIVE) > (result - POSITIVE)) {
				return POSITIVE;
			}
			return NEGATIVE;
		}

		void train() {
			std::vector<std::vector<double>> intermediateLayer;
			intermediateLayer.resize(this->trainningModels.size(), std::vector<double>(this->trainningModels.size()));

			// creating the layer values
			for (unsigned int fi = 0; fi < this->trainningModels.size(); fi++) {
				for (unsigned int ii = 0; ii < this->trainningModels.size(); ii++) {
					intermediateLayer[fi][ii] = this->radialDistance(this->trainningModels[fi], this->trainningModels[ii]);
				}

				// Adding labels to the data
				intermediateLayer[fi].push_back(this->trainningLabels[fi]);
			}

			// Tests:
			//	intermediateLayer = { { 1, 1, 1, 7 }, { 2, 1, -1, 9 }, { 1, -2, 2, 2 } };
			//		results 4,2,1
			//	intermediateLayer = { { 4, 2, 1, -2, 3 }, { 3, -3, -1, -1, 2 }, { 3, 5, 1, 1, 0 }, { 1, -1, -1, 4, -2 } };
			//		results 0.461538, -0.384615, 1, -0.461538

			// Matrix scaling for linear system solving
			linearAlgebra::scaleMatrix(intermediateLayer);

			// system solved!!!
			this->hiddenToOutputWeights = linearAlgebra::solveMatrix(intermediateLayer);
		}
};

#endif /* SRC_LIB_CLASSIFIERS_SUPPORTVECTORMACHINE_CPP_ */
