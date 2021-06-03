/**
 * @author André Furlan
 * @email ensismoebius@gmail.com
 * This whole project are under GPLv3, for
 * more information read the license file
 *
 * 12 de mai de 2020
 */

#include <cmath>
#include <vector>
#include "SupportVectorMachine.h"
#include "../vector/vectorUtils.h"
#include "../linearAlgebra/linearAlgebra.h"

namespace classifiers
{

	/**
	 * Used to calculate the radial distances between two vectors
	 * @param currentInputVector
	 * @param v2
	 * @return distance
	 */
	inline double SupportVectorMachine::radialDistance(std::vector<double> currentInputVector, std::vector<double> v2)
	{
		return std::exp(-euclidianDistance(currentInputVector, v2));
	}

	/**
	 * Add training matrices to SVM
	 * @param matrix - A matrix (2d vector) with training cases
	 * @param label - Label indicating if it is positive or negative sample
	 */
	void SupportVectorMachine::addTrainingCases(std::vector<std::vector<double>> matrix, SupportVectorMachine::LABEL label)
	{
		this->trainingLabels.resize(this->trainingLabels.size() + matrix.size(), label);
		this->trainingModels.insert(this->trainingModels.end(), matrix.begin(), matrix.end());
	}

	/**
	 * Do the classification!!!
	 * @param input - The vector to be tested
	 * @return POSITIVE or NEGATIVE
	 */
	SupportVectorMachine::LABEL SupportVectorMachine::evaluate(std::vector<double> input)
	{

		// Reset Weighted distance for next classification
		wDistance = 0;

		// creating the layer values
		for (unsigned int fi = 0; fi < this->trainingModels.size(); fi++)
		{
			wDistance += this->outputWeights[fi] * this->radialDistance(input, this->trainingModels[fi]);
		}

		return (wDistance / this->trainingModels.size() < 0) ? NEGATIVE : POSITIVE;
	}

	/**
	 * Training method, use this after use
	 * @see addTrainingCases method
	 * @see addTrainingCases
	 */
	void SupportVectorMachine::train()
	{
		// Used to determine the weights for the connections to the output
		std::vector<std::vector<double>> distancesMatrix;
		distancesMatrix.resize(this->trainingModels.size(), std::vector<double>(this->trainingModels.size()));

		// creating the distances matrix values
		// i.e. the distances between the vectors
		for (unsigned int fi = 0; fi < this->trainingModels.size(); fi++)
		{
			for (unsigned int ii = 0; ii < this->trainingModels.size(); ii++)
			{
				distancesMatrix[fi][ii] = this->radialDistance(this->trainingModels[fi], this->trainingModels[ii]);
			}

			// Adding labels to the data
			distancesMatrix[fi].push_back(this->trainingLabels[fi]);
		}

		// Tests:
		//	distancesMatrix = { { 1, 1, 1, 7 }, { 2, 1, -1, 9 }, { 1, -2, 2, 2 } };
		//		results 4,2,1
		//	distancesMatrix = { { 4, 2, 1, -2, 3 }, { 3, -3, -1, -1, 2 }, { 3, 5, 1, 1, 0 }, { 1, -1, -1, 4, -2 } };
		//		results 0.461538, -0.384615, 1, -0.461538

		// Matrix scaling for linear system solving
		linearAlgebra::scaleMatrix(distancesMatrix);

		// system solved!!! Return the coeficcients
		this->outputWeights = linearAlgebra::solveMatrix(distancesMatrix);
	}

	void SupportVectorMachine::clearTrain()
	{
		this->outputWeights.clear();
		this->trainingLabels.clear();
		this->trainingModels.clear();
	}
}
