#include <cmath>
#include <string>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <algorithm>

#include "lib/wave/Wav.cpp"
#include "lib/wavelet/waveletCoeficients.h"

void normalizeData(double*, int);
double createAlpha(double, double, bool);
double* buildOrthogonalVector(double*, int);

void doAFineAmplification(double *signal, int signalLength) {
	double highestSignal = 0;

	// find the highest signal
	for (int i = 0; i < signalLength; ++i) {
		double value = abs(signal[i]);

		if (value > highestSignal) highestSignal = value;

	}

	double multiplicationRatio = 32767 / highestSignal;

	for (int i = 0; i < signalLength; ++i) {
		signal[i] *= multiplicationRatio;
	}
}

void silentHalfOfTheSoundTrack(double *signal, int signalLength) {
	int middleSignalIndex = signalLength / 2;

	for (int i = middleSignalIndex; i < signalLength; ++i) {
		signal[i] = 0;
	}
}

void xuxasDevilInvocation(double *signal, int signalLength) {
	int middleSignalIndex = signalLength / 2;
	int tempVar;

	for (int i = middleSignalIndex; i < signalLength; ++i) {
		tempVar = signal[i];
		signal[i] = signal[signalLength - i - 1];
		signal[signalLength - i - 1] = tempVar;
	}
}

void halfVolume(double *signal, int signalLength) {

	for (int i = 0; i < signalLength; ++i) {
		signal[i] *= .5;
	}
}

void addEchoes(double *signal, int signalLength) {

	// the "time" sound get to bounce and back
	int bouncingTime = 100000;

	// Iterate over all values
	for (int i = 0; i < signalLength; ++i) {

		// while the data starts to end we decrease
		// the bouncing time to avoid access data
		// outside the array
		if (i + bouncingTime - 1 == signalLength) {
			bouncingTime--;
		}

		// we have to wait the bouncingTime before start echoing
		if (i > bouncingTime - 1) {

			// the resulting signal are going to be
			// the average of the current signal
			// plus 80% of the previous signal
			signal[i] = (signal[i - bouncingTime] * .8 + signal[i]) / 2;
		}
	}
}

bool convolution(double *data, int dataLength, double *kernel, int kernelSize) {
	int i, j, k;

	double *convolutedSignal = new double[dataLength];

	// check validity of params
	if (!data || !convolutedSignal || !kernel) return false;
	if (dataLength <= 0 || kernelSize <= 0) return false;

	// start convolution from out[kernelSize-1] to out[dataSize-1] (last)
	for (i = kernelSize - 1; i < dataLength; ++i) {
		convolutedSignal[i] = 0;                             // init to 0 before accumulate

		for (j = i, k = 0; k < kernelSize; --j, ++k) {
			convolutedSignal[i] += data[j] * kernel[k];
		}
	}

	// convolution from out[0] to out[kernelSize-2]
	for (i = 0; i < kernelSize - 1; ++i) {
		convolutedSignal[i] = 0;                             // init to 0 before sum

		for (j = i, k = 0; j >= 0; --j, ++k) {
			convolutedSignal[i] += data[j] * kernel[k];
		}
	}

	std::copy(convolutedSignal, convolutedSignal + dataLength, data);

	delete[] convolutedSignal;

	return true;
}

void detectSilences(double *signal, int signalLength) {
	double lowestSignal = 0;
	int tolerance = 10;

	// find the highest signal
	for (int i = 0; i < signalLength; ++i) {
		double value = abs(signal[i]);

		if (value < lowestSignal) lowestSignal = value;

	}

	for (int i = 0; i < signalLength; ++i) {

		if (signal[i] <= lowestSignal + tolerance && signal[i] >= lowestSignal - tolerance) {
			std::cout << "\nSilence in the room!!!!" << signal[i];
		}
	}
}

double createAlpha(double samplingRate, double filterMaxFrequency, bool highPass = false) {

	double alpha = M_PI * filterMaxFrequency / (samplingRate / 2);

	if (highPass) {
		return M_PI - alpha;
	}

	return alpha;
}

void normalizeData(double *signal, int signalLength) {

	double sum = 0;

	for (int i = 0; i < signalLength; ++i) {
		sum += signal[i];
	}

	for (int i = 0; i < signalLength; ++i) {
		signal[i] /= sum;
	}

}

double* createLowPassFilter(int order, double samplingRate, double filterMaxFrequency) {

	// Order MUST be odd
	if (order % 2 == 0) {
		throw std::runtime_error("Order MUST be an odd number!");
		return 0;
	}

	double *filter = new double[order + 1];

	//Calculating the alpha
	double alpha = createAlpha(samplingRate, filterMaxFrequency);

	double halfOrderSize = (double) (order / 2.0);

	for (int n = 0; n <= order; ++n) {

		filter[n] = sin(alpha * (n - halfOrderSize)) / (M_PI * (n - halfOrderSize));
	}

	normalizeData(filter, order + 1);

	return filter;
}

