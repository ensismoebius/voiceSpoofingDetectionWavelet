#include <cmath>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <stdexcept>

std::vector<std::string> explode(std::string str, std::string delimiter)
{
	std::vector<std::string> res;

	size_t pos = 0;
	std::string token;
	while ((pos = str.find(delimiter)) != std::string::npos)
	{
		token = str.substr(0, pos);
		if (token.compare("") != 0)
		{
			res.push_back(token);
		}
		str.erase(0, pos + delimiter.length());
	}

	res.push_back(str);

	return res;
}

double euclidianDistance(std::vector<double> &point1, std::vector<double> &point2)
{
	if (point1.size() != point2.size())
	{
		throw std::runtime_error("Vectors must have the same size");
	}

	double temp = 0;

	for (unsigned int i = 0; i < point1.size(); ++i)
	{
		temp += std::pow(point1[i] - point2[i], 2);
	}

	return std::sqrt(temp);

}

double manhattanDistance(std::vector<double> &point1, std::vector<double> &point2)
{
	if (point1.size() != point2.size())
	{
		throw std::runtime_error("Vectors must have the same size");
	}

	double temp = 0;

	for (unsigned int i = 0; i < point1.size(); ++i)
	{
		temp += std::abs(point1[i] - point2[i]);
	}

	return std::sqrt(temp);

}
