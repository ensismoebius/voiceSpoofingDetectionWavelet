#include <vector>
#include <algorithm>
#include <stdexcept>

namespace classifiers {

	void raflleFeaturesVectors(std::vector<std::vector<double>> &source, std::vector<std::vector<double>> &model, std::vector<std::vector<double>> &test, float modelSize) {

		// Validates modelsize
		if (modelSize > 1 || !(modelSize > 0)) {
			throw std::runtime_error("Model size must be between 0 and 1 (Zero not included)");
		}

		// Create a vector of indexes because it is easier
		// to shuffle the indexes than the given vectors
		// itself
		std::vector<unsigned int> indexes(source.size());
		for (unsigned int counter = 0; counter < source.size(); counter++) {
			indexes[counter] = counter;
		}

		// Shuffle the indexes
		std::random_shuffle(indexes.begin(), indexes.end());

		// Based on the modelSize informed percentage
		// calculates the amount of points needed
		unsigned int amountOfModelSamples = modelSize * source.size();

		// Resize the the ones who will receive
		// the scrambled vectors
		model.resize(amountOfModelSamples);
		test.resize(source.size() - amountOfModelSamples);

		// fills the model
		for (unsigned int i = 0; i < amountOfModelSamples; i++) {
			model[i] = source[indexes[i]];
		}

		// fills the test
		for (unsigned int i = amountOfModelSamples; i < source.size(); i++) {
			test[i - amountOfModelSamples] = source[indexes[i]];
		}
	}
}
