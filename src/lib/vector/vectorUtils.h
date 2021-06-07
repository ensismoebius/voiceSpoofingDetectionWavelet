/*
 * vectorUtils.h
 *
 *  Created on: 4 de mai de 2020
 *      Author: ensismoebius
 */

#ifndef SRC_LIB_VECTOR_VECTORUTILS_H_
#define SRC_LIB_VECTOR_VECTORUTILS_H_

#include <vector>
#include <string>
/**
 * Given a string, splits it into an vector
 * according to some given delimiter
 * @example
 * std::vector<std::string> t = explode("Firefox jumps over the cliff", "e");
 * t = {"Fir","fox jumps ov","r th"," cliff"}
 * @param str - String to be exploded
 * @param delimiter
 * @return vector
 */
std::vector<std::string> explode(std::string str, std::string delimiter);

/**
 * Given point1 and point2 returns its euclidian distance
 * @param point1
 * @param point2
 * @return
 */
long double euclidianDistance(std::vector<long double> &point1, std::vector<long double> &point2);

/**
 * Given point1 and point2 returns its manhattan distance
 * @param point1
 * @param point2
 * @return
 */
long double manhattanDistance(std::vector<long double> &point1, std::vector<long double> &point2);

#endif /* SRC_LIB_VECTOR_VECTORUTILS_H_ */
