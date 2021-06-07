/**
 * @author André Furlan
 * @email ensismoebius@gmail.com
 * This whole project are under GPLv3, for
 * more information read the license file
 *
 * 6 de mai de 2020
 */

#include <map>
#include <vector>
#include <string>
#include <limits>
#include "DistanceClassifier.h"
#include "../vector/vectorUtils.h"

namespace classifiers
{
	void DistanceClassifier::clearRefenceModels()
	{
		this->referenceModels.clear();
	}

	void DistanceClassifier::addReferenceModels(std::string label, std::vector<std::vector<long double>> models)
	{
		this->referenceModels[label] = models;
	}

	void DistanceClassifier::setDistanceType(DISTANCE_TYPE type)
	{
		this->distanceType = type;
	}

	std::string DistanceClassifier::classify(std::vector<long double> featureVector)
	{

		long double distance;

		std::string nearestLabel;
		long double nearestDistance = std::numeric_limits<double>().max(); // @suppress("Ambiguous problem")

		if (this->distanceType == EUCLICIDIAN)
		{
			for (auto model : this->referenceModels)
			{
				for (auto modelFeatureVector : model.second)
				{

					distance = euclidianDistance(modelFeatureVector, featureVector);

					if (distance < nearestDistance)
					{
						nearestDistance = distance;
						nearestLabel = model.first;
					}
				}
			}

			return nearestLabel;
		}

		if (this->distanceType == MANHATTAN)
		{
			for (auto model : this->referenceModels)
			{
				for (auto modelFeatureVector : model.second)
				{

					distance = manhattanDistance(modelFeatureVector, featureVector);

					if (distance < nearestDistance)
					{
						nearestDistance = distance;
						nearestLabel = model.first;
					}
				}
			}

			return nearestLabel;
		}

		return "";
	}
}
