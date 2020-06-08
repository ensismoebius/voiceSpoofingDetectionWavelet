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
		public:

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

				this->initilizeWeights(this->weights_ih);
			}

			void addHiddenLayer(unsigned int hiddenLayerSize) {

				// If there is not hidden weights yet
				// connect it to a new weight
				if (this->weights.size() == 0) {

					// Weights that connects input to hidden
					this->weights_ih.resize(hiddenLayerSize, this->inputNodes); //ok!
					this->initilizeWeights(this->weights_ih); //ok!

//					// Weights that connects new hidden to output
//					arma::Mat<double> newHiddenWeight(this->outputNodes, hiddenLayerSize); //ok!
//					this->initilizeWeights(newHiddenWeight); //ok!
//
//					// The output will be the last
//					// result from the last weight
//					this->weights.push_back(newHiddenWeight);
//					this->hiddenNodes.push_back(hiddenLayerSize);
//					this->biases.push_back(arma::Col<double>(hiddenLayerSize));
//
//					// Nothing else to do exit
//					return;
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
				this->biases.push_back(arma::Col<double>(hiddenLayerSize));
			}

			std::vector<double> feedForward(std::vector<double> &input) {
				arma::Mat<double> output;
				this->feedForward(input, output);

				// Builds the output vector and returns it
				return std::vector<double>(output.Mat::mem, output.Mat::mem + output.n_rows);
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

				//// Generating the hidden outputs
				// Multiply inputs with the weights
				arma::Mat<double> hidden = this->weights_ih * arma::Mat<double>(input);
				// Sum the bias
				//hidden += this->bias_h;

				// Apply activation function
				hidden.for_each(this->functions.func);

				//// Generating the outputs
				// Multiply hidden with the weights
				//arma::Mat<double> outputs = this->weights_ho * hidden;
				// Sum the bias
				//outputs += this->bias_o;
				// Apply activation function
				//outputs.for_each(this->functions.func);

				//// Backpropagation starts here

				//////// From target to output
				//arma::Mat<double> outputErrors = arma::Mat<double>(target) - outputs;

				// Calculating the output gradient
				//arma::Mat<double> outputGradients = outputs;
				//outputGradients.for_each(this->functions.dfunc);
				//outputGradients %= outputErrors;
				//outputGradients *= this->learningRate;

				// Calculating hidden > output deltas weights
				//arma::Mat<double> weight_ho_deltas = outputGradients * hidden.t();

				// Updating weights
				//this->bias_o += outputGradients;
				//this->weights_ho += weight_ho_deltas;

				//////// From output to hidden
				//arma::Mat<double> hiddenErrors = this->weights_ho.t() * outputErrors;

				// Calculating the hidden gradient
				arma::Mat<double> hiddenGradients = hidden;
				hiddenGradients.for_each(this->functions.dfunc);
				//hiddenGradients %= hiddenErrors;
				hiddenGradients *= this->learningRate;

				// Calculating input > hidden deltas weights
				arma::Mat<double> weight_ih_deltas = hiddenGradients * arma::Mat<double>(input).t();

				// Updating weights
				//this->bias_h += hiddenGradients;
				this->weights_ih += weight_ih_deltas;
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
				//// Generating the hidden outputs
				// Multiply inputs with the weights
				arma::Mat<double> hidden = this->weights_ih * arma::Mat<double>(input);
				// Sum the bias
				//hidden += this->bias_h;
				// Apply activation function
				hidden.for_each(this->functions.func);

				//// Generating the output
				// Multiply hidden with the weights
				//output = this->weights_ho * hidden;
				// Sum the bias
				//output += this->bias_o;
				// Apply activation function
				output.for_each(this->functions.func);
			}

	};
}
#endif /* NeuralNetwork_H_ */

