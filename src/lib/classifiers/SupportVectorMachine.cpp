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
#include "../linearAlgebra/linearAlgebra.h"

namespace classifiers {

	/**
	 * A support vector machine implementation
	 */
	class SupportVectorMachine {
		public:

			/**
			 * Labels for data
			 */
			enum LABEL {
				POSITIVE = 1, NEGATIVE = -1
			};
		private:

			/**
			 * Labels to the trainning models
			 */
			std::vector<LABEL> trainningLabels;

			/**
			 * This is the reference for trainning 
			 * and classification of further data
			 */
			std::vector<std::vector<double>> trainningModels;

			/**
			 * Holds the weights used for classification
			 */
			std::vector<double> outputWeights;

			/**
			 * Used to calculate the radial distances between two vectors
			 * @param currentInputVector
			 * @param v2
			 * @return distance
			 */
			inline double radialDistance(std::vector<double> currentInputVector, std::vector<double> v2) {
				return std::exp(-euclidianDistance(currentInputVector, v2));
			}

		public:

			/**
			 * Add trainning matrices to SVM
			 * @param matrix - A matrix (2d vector) with trainning cases 
			 * @param label - Label indicating if it is positive or negative sample
			 */
			void addTrainningCases(std::vector<std::vector<double>> matrix, LABEL label) {
				this->trainningLabels.resize(this->trainningLabels.size() + matrix.size(), label);
				this->trainningModels.insert(this->trainningModels.end(), matrix.begin(), matrix.end());
			}

			/**
			 * Do the classification!!!
			 * @param input - The vector to be tested
			 * @return POSITIVE or NEGATIVE
			 */
			LABEL evaluate(std::vector<double> input) {

				// Weighted distance from input and trainning models
				double wDistance = 0;

				// creating the layer values
				for (unsigned int fi = 0; fi < this->trainningModels.size(); fi++) {
					wDistance += this->outputWeights[fi] * this->radialDistance(input, this->trainningModels[fi]);
				}

				// If the distance from NEGATIVE is greater
				// than the distance from POSITIVE return
				// POSITIVE otherwise return NEGATIVE
				if (std::pow(wDistance - NEGATIVE, 2) > std::pow(wDistance - POSITIVE, 2)) {
					return POSITIVE;
				}
				return NEGATIVE;
			}

			/**
			 * Trainning method, use this after use
			 * @addTrainningCases method
			 * @see addTrainningCases
			 */
			void train() {
				// Used to determine the weights for the connections to the output
				std::vector<std::vector<double>> distancesMatrix;
				distancesMatrix.resize(this->trainningModels.size(), std::vector<double>(this->trainningModels.size()));

				// creating the distances matrix values
				// i.e. the distances between the vectors
				for (unsigned int fi = 0; fi < this->trainningModels.size(); fi++) {
					for (unsigned int ii = 0; ii < this->trainningModels.size(); ii++) {
						distancesMatrix[fi][ii] = this->radialDistance(this->trainningModels[fi], this->trainningModels[ii]);
					}

					// Adding labels to the data
					distancesMatrix[fi].push_back(this->trainningLabels[fi]);
				}

				// Tests:
				//	intermediateLayer = { { 1, 1, 1, 7 }, { 2, 1, -1, 9 }, { 1, -2, 2, 2 } };
				//		results 4,2,1
				//	intermediateLayer = { { 4, 2, 1, -2, 3 }, { 3, -3, -1, -1, 2 }, { 3, 5, 1, 1, 0 }, { 1, -1, -1, 4, -2 } };
				//		results 0.461538, -0.384615, 1, -0.461538

				// Matrix scaling for linear system solving
				linearAlgebra::scaleMatrix(distancesMatrix);

				// system solved!!! Return the coeficcients
				this->outputWeights = linearAlgebra::solveMatrix(distancesMatrix);
			}
	};
}
#endif /* SRC_LIB_CLASSIFIERS_SUPPORTVECTORMACHINE_CPP_ */
