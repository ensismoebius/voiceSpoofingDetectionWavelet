#include <string>

#include "experiments/01/Experiment01.cpp"
#include "experiments/02/Experiment02.cpp"

bool fileExistis(const std::string &name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

int main(int argc, char *args[]) {

	if (argc != 5) {
		std::cout << "Usage: mestrado --live <file> --spoofing <file>" << std::endl;
		return 1;
	}

	std::string liveFileList;
	std::string spoofingFileList;

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

	//waveletExperiments::Experiment01::perform( { liveFileList, spoofingFileList }, 2);
	waveletExperiments::Experiment02::perform( { liveFileList, spoofingFileList }, 2);

	return 0;
}
