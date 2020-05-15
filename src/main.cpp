#include <string>

#include "lib/file/fileUtils.h"
#include "experiments/01/Experiment01.cpp"
#include "experiments/02/Experiment02.cpp"
#include "experiments/03/Experiment03.cpp"

int parseArguments(int argc, char *args[], std::string &liveFileList, std::string &spoofingFileList, std::string &resultsDestiny, unsigned int &numberOfTests, double &minModel, double &maxModel) {

	if (argc != 13) {
		std::cout << "Usage: mestrado --live <path list of wave files> --spoofing <path list of wave files> --out <results directory path> --tests <number of tests> --minModel <min size of model> --maxModel <max size of model>" << std::endl;
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
		if (argument.compare("--out") == 0) {
			if (fileExistis(args[j + 1])) {
				resultsDestiny = args[j + 1];
				++j;
				continue;
			}
		}
		if (argument.compare("--tests") == 0) {
			numberOfTests = std::stoi(args[j + 1]);
			++j;
			continue;
		}
		if (argument.compare("--minModel") == 0) {
			minModel = std::stod(args[j + 1]);
			++j;
			continue;
		}
		if (argument.compare("--maxModel") == 0) {
			maxModel = std::stod(args[j + 1]);
			++j;
			continue;
		}
		std::cout << "Usage: mestrado --live <path list of wave files> --spoofing <path list of wave files> --out <results directory path> --tests <number of tests> --minModel <min size of model> --maxModel <max size of model>" << std::endl;
		return 2;
	}

	return 0;
}

int main(int argc, char *args[]) {

	std::string liveFileList;
	std::string spoofingFileList;
	std::string resultsDestiny;
	unsigned int numberOfTests;
	double minModel;
	double maxModel;

	int res = parseArguments(argc, args, liveFileList, spoofingFileList, resultsDestiny, numberOfTests, minModel, maxModel);

	if (res != 0) return res;

	//waveletExperiments::Experiment01::perform( { liveFileList, spoofingFileList }, resultsDestiny, numberOfTests, minModel, maxModel);
	//waveletExperiments::Experiment02::perform( { liveFileList, spoofingFileList }, resultsDestiny, numberOfTests, minModel, maxModel);
	waveletExperiments::Experiment03::perform( { liveFileList, spoofingFileList }, resultsDestiny, numberOfTests, minModel, maxModel);

	return 0;
}
