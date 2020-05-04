#include <vector>
#include <string>

std::vector<std::string> explode(std::string str, std::string delimiter) {
	std::vector<std::string> res;

	size_t pos = 0;
	std::string token;
	while ((pos = str.find(delimiter)) != std::string::npos) {
		token = str.substr(0, pos);
		if (token.compare("") != 0) {
			res.push_back(token);
		}
		str.erase(0, pos + delimiter.length());
	}

	res.push_back(str);

	return res;
}
