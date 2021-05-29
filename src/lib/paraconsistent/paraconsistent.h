/*
 * paraconsistent.h
 *
 *  Created on: 30 de abr de 2019
 *      Author: ensis
 */

#ifndef LIB_PARACONSISTENT_H_
#define LIB_PARACONSISTENT_H_

#include <map>
#include <limits>
#include <vector>

double calcCertaintyDegree_G1(double alpha, double betha);
double calcContradictionDegree_G2(double alpha, double betha);

double calculateAlpha(unsigned int amountOfClasses, unsigned int featureVectorsPerClass, unsigned int featureVectorSize, std::map<std::string, std::vector<std::vector<double>>> &arrClasses);
double calculateBeta(unsigned int amountOfClasses, unsigned int featureVectorsPerClass, unsigned int featureVectorSize, std::map<std::string, std::vector<std::vector<double>>> &arrClasses);
void normalizeClassesFeatureVectors(unsigned int amountOfClasses, unsigned int featureVectorsPerClass, unsigned int featureVectorSize, std::map<std::string, std::vector<std::vector<double>>> &arrClasses);

#endif /* LIB_PARACONSISTENT_H_ */
