/**
 * @author André Furlan
 * @email ensismoebius@gmail.com
 * This whole project are under GPLv3, for
 * more information read the license file
 *
 * Based on code of Daniel Shiffman (thanks man!!)
 * @ref https://github.com/CodingTrain/Toy-Neural-Network-JS
 *
 * 5 de jun de 2020
 *
 */
#ifndef NeuralNetwork_H_
#define NeuralNetwork_H_

#include <vector>
#include <random>
#include <stdexcept>
#include <armadillo>
namespace classifiers {

	class NeuralNetwork {
		private:
			////////////////////////////////////
			/// Temp variables for trainning ///
			////////////////////////////////////
			size_t biasSize;
			size_t biasIndex;
			int layerIndex;
			int weightIndex;
			arma::Mat<double> errors;
			arma::Mat<double> gradients;
			arma::Mat<double> hiddenLayer;
			arma::Mat<double> deltaWeights;
			std::vector<arma::Mat<double>> layers;
			////////////////////////////////////
			////////////// End /////////////////
			////////////////////////////////////

			/////////////////////////////////////////
			/// Temp variables for classification ///
			/////////////////////////////////////////
			arma::Mat<double> output;
			////////////////////////////////////
			////////////// End /////////////////
			////////////////////////////////////

			struct activationFunction {
					void (*func)(double &v);
					void (*dfunc)(double &v);
			};

			// Info about input and output layers
			unsigned int inputNodes;
			unsigned int outputNodes;
			arma::Mat<double> weights_ih;

			// Info about the hidden layers
			std::vector<unsigned int> hiddenNodes;
			std::vector<arma::Mat<double>> weights;
			std::vector<arma::Col<double>> biases;

			double learningRate;
			activationFunction functions;
		public:

			inline static void sigmoid(double &v) {
				// sigmoid: 1 / (1 + std::exp(-v))
				v = 1 / (1 + std::exp(-v));
			}

			inline static void dsigmoid(double &sigmoided) {
				// sigmoid: 1 / (1 + std::exp(-v))
				// dsigmoid: 1 / (1 + std::exp(-v)) * (1 - 1 / (1 + std::exp(-v)))
				// i.e. sigmoid derivative dsigmoid = sigmoid + (1 - sigmoid)
				sigmoided = sigmoided * (1 - sigmoided);
			}

			inline static void leakyRelu(double &v) {
				v = v > 0 ? v : 0.01 * v;
			}

			inline static void dleakyRelu(double &v) {
				v = v > 0 ? 1 : 0.5;
			}

			NeuralNetwork(unsigned int inputNodes, unsigned int outputNodes, double learningRate, void (*activationFunction)(double &v), void (*derivativeOfActivationFunction)(double &v)) {

				this->inputNodes = inputNodes;
				this->outputNodes = outputNodes;

				// Weights that connects input to output
				this->weights_ih.set_size(this->outputNodes, this->inputNodes); //ok!
				this->biases.push_back(arma::Col<double>(this->outputNodes));

				this->learningRate = 0.01;
				this->functions.func = activationFunction;
				this->functions.dfunc = derivativeOfActivationFunction;

				this->initilizeWeights(this->biases[0]);
				this->initilizeWeights(this->weights_ih);
			}

			void addHiddenLayer(unsigned int hiddenLayerSize) {

				// If there is not hidden weights yet
				// connect it to a new weight
				if (this->weights.size() == 0) {
					// Weights that connects input to hidden
					this->weights_ih.resize(hiddenLayerSize, this->inputNodes); //ok!
					this->initilizeWeights(this->weights_ih); //ok!
				} else {
					// Resize the last weight
					arma::Mat<double> &lastWeight = weights[weights.size() - 1]; //ok!
					lastWeight.resize(hiddenLayerSize, lastWeight.n_cols); //ok!
				}

				// Create new weight
				arma::Mat<double> newHiddenWeight(this->outputNodes, hiddenLayerSize); //ok!
				this->initilizeWeights(newHiddenWeight); //ok!

				// Adds the weight
				this->weights.push_back(newHiddenWeight);
				this->hiddenNodes.push_back(hiddenLayerSize);

				// Reordering the biases
				auto outputBias = this->biases[this->biases.size() - 1];
				auto newBias = arma::Col<double>(hiddenLayerSize);
				this->initilizeWeights(newBias);

				this->biases.resize(this->biases.size() + 1);
				this->biases[this->biases.size() - 2] = newBias;
				this->biases[this->biases.size() - 1] = outputBias;

				// Allocate memory for trainning
				layers.reserve(this->weights.size() + 1);
			}

