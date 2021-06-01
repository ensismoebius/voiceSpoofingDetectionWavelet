#include <string>

#include "lib/file/fileUtils.h"
#include "experiments/01/Experiment01.cpp"
#include "experiments/02/Experiment02.cpp"
#include "experiments/03/Experiment03.cpp"
#include "experiments/04/Experiment04.cpp"
#include "experiments/05/Experiment05.cpp"
#include "experiments/06/Experiment06.cpp"
#include "experiments/07/Experiment07.cpp"
#include "experiments/08/Experiment08.cpp"

int parseArguments(int argc, char *args[], std::string &liveFileList, std::string &spoofingFileList, std::string &resultsDestiny, unsigned int &numberOfTests, double &minModel, double &maxModel, int experiment);

int main(int argc, char *args[])
{
	int experiment = -1;

	double minModel;
	double maxModel;
	std::string liveFileList;
	std::string resultsDestiny;
	unsigned int numberOfTests;
	std::string spoofingFileList;

	// Ensuring that the random numbers are going to honor its names
	std::srand(std::time(nullptr));

	int res = parseArguments(argc, args, liveFileList, spoofingFileList, resultsDestiny, numberOfTests, minModel, maxModel, experiment);

	switch (res)
	{
		case 1:
			waveletExperiments::Experiment01::perform( { liveFileList, spoofingFileList }, resultsDestiny);
			return 0;
		case 2:
			waveletExperiments::Experiment02::perform( { liveFileList, spoofingFileList }, resultsDestiny, numberOfTests, minModel, maxModel);
			return 0;
		case 3:
			waveletExperiments::Experiment03::perform( { liveFileList, spoofingFileList }, resultsDestiny, numberOfTests, minModel, maxModel);
			return 0;
		case 4:
			waveletExperiments::Experiment04::perform();
			return 0;
		case 5:
			waveletExperiments::Experiment05::perform( { liveFileList, spoofingFileList }, resultsDestiny);
			return 0;
		case 6:
			// TODO Neural network experiment
			return 0;
		case 7:
			waveletExperiments::Experiment07::perform( { liveFileList, spoofingFileList }, resultsDestiny, numberOfTests, minModel, maxModel);
			return 0;
		case 8:
			waveletExperiments::Experiment08::perform( { liveFileList, spoofingFileList }, resultsDestiny, numberOfTests, minModel, maxModel);
			return 0;
		case 0:
			waveletExperiments::Experiment01::perform( { liveFileList, spoofingFileList }, resultsDestiny);
			waveletExperiments::Experiment02::perform( { liveFileList, spoofingFileList }, resultsDestiny, numberOfTests, minModel, maxModel);
			waveletExperiments::Experiment03::perform( { liveFileList, spoofingFileList }, resultsDestiny, numberOfTests, minModel, maxModel);
			waveletExperiments::Experiment04::perform();
			waveletExperiments::Experiment05::perform( { liveFileList, spoofingFileList }, resultsDestiny);
			// TODO Neural network experiment
			waveletExperiments::Experiment07::perform( { liveFileList, spoofingFileList }, resultsDestiny, numberOfTests, minModel, maxModel);
			waveletExperiments::Experiment08::perform( { liveFileList, spoofingFileList }, resultsDestiny, numberOfTests, minModel, maxModel);
			return 0;
	}

	// Bad arguments nothing done!
	return -1;
}

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
int parseArguments(int argc, char *args[], std::string &liveFileList, std::string &spoofingFileList, std::string &resultsDestiny, unsigned int &numberOfTests, double &minModel, double &maxModel, int experiment)
{
	for (int j = 1; j < argc; ++j)
	{
		std::string argument(args[j]);

		if (argument.compare("--experiment") == 0)
		{
			experiment = std::stoi(args[j + 1]);
			++j;
			continue;
		}

		if (argument.compare("--live") == 0)
		{
			if (fileExistis(args[j + 1]))
			{
				liveFileList = args[j + 1];
				++j;
				continue;
			}
			std::cout << "The file: " << args[j + 1] << " do not existis\n" << std::endl;
		}
		if (argument.compare("--spoofing") == 0)
		{
			if (fileExistis(args[j + 1]))
			{
				spoofingFileList = args[j + 1];
				++j;
				continue;
			}
			std::cout << "The file: " << args[j + 1] << " do not existis\n" << std::endl;
		}
		if (argument.compare("--out") == 0)
		{
			if (fileExistis(args[j + 1]))
			{
				resultsDestiny = args[j + 1];
				++j;
				continue;
			}
			std::cout << "The directory: " << args[j + 1] << " do not existis\n" << std::endl;
		}
		if (argument.compare("--tests") == 0)
		{
			numberOfTests = std::stoi(args[j + 1]);
			++j;
			continue;
		}
		if (argument.compare("--minModel") == 0)
		{
			minModel = std::stod(args[j + 1]);
			++j;
			continue;
		}
		if (argument.compare("--maxModel") == 0)
		{
			maxModel = std::stod(args[j + 1]);
			++j;
			continue;
		}
	}

	if (experiment == -1)
	{
		std::cout << "Usage: mestrado --experiment <experiment number> --live <path list of wave files> --spoofing <path list of wave files> --out <results directory path> --tests <number of tests> --minModel <min size of model> --maxModel <max size of model>\n" << std::endl;
		std::cout << "You must choose an experiment:\n";
		std::cout << "\t1-Wavelet features vectors over BARK or MEL comparation\n";
		std::cout << "\t2-Wavelet features vector over BARK using distance classifiers\n";
		std::cout << "\t3-Wavelet features vector over BARK using SVM classifier\n";
		std::cout << "\t0-All experiments" << std::endl << std::endl;
		return -1;
	}

	if (experiment == 1 || experiment == 5)
	{
		if (argc != 9)
		{
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

	if (experiment == 2 || experiment == 3 || experiment == 0 || experiment == 6 || experiment == 7 || experiment == 8)
	{
		if (argc != 15)
		{
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

	if (experiment == 4)
	{
		return experiment;
	}

	std::cout << "Usage: mestrado --experiment <experiment number> --live <path list of wave files> --spoofing <path list of wave files> --out <results directory path> --tests <number of tests> --minModel <min size of model> --maxModel <max size of model>" << std::endl << std::endl;
	return -1;
}
