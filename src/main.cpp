#include <string>

#include "lib/file/fileUtils.h"
#include "experiments/01/Experiment01.cpp"
#include "experiments/02/Experiment02.cpp"
#include "experiments/03/Experiment03.cpp"

/**
 * Parses the command line arguments
 * @param argc
 * @param args
 * @param liveFileList
 * @param spoofingFileList
 * @param resultsDestiny
 * @param numberOfTests
 * @param minModel
 * @param maxModel
 * @param experiment
 * @return the experiment choosen
 */
int parseArguments(int argc, char *args[], std::string &liveFileList, std::string &spoofingFileList, std::string &resultsDestiny, unsigned int &numberOfTests, double &minModel, double &maxModel, int experiment) {

	for (int j = 1; j < argc; ++j) {
		std::string argument(args[j]);

		if (argument.compare("--experiment") == 0) {
			experiment = std::stoi(args[j + 1]);
			++j;
			continue;
		}

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
	}

	if (experiment == -1) {
		std::cout << "Usage: mestrado --experiment <experiment number> --live <path list of wave files> --spoofing <path list of wave files> --out <results directory path> --tests <number of tests> --minModel <min size of model> --maxModel <max size of model>\n" << std::endl;
		std::cout << "You must choose an experiment:\n";
		std::cout << "\t1-Wavelet feature vectors over BARK or MEL comparation\n";
		std::cout << "\t2-Wavelet feature vector over BARK using distance classifiers\n";
		std::cout << "\t3-Wavelet feature vector over BARK using SVM classifier\n";
		std::cout << "\t0-All experiments" << std::endl << std::endl;
		return -1;
	}

	if (liveFileList.empty() || spoofingFileList.empty()) {
		std::cout << "Please inform the lists of spoofing wav files and the list of live wav files" << std::endl;
		std::cout << "Usage: mestrado --experiment " << experiment << " --live <path list of wave files> --spoofing <path list of wave files> --out <results directory path> --tests <number of tests> --minModel <min size of model> --maxModel <max size of model>" << std::endl << std::endl;
		return -1;
	}

	if (experiment == 1) {
		if (argc != 9) {
			std::cout << "Usage: mestrado --experiment " << experiment << " --live <path list of wave files> --spoofing <path list of wave files> --out <results directory path>" << std::endl << std::endl;
			return -1;
		}

		bool ok = true;
		ok &= !liveFileList.empty();
		ok &= !resultsDestiny.empty();
		ok &= !spoofingFileList.empty();

		if (ok) return experiment;

		std::cout << "Usage: mestrado --experiment " << experiment << " --live <path list of wave files> --spoofing <path list of wave files> --out <results directory path>" << std::endl << std::endl;
		return -1;

	}

	if (experiment == 2 || experiment == 3 || experiment == 0) {
		if (argc != 15) {
			std::cout << "Usage: mestrado --experiment " << experiment << " --live <path list of wave files> --spoofing <path list of wave files> --out <results directory path> --tests <number of tests> --minModel <min size of model> --maxModel <max size of model>" << std::endl << std::endl;
			return -1;
		}

		bool ok = true;
		ok &= !liveFileList.empty();
		ok &= !resultsDestiny.empty();
		ok &= !spoofingFileList.empty();
		ok &= numberOfTests > 0;
		ok &= minModel >= .1;
		ok &= maxModel >= minModel;

		if (ok) return experiment;

		std::cout << "Usage: mestrado --experiment " << experiment << " --live <path list of wave files> --spoofing <path list of wave files> --out <results directory path> --tests <number of tests> --minModel <min size of model> --maxModel <max size of model>" << std::endl << std::endl;
		return -1;
	}

	std::cout << "Usage: mestrado --experiment <experiment number> --live <path list of wave files> --spoofing <path list of wave files> --out <results directory path> --tests <number of tests> --minModel <min size of model> --maxModel <max size of model>" << std::endl << std::endl;
	return -1;
}

int main(int argc, char *args[]) {

	int experiment = -1;

	double minModel;
	double maxModel;
	std::string liveFileList;
	std::string resultsDestiny;
	unsigned int numberOfTests;
	std::string spoofingFileList;

	int res = parseArguments(argc, args, liveFileList, spoofingFileList, resultsDestiny, numberOfTests, minModel, maxModel, experiment);

	switch (res) {
		case 1:
			waveletExperiments::Experiment01::perform( { liveFileList, spoofingFileList }, resultsDestiny);
			return 0;
			break;
		case 2:
			waveletExperiments::Experiment02::perform( { liveFileList, spoofingFileList }, resultsDestiny, numberOfTests, minModel, maxModel);
			return 0;
			break;
		case 3:
			waveletExperiments::Experiment03::perform( { liveFileList, spoofingFileList }, resultsDestiny, numberOfTests, minModel, maxModel);
			return 0;
			break;
		case 0:
			waveletExperiments::Experiment01::perform( { liveFileList, spoofingFileList }, resultsDestiny);
			waveletExperiments::Experiment02::perform( { liveFileList, spoofingFileList }, resultsDestiny, numberOfTests, minModel, maxModel);
			waveletExperiments::Experiment03::perform( { liveFileList, spoofingFileList }, resultsDestiny, numberOfTests, minModel, maxModel);
			return 0;
			break;
	}

	// Bad arguments nothing done!
	return -1;
}
