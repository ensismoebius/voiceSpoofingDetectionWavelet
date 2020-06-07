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

			unsigned int inputNodes;
			unsigned int hiddenNodes;
			unsigned int outputNodes;

			arma::Mat<double> weights_ih;
			arma::Mat<double> weights_ho;

			arma::Col<double> bias_h;
			arma::Col<double> bias_o;

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

			NeuralNetwork(unsigned int inputNodes, unsigned int hiddenNodes, unsigned int outputNodes, double learningRate, void (*activationFunction)(double &v), void (*derivativeOfActivationFunction)(double &v)) {
				this->inputNodes = inputNodes;
				this->hiddenNodes = hiddenNodes;
				this->outputNodes = outputNodes;

				this->weights_ih.set_size(this->hiddenNodes, this->inputNodes);
				this->weights_ho.set_size(this->outputNodes, this->hiddenNodes);

				this->bias_h.set_size(this->hiddenNodes);
				this->bias_o.set_size(this->outputNodes);

				this->learningRate = 0.01;
				this->functions.func = activationFunction;
				this->functions.dfunc = derivativeOfActivationFunction;

				this->initilizeWeights(this->weights_ih);
				this->initilizeWeights(this->weights_ho);
				this->initilizeWeights(this->bias_h);
				this->initilizeWeights(this->bias_o);
			}

			std::vector<double> feedForward(std::vector<double> &input) {

				arma::Mat<double> output;
				this->feedForward(input, output);

				// Builds the output vector and returns it
				return std::vector<double>(output.Mat::mem, output.Mat::mem + output.n_rows);
			}

			void train(std::vector<double> &input, std::vector<double> &target) {

				//// Generating the hidden outputs
				// Multiply inputs with the weights
				arma::Mat<double> hidden = this->weights_ih * arma::Mat<double>(input);
				// Sum the bias
				hidden += this->bias_h;
				// Apply activation function
				hidden.for_each(this->functions.func);

				//// Generating the outputs
				// Multiply hidden with the weights
				arma::Mat<double> outputs = this->weights_ho * hidden;
				// Sum the bias
				outputs += this->bias_o;
				// Apply activation function
				outputs.for_each(this->functions.func);

				//// Backpropagation starts here

				//////// From target to output
				arma::Mat<double> outputErrors = arma::Mat<double>(target) - outputs;

				// Calculating the output gradient
				arma::Mat<double> outputGradients = outputs;
				outputGradients.for_each(this->functions.dfunc);
				outputGradients %= outputErrors;
				outputGradients *= this->learningRate;

				// Calculating hidden > output deltas weights
				arma::Mat<double> weight_ho_deltas = outputGradients * hidden.t();

				// Updating weights
				this->bias_o += outputGradients;
				this->weights_ho += weight_ho_deltas;

				//////// From output to hidden
				arma::Mat<double> hiddenErrors = this->weights_ho.t() * outputErrors;

				// Calculating the hidden gradient
				arma::Mat<double> hiddenGradients = hidden;
				hiddenGradients.for_each(this->functions.dfunc);
				hiddenGradients %= hiddenErrors;
				hiddenGradients *= this->learningRate;

				// Calculating input > hidden deltas weights
				arma::Mat<double> weight_ih_deltas = hiddenGradients * arma::Mat<double>(input).t();

				// Updating weights
				this->bias_h += hiddenGradients;
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
				hidden += this->bias_h;
				// Apply activation function
				hidden.for_each(this->functions.func);

				//// Generating the output
				// Multiply hidden with the weights
				output = this->weights_ho * hidden;
				// Sum the bias
				output += this->bias_o;
				// Apply activation function
				output.for_each(this->functions.func);
			}

	};
}
#endif /* NeuralNetwork_H_ */

