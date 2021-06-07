/**
 * @author André Furlan
 * @email ensismoebius@gmail.com
 * This whole project are under GPLv3, for
 * more information read the license file
 *
 * 12 de mai de 2020
 *
 */

#ifndef SRC_LIB_CLASSIFIERS_SUPPORTVECTORMACHINE_H_
#define SRC_LIB_CLASSIFIERS_SUPPORTVECTORMACHINE_H_
#include <cmath>
#include <vector>
#include "../vector/vectorUtils.h"
#include "../linearAlgebra/linearAlgebra.h"

namespace classifiers
{
	/**
	 * A support vector machine implementation
	 */
	class SupportVectorMachine
	{
		public:

			/**
			 * Labels for data
			 */
			enum LABEL
			{
				POSITIVE = 1, NEGATIVE = -1
			};

		private:

			/**
			 * Labels to the training models
			 */
			std::vector<LABEL> trainingLabels;

			/**
			 * This is the reference for training
			 * and classification of further data
			 */
			std::vector<std::vector<long double>> trainingModels;

			/**
			 * Holds the weights used for classification
			 */
			std::vector<long double> outputWeights;

			/**
			 * Weighted distance from input and training models
			 * Used on @see evaluate method
			 */
			double wDistance = 0;

			/**
			 * Used to calculate the radial distances between two vectors
			 * @param currentInputVector
			 * @param v2
			 * @return distance
			 */
			inline double radialDistance(std::vector<long double> currentInputVector, std::vector<long double> v2);

		public:

			/**
			 * Add training matrices to SVM
			 * @param matrix - A matrix (2d vector) with training cases
			 * @param label - Label indicating if it is positive or negative sample
			 */
			void addTrainingCases(std::vector<std::vector<long double>> matrix, LABEL label);

			/**
			 * Do the classification!!!
			 * @param input - The vector to be tested
			 * @return POSITIVE or NEGATIVE
			 */
			LABEL evaluate(std::vector<long double> input);

			/**
			 * Training method, use this after use
			 * @see addTrainingCases method
			 * @see addTrainingCases
			 */
			void train();

			void clearTrain();
	};
}
#endif /* SRC_LIB_CLASSIFIERS_SUPPORTVECTORMACHINE_H_ */
