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

/**
 * @brief Given the alfa and the betha calculates the certainty degree
 * 
 * @param alpha 
 * @param betha 
 * @return long double 
 */
long double calcCertaintyDegree_G1(long double alpha, long double betha);

/**
 * @brief Given the alfa and the betha calculates the contradiction degree
 * 
 * @param alpha 
 * @param betha 
 * @return long double 
 */
long double calcContradictionDegree_G2(long double alpha, long double betha);

/**
 * @brief Calculates the alpha value
 * 
 * @param amountOfClasses Amount of classes
 * @param featureVectorsPerClass Amount of feature vectors per class
 * @param featureVectorSize Size of the feature vectors
 * @param arrClasses Holds all classes
 * @return long double Value of alpha
 */
long double calculateAlpha(unsigned int amountOfClasses, unsigned int featureVectorsPerClass, unsigned int featureVectorSize, std::map<std::string, std::vector<std::vector<long double>>> &arrClasses);

/**
 * @brief Calculates the beta value
 * 
 * @param amountOfClasses Amount of classes
 * @param featureVectorsPerClass Amount of feature vectors per class
 * @param featureVectorSize Size of the feature vectors
 * @param arrClasses Holds all classes
 * @return long double Value of beta
 */
long double calculateBeta(unsigned int amountOfClasses, unsigned int featureVectorsPerClass, unsigned int featureVectorSize, std::map<std::string, std::vector<std::vector<long double>>> &arrClasses);

/**
 * @brief Normalize all feature vectors from all classes
 * 
 * @param amountOfClasses Amount of classes
 * @param featureVectorsPerClass Amount of feature vectors per class
 * @param featureVectorSize Size of the feature vectors
 * @param arrClasses Holds all classes
 */
void normalizeClassesFeatureVectors(unsigned int amountOfClasses, unsigned int featureVectorsPerClass, unsigned int featureVectorSize, std::map<std::string, std::vector<std::vector<long double>>> &arrClasses);

#endif /* LIB_PARACONSISTENT_H_ */
