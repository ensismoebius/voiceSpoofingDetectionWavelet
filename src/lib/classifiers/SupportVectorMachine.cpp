/**
 * @author André Furlan
 * @email ensismoebius@gmail.com
 * This whole project are under GPLv3, for
 * more information read the license file
 *
 * 12 de mai de 2020
 *
 */

#ifndef SRC_LIB_CLASSIFIERS_SUPPORTVECTORMACHINE_CPP_
#define SRC_LIB_CLASSIFIERS_SUPPORTVECTORMACHINE_CPP_

#include <cmath>
#include <vector>
#include "../vector/vectorUtils.h"

class SupportVectorMachine {
	public:
		enum LABEL {
			POSITIVE = 1, NEGATIVE = -1
		};
	private:

		std::vector<std::vector<double>> trainningModels;
		std::vector<LABEL> trainningLabels;

		std::vector<std::vector<double>> hiddenLayer;
		std::vector<double> outputLayer;

		inline double radialDistance(std::vector<double> currentInputVector, std::vector<double> v2) {
			return std::exp(-euclidianDistance(currentInputVector, v2));
		}

	public:

		void addTrainningCases(std::vector<std::vector<double>> input, LABEL label) {
			this->trainningLabels.resize(this->trainningLabels.size() + input.size(), label);
			this->trainningModels.insert(this->trainningModels.end(), input.begin(), input.end());
		}

		void train() {
			this->hiddenLayer.clear();
			this->hiddenLayer.resize(this->trainningModels.size(), std::vector<double>(this->trainningModels.size()));

			// creating the coeficients
			for (unsigned int fi = 0; fi < this->trainningModels.size(); fi++) {
				for (unsigned int ii = 0; ii < this->trainningModels.size(); ii++) {
					this->hiddenLayer[fi][ii] = this->radialDistance(this->trainningModels[fi], this->trainningModels[ii]);
				}
			}

			this->hiddenLayer = { { 1, 1, 1, 7 }, { 2, 1, -1, 9 }, { 1, -2, 2, 2 } };
//			this->hiddenLayer = { { 4, 2, 1, -2, 3 }, { 3, -3, -1, -1, 2 }, { 3, 5, 1, 1, 0 }, { 1, -1, -1, 4, -2 } };

// Linear system solving:

			// Points to the best line that can nullify our values
			unsigned int bestLineForSubtration = 0;

			// Selecting the line on which we have the value to nullyfy
			for (unsigned int lineIndex = 1; lineIndex < this->hiddenLayer.size(); lineIndex++) {

				// Selecting the column on which we have the value to nullyfy
				for (unsigned columnIndex = 0; columnIndex < lineIndex; columnIndex++) {

					// If this value is already zero then we are ok, move on
					if (this->hiddenLayer[lineIndex][columnIndex] == 0) continue;

					// Otherwise we must find the best line for subtraction
					bestLineForSubtration = 0;
					for (; bestLineForSubtration < this->hiddenLayer.size(); bestLineForSubtration++) {

						// The line must have an value diferent of
						// zero at the position we want nullify
						if (this->hiddenLayer[bestLineForSubtration][columnIndex] == 0) continue;

						// The line must have zeros BEFORE the
						// current position we want to nullify
						bool zeroedBefore = true;
						for (unsigned int ci = 0; ci < columnIndex; ci++) {
							if (this->hiddenLayer[bestLineForSubtration][ci] != 0) {
								zeroedBefore = false;
								break;
							}
						}
						if (!zeroedBefore) continue;

						// We got it! Stop now!
						break;
					}

					// Ready to calculate the coefficient
					double coef = this->hiddenLayer[lineIndex][columnIndex] / this->hiddenLayer[bestLineForSubtration][columnIndex];

					for (unsigned int ci = 0; ci < this->hiddenLayer[lineIndex].size(); ci++) {
						this->hiddenLayer[lineIndex][ci] -= coef * this->hiddenLayer[bestLineForSubtration][ci];
					}
				}

			}

		}
};

#endif /* SRC_LIB_CLASSIFIERS_SUPPORTVECTORMACHINE_CPP_ */
