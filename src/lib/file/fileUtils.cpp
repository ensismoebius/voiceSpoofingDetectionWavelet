/**
 * @author André Furlan
 * @email ensismoebius@gmail.com
 * This whole project are under GPLv3, for
 * more information read the license file
 *
 * 7 de mai de 2020
 */
#include <string>
#include <sys/stat.h>

bool fileExistis(const std::string &name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