			std::vector<double> feedForward(std::vector<double> &input) {
				this->feedForward(input, this->output);

				// Builds the output vector and returns it
				return std::vector<double>(this->output.Mat::mem, this->output.Mat::mem + this->output.n_rows);
			}

			void showLayers() {
				std::cout << "Input" << std::endl;
				std::cout << arma::Col<int>(this->weights_ih.n_cols) * 0 << std::endl;

				std::cout << "Hidden" << std::endl;
				for (auto w : this->weights) {
					std::cout << arma::Col<int>(w.n_cols) * 0 << std::endl;
				}

				std::cout << "Output" << std::endl;
				std::cout << arma::Col<int>(this->weights[this->weights.size() - 1].n_rows) * 0 << std::endl;
			}

			void train(std::vector<double> &input, std::vector<double> &target) {

				biasIndex = 0;
				biasSize = this->biases.size();

				//// Feedforwarding all input to hidden layer
				// Multiply inputs with the weights
				this->hiddenLayer = this->weights_ih * arma::Mat<double>(input);
				// Sum the the first bias
				this->hiddenLayer += this->biases[biasIndex];
				// Apply activation function
				this->hiddenLayer.for_each(this->functions.func);

				// We need to store layers values for trainning
				// The number of layers is this->weights.size() + 1
				this->layers.push_back(arma::Mat<double>(input));
				this->layers.push_back(this->hiddenLayer);

				//// Feedforwarding all hidden layers and the output layer
				for (auto &w : this->weights) {
					this->hiddenLayer = w * this->hiddenLayer;
					this->hiddenLayer.for_each(this->functions.func);

					this->hiddenLayer += this->biases[++biasIndex];
					this->layers.push_back(hiddenLayer);
				}

				//// Backpropagation starts here
				this->weightIndex = 0;
				this->layerIndex = this->layers.size() - 1;

				this->errors = arma::Mat<double>(target) - layers[layerIndex];
				this->weights.push_back(this->weights_ih);
				std::rotate(this->weights.rbegin(), this->weights.rbegin() + 1, this->weights.rend());

				for (; layerIndex > 0; layerIndex--) {

					// Calculating the output gradient
					this->gradients = this->layers[layerIndex];
					this->gradients.for_each(this->functions.dfunc);
					this->gradients %= this->errors;
					this->gradients *= this->learningRate;

					// Calculating hidden > output deltas weights
					this->deltaWeights = gradients * layers[layerIndex - 1].t();

					// Updating weights
					this->biases[layerIndex - 1] += gradients;
					this->weights[layerIndex - 1] += deltaWeights;

					// Calculating next error
					this->errors = this->weights[layerIndex - 1].t() * this->errors;
				}

				std::rotate(this->weights.begin(), this->weights.begin() + 1, this->weights.end());
				this->weights.pop_back();
				this->layers.clear();
			}

		private:

			/**
			 * Initializes weights beetween -1 and 1
			 * @param weights
			 */
			inline static void initilizeWeights(arma::Mat<double> &weights) {
				weights.randu();
				weights -= .5;
				weights *= 2;
			}

			void feedForward(std::vector<double> &input, arma::Mat<double> &output) {
				biasIndex = 0;
				biasSize = this->biases.size();

				//// Feedforwarding all input to hidden layer
				// Multiply inputs with the weights
				output = this->weights_ih * arma::Mat<double>(input);
				// Sum the the first bias
				output += this->biases[biasIndex];
				// Apply activation function
				output.for_each(this->functions.func);

				//// Feedforwarding all hidden layers and the output layer
				for (auto &w : this->weights) {
					output = w * output;
					output.for_each(this->functions.func);

					output += this->biases[++biasIndex];
				}
			}
	};
}
#endif /* NeuralNetwork_H_ */

