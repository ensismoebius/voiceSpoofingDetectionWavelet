/**
 * @author André Furlan
 * @email ensismoebius@gmail.com
 * This whole project are under GPLv3, for
 * more information read the license file
 *
 * 29 de mar de 2020
 *
 */

#include "linearAlgebra.h"

long double dotProduct(std::vector<double> a, std::vector<double> b) {

	long double product = 0;

	// Loop for calculate cot product
	for (unsigned int i = 0; i < a.size(); i++) {
		product = product + a.at(i) * b.at(i);
	}
	return product;
}
