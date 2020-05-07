#include <string>

#include "lib/file/fileUtils.h"
#include "experiments/01/Experiment01.cpp"
#include "experiments/02/Experiment02.cpp"

int parseArguments(int argc, char *args[], std::string &liveFileList, std::string &spoofingFileList) {

	if (argc != 5) {
		std::cout << "Usage: mestrado --live <path list of wave files> --spoofing <path list of wave files>" << std::endl;
		return 1;
	}

	for (int j = 1; j < argc; ++j) {
		std::string argument(args[j]);

		if (argument.compare("--live") == 0) {
			if (fileExistis(args[j + 1])) {
				liveFileList = args[j + 1];
				++j;
				continue;
			}
		}
		if (argument.compare("--spoofing") == 0) {
			if (fileExistis(args[j + 1])) {
				spoofingFileList = args[j + 1];
				++j;
				continue;
			}
		}
		std::cout << "Usage: mestrado --live <file> --spoofing <file>" << std::endl;
		return 2;
	}

	return 0;
}

int main(int argc, char *args[]) {

	std::string liveFileList;
	std::string spoofingFileList;

	int res = parseArguments(argc, args, liveFileList, spoofingFileList);

	if (res != 0) return res;

	//waveletExperiments::Experiment01::perform( { liveFileList, spoofingFileList });
	waveletExperiments::Experiment02::perform( { liveFileList, spoofingFileList });

	return 0;
}
