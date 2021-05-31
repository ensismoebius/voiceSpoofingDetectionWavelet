/**
 * @author André Furlan
 * @email ensismoebius@gmail.com
 * This whole project are under GPLv3, for
 * more information read the license file
 *
 * 6 de mai de 2020
 */
#ifndef SRC_LIB_CLASSIFIERS_DISTANCECLASSIFIER_H_
#define SRC_LIB_CLASSIFIERS_DISTANCECLASSIFIER_H_

#include <map>
#include <vector>
#include <string>
#include <limits>
#include "../vector/vectorUtils.h"

namespace classifiers
{
	class DistanceClassifier
	{
		public:
			enum DISTANCE_TYPE
			{
				EUCLICIDIAN, MANHATTAN
			};

		private:
			std::map<std::string, std::vector<std::vector<double>>> referenceModels;
			DISTANCE_TYPE distanceType;

		public:
			void clearRefenceModels();
			void addReferenceModels(std::string label, std::vector<std::vector<double>> models);
			void setDistanceType(DISTANCE_TYPE type);
			std::string classify(std::vector<double> featureVector);
	};
}
#endif /* SRC_LIB_CLASSIFIERS_DISTANCECLASSIFIER_H_ */
