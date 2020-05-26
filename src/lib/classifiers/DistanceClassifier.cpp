/**
 * @author André Furlan
 * @email ensismoebius@gmail.com
 * This whole project are under GPLv3, for
 * more information read the license file
 *
 * 6 de mai de 2020
 */
#ifndef SRC_LIB_CLASSIFIERS_DISTANCECLASSIFIER_CPP_
#define SRC_LIB_CLASSIFIERS_DISTANCECLASSIFIER_CPP_

#include <map>
#include <vector>
#include <string>
#include "../vector/vectorUtils.h"

namespace classifiers {

	class DistanceClassifier {

		public:
			enum DISTANCE_TYPE {
				EUCLICIDIAN, MANHATTAN
			};

		private:
			std::map<std::string, std::vector<std::vector<double>>> referenceModels;
			DISTANCE_TYPE distanceType;

		public:

			void clearRefenceModels() {
				this->referenceModels.clear();
			}

			void addReferenceModels(std::string label, std::vector<std::vector<double>> models) {
				this->referenceModels[label] = models;
			}

			void setDistanceType(DISTANCE_TYPE type) {
				this->distanceType = type;
			}

			std::string classify(std::vector<double> featureVector) {

				double distance;

				std::string nearestLabel;
				double nearestDistance = std::numeric_limits<double>().max();

				if (this->distanceType == EUCLICIDIAN) {
					for (auto model : this->referenceModels) {
						for (auto modelFeatureVector : model.second) {

							distance = euclidianDistance(modelFeatureVector, featureVector);

							if (distance < nearestDistance) {
								nearestDistance = distance;
								nearestLabel = model.first;
							}
						}
					}

					return nearestLabel;
				}

				if (this->distanceType == MANHATTAN) {
					for (auto model : this->referenceModels) {
						for (auto modelFeatureVector : model.second) {

							distance = manhattanDistance(modelFeatureVector, featureVector);

							if (distance < nearestDistance) {
								nearestDistance = distance;
								nearestLabel = model.first;
							}
						}
					}

					return nearestLabel;
				}

				return "";
			}
	};
}
#endif /* SRC_LIB_CLASSIFIERS_DISTANCECLASSIFIER_CPP_ */