double* buildOrthogonalVector(double *originalVector, int vectorSize) {

	double *finalResult = new double[vectorSize];

	int middleSignalIndex = vectorSize / 2;
	double tempVar;
	double inverter = 1.0;

	for (int i = middleSignalIndex; i < vectorSize; ++i) {
		tempVar = originalVector[i];

		finalResult[i] = originalVector[vectorSize - i - 1] * (-inverter);
		finalResult[vectorSize - i - 1] = tempVar * inverter;
		inverter *= -1.0;
	}

	return finalResult;
}

double* createHighPassFilter(int order, double samplingRate, double filterStartFrequency) {

	// Order MUST be odd
	if (order % 2 == 0) {
		throw std::runtime_error("Order MUST be an odd number!");
		return 0;
	}

	// Filter holder
	double *filter = new double[order + 1];

	//Calculating the alpha for high pass filter
	double alpha = createAlpha(samplingRate, filterStartFrequency, true);

	double halfOrderSize = (double) (order / 2.0);

	// Calculate low pass filter
	for (int n = 0; n <= order; ++n) {
		filter[n] = sin(alpha * (n - halfOrderSize)) / (M_PI * (n - halfOrderSize));
	}

	// normalizing data
	normalizeData(filter, order + 1);

	// Builds the orthogonal vector
	// and return the final result (high pass filter)
	return buildOrthogonalVector(filter, order + 1);
}

double* createBandPassFilter(int order, double samplingRate, double startFrequency, double finalFrequency) {

	// Order MUST be odd
	if (order % 2 == 0) {
		throw std::runtime_error("Order MUST be an odd number!");
		return 0;
	}

	double *lowPassMax = createLowPassFilter(order, samplingRate, finalFrequency);
	double *lowPassMin = createLowPassFilter(order, samplingRate, startFrequency);

	normalizeData(lowPassMax, order + 1);
	normalizeData(lowPassMin, order + 1);

	for (int i = 0; i < order + 1; i++) {
		lowPassMax[i] = lowPassMax[i] - lowPassMin[i];
	}

	delete[] lowPassMin;

	return lowPassMax;
}

double* bandStopFilter(int order, double samplingRate, double startFrequency, double finalFrequency) {

	// Order MUST be odd
	if (order % 2 == 0) {
		throw std::runtime_error("Order MUST be an odd number!");
		return 0;
	}

	double *highPass = createHighPassFilter(order, samplingRate, startFrequency);
	double *lowPass = createLowPassFilter(order, samplingRate, finalFrequency);

	normalizeData(highPass, order + 1);
	normalizeData(lowPass, order + 1);

	for (int i = 0; i < order + 1; i++) {
		lowPass[i] = lowPass[i] + highPass[i];
	}

	return lowPass;
}

double* createTriangularWindow(int order) {

	// order plus 1 is the amount of items
	double *w = new double[order + 1];

	// The reference point is amount of items divided by 2
	double referencePoint = order / 2.0;

	int n = 0;
	for (; n <= referencePoint; n++) {
		w[n] = 2.0 * n / order;
	}

	for (; n <= order; n++) {
		w[n] = 2.0 - 2.0 * n / order;
	}
	return w;
}

void applyWindow(double *filter, double *window, int order) {
	do {
		filter[order] *= window[order];
	} while (order--);
}

