/**
 * @author André Furlan
 * @email ensismoebius@gmail.com
 * This whole project are under GPLv3, for
 * more information read the license file
 *
 * 7 de jun de 2020
 *
 */
#include <iostream>
#include <algorithm>

#include "../lib/classifiers/NeuralNetwork.cpp"
void neuralNetwork() {
	struct trainigSample {
			std::vector<double> inputs;
			std::vector<double> targets;

			trainigSample(std::vector<double> inputs, std::vector<double> targets) {
				this->inputs = inputs;
				this->targets = targets;
			}
	};

	std::vector<trainigSample> samples;

	samples.push_back(trainigSample( { 0, 1 }, { 1 }));
	samples.push_back(trainigSample( { 1, 0 }, { 1 }));
	samples.push_back(trainigSample( { 0, 0 }, { 0 }));
	samples.push_back(trainigSample( { 1, 1 }, { 0 }));

	classifiers::NeuralNetwork nn(2, 1, 0.01, classifiers::NeuralNetwork::leakyRelu, classifiers::NeuralNetwork::dleakyRelu);

	for (int i = 0; i < 2000; i++) {

		std::random_shuffle(samples.begin(), samples.end());

		for (auto sample : samples) {
			nn.train(sample.inputs, sample.targets);
		}
	}

	for (auto sample : samples) {
		std::cout << "Target|" + std::to_string(sample.targets[0]);
		std::cout << "|" << nn.feedForward(sample.inputs)[0] << "|" << std::endl;
	}
}
