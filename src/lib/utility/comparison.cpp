/**
 * @author André Furlan
 * @email ensismoebius@gmail.com
 * This whole project are under GPLv3, for
 * more information read the license file
 *
 * 01 de jun de 2021
 *
 * Contains comparison routines
 */
#include <stdexcept>


/**
 * Checks if an value is between another two
 * @param val
 * @param lowerLimit
 * @param upperLimit
 * @return
 */
bool inRange(double val, double lowerLimit, double upperLimit)
{
	if (lowerLimit > upperLimit) throw std::invalid_argument("lower lowerLimit be lower than upperLimit");
	return val >= lowerLimit && val <= upperLimit;
}