void discreteCosineTransform(double *vector, long vectorLength) {

	long N = vectorLength;
	double sum;
	double multi;
	double F[vectorLength];

	for (int u = 0; u < N; u++) {

		sum = 0.0;
		multi = u == 0 ? 1.0 / sqrt(2.0) : 1.0;

		for (int i = 0; i < N; i++) {
			sum += multi * cos(((M_PI * u) / (2.0 * N)) * (2.0 * i + 1)) * vector[u];
		}
		F[u] = sum;
	}

	double maior = 0;
	maior = F[1];

	for (int i = 2; i < N; i++) {

		if (F[i] > maior) {
			maior = F[i];
		}
	}

	for (int i = 1; i < N; i++) {
		F[i] /= maior;
	}

	for (int i = 0; i < N; i++) {
		vector[i] = F[i];
	}

}
double* createFeatureVector(double *signal, int signalLength, unsigned int samplingRate, int filterOrder, std::string path, bool logSmooth = false) {

	// size of the range
	int rangesSize = 14;

	// Ranges for MEL scale
	double ranges[14] = { 20, 160, 394, 670, 1000, 1420, 1900, 2450, 3120, 4000, 5100, 6600, 9000, 14000 };

	double *window = createTriangularWindow(filterOrder);

	// feature vector must be 1 space lesser than ranges
	double *featureVector = new double[rangesSize - 1];

	// Cleaning up the vector
	for (int i = 0; i < rangesSize - 1; i++) {
		featureVector[i] = 0;
	}

	/** Calculating the signal strength for each interval **/
	double rangeEnd = 0;
	double rangeStart = 0;

	// for every pair of ranges we need to copy the original
	double *copiedSignal = new double[signalLength];

	for (int i = 0; i < rangesSize - 1; i++) {

		// Select start and end ranges
		rangeStart = ranges[i];
		rangeEnd = ranges[i + 1];

		// Create the signal filter
		double *filter = createBandPassFilter(filterOrder, samplingRate, rangeStart, rangeEnd);

		// Apply window
		applyWindow(filter, window, filterOrder);

		// Create a copy of the signal
		std::copy(signal, signal + signalLength, copiedSignal);

		// Apply the filter
		convolution(copiedSignal, signalLength, filter, filterOrder);

		// dispose filter
		delete[] filter;

		// normalize signal
		normalizeData(copiedSignal, signalLength);

		// Calculating the energies
		double energy = 0;
		for (int j = 0; j < signalLength; j++) {

			// Calculate the energies for each energy interval
			energy = pow(copiedSignal[j], 2);

			if (logSmooth) {
				// apply log to it.
				energy = energy == 0 ? 0 : log(energy);
			}

			// Calculate the sum of all energies for this range
			featureVector[i] += energy;
		}

	}

	// Normalize the resulting feature vector
	normalizeData(featureVector, rangesSize - 1);

	// Apply a DCT (Discrete Cosine Transform)
	discreteCosineTransform(featureVector, rangesSize - 1);

	delete[] window;
	delete[] copiedSignal;
	return featureVector;
}

unsigned int resultIndex = 0;
std::string **results = 0;

void analiticFunction(double *signal, int signalLength, unsigned int samplingRate, std::string path) {

	unsigned int filterOrder = 27;

	std::stringstream dataColumn;

	dataColumn << "Digit:" << path.substr(path.find_last_of('/') - 1, 1) << "-";
	dataColumn << "Signal:" << path.substr(path.find_last_of('/') + 1, 255);

	results[resultIndex] = new std::string[14];
	results[resultIndex][0] = dataColumn.str();

	double *fv = createFeatureVector(signal, signalLength, samplingRate, filterOrder, path);
	for (int i = 0; i < 13; i++) {
		results[resultIndex][i + 1] = std::to_string(fv[i]);
	}
	delete[] fv;

	resultIndex++;
}

void transforFunction(double *signal, int signalLength, unsigned int samplingRate, std::string path) {

	// unsigned int filterOrder = 27;

	// detectSilences(signal, comprimento_do_sinal);
	// xuxasDevilInvocation(signal, signalLength);
	// addEchoes(signal, signalLength);
	// doAFineAmplification(signal, signalLength);
	// silentHalfOfTheSoundTrack(signal, signalLength);
	//	double* filter = createBandPassFilter(filterOrder, samplingRate, 670, 1000);
	//	double* window = createTriangularWindow(filterOrder);
	//	applyWindow(filter, window, filterOrder);
	//	convolution(signal, signalLength, filter, filterOrder);
	//
	//	delete[] filter;
	//	delete[] window;
}

// Function that return
// dot product of two vector array.
double dotProduct(std::vector<double> a, std::vector<double> b) {

	long double product = 0;

	// Loop for calculate cot product
	for (unsigned int i = 0; i < a.size(); i++) {
		product = product + a.at(i) * b.at(i);
	}
	return product;
}

int main(int i, char *args[]) {

//	std::cout << std::fixed;
//	std::cout << std::setprecision(20);
//
//	results = new std::string*[200];
//
//	Wav w;
//	w.setCallbackFunction(analiticFunction);
//
//	std::ifstream fileListStream;
//	fileListStream.open(args[1], std::ios::in);
//
//	std::string line;
//	while (std::getline(fileListStream, line)) {
//		std::cout << resultIndex << ":" << line << std::endl;
//
//		// lines that begins with # are going to be ignored
//		if (line.find("#") == 0) continue;
//
//		w.read(line.data());
//		w.process();
//		//	w.write("/tmp/teste.wav");
//	}
//
//	for (unsigned int columns = 0; columns < 14; columns++) {
//		for (unsigned int files = 0; files < resultIndex; files++) {
//			std::cout << results[files][columns] << "\t";
//		}
//		std::cout << std::endl;
//	}

	std::vector<double> original = wavelets::daub18;
	std::vector<double> orthogonal = wavelets::calcOrthogonal(original);

	double res = dotProduct(orthogonal, original);

	return 0;
}
