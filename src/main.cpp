#include <cmath>
#include <iomanip>
#include <iostream>
#include <stdexcept>

#include "debug.cpp"
#include "lib/wave/Wav.cpp"

int abs(int __x);

void normalizeData(double*, int);
double createAlpha(double, double, bool);
double* buildOrthogonalVector(double*, int);

void doAFineAmplification(double* signal, int signalLength) {
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

void silentHalfOfTheSoundTrack(double* signal, int signalLength) {
	int middleSignalIndex = signalLength / 2;

	for (int i = middleSignalIndex; i < signalLength; ++i) {
		signal[i] = 0;
	}
}

void xuxasDevilInvocation(double* signal, int signalLength) {
	int middleSignalIndex = signalLength / 2;
	int tempVar;

	for (int i = middleSignalIndex; i < signalLength; ++i) {
		tempVar = signal[i];
		signal[i] = signal[signalLength - i - 1];
		signal[signalLength - i - 1] = tempVar;
	}
}

void halfVolume(double* signal, int signalLength) {

	for (int i = 0; i < signalLength; ++i) {
		signal[i] *= .5;
	}
}

void addEchoes(double* signal, int signalLength) {

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

void convolution(double* data, int dataLength, double* inverseDFTFilter, int inverseDFTFilterLength) {

	// holds the final convoluted data
	double* convolutedSignal = new double[dataLength + inverseDFTFilterLength - 1];

	// the the convoluted data index
	int convIndex = 0;

	// Iterates over filter
	for (int filterIndex = 0; filterIndex < inverseDFTFilterLength; ++filterIndex) {

		// this guarantee the digit shift
		convIndex = filterIndex;

		// do the the math
		convolutedSignal[convIndex] = 0;
		for (int dataIndex = 0; dataIndex < dataLength; dataIndex++) {
			convolutedSignal[convIndex] += data[dataIndex] * inverseDFTFilter[filterIndex];
			convIndex++;
		}
	}

	for (int i = 0; i < dataLength; i++) {
		data[i] = convolutedSignal[i];
	}

	delete[] convolutedSignal;
}

void detectSilences(double* signal, int signalLength) {
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

void normalizeData(double* signal, int signalLength) {

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

	double* filter = new double[order + 1];

	//Calculating the alpha
	double alpha = createAlpha(samplingRate, filterMaxFrequency);

	double halfOrderSize = (double) (order / 2.0);

	for (int n = 0; n <= order; ++n) {

		filter[n] = sin(alpha * (n - halfOrderSize)) / (M_PI * (n - halfOrderSize));
	}

	normalizeData(filter, order + 1);

	return filter;
}

double* buildOrthogonalVector(double* originalVector, int vectorSize) {

	double* finalResult = new double[vectorSize];

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
	double* filter = new double[order + 1];

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

double *createBandPassFilter(int order, double samplingRate, double startFrequency, double finalFrequency) {

	// Order MUST be odd
	if (order % 2 == 0) {
		throw std::runtime_error("Order MUST be an odd number!");
		return 0;
	}

	double* lowPassMax = createLowPassFilter(order, samplingRate, finalFrequency);
	double* lowPassMin = createLowPassFilter(order, samplingRate, startFrequency);

	normalizeData(lowPassMax, order + 1);
	normalizeData(lowPassMin, order + 1);

	for (int i = 0; i < order + 1; i++) {
		lowPassMax[i] = lowPassMax[i] - lowPassMin[i];
	}

	delete[] lowPassMin;

	return lowPassMax;
}

double *bandStopFilter(int order, double samplingRate, double startFrequency, double finalFrequency) {

	// Order MUST be odd
	if (order % 2 == 0) {
		throw std::runtime_error("Order MUST be an odd number!");
		return 0;
	}

	double* highPass = createHighPassFilter(order, samplingRate, startFrequency);
	double* lowPass = createLowPassFilter(order, samplingRate, finalFrequency);

	normalizeData(highPass, order + 1);
	normalizeData(lowPass, order + 1);

	for (int i = 0; i < order + 1; i++) {
		lowPass[i] = lowPass[i] + highPass[i];
	}

	return lowPass;
}

double *createTriangularWindow(int order) {

	// order plus 1 is the amount of items
	double* w = new double[order + 1];

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

void applyWindow(double* filter, double *window, int order) {
	do {
		filter[order] *= window[order];
	} while (order--);
}

void discreteCosineTransform(double* vector, long vectorLength) {

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

double* createFeatureVector(double* signal, int signalLength, int order, double samplingRate) {
	// Ranges for MEL scale
	double ranges[14] = { 20, 160, 394, 670, 1000, 1420, 1900, 2450, 3120, 4000, 5100, 6600, 9000, 14000 };

	// Ranges for BARK scale
	//double ranges[25] = { 20, 100, 200, 300, 400, 510, 630, 770, 920, 1080, 1270, 1480, 1720, 2000, 2320, 2700, 3150, 3700, 4400, 5300, 6400, 7700, 9500, 12000, 15500 };

	int rangesSize = 14;
	double rangeEnd = 0;
	double rangeStart = 0;

	double* filter = 0;
	double* window = createTriangularWindow(order);

	// feature vector must be 1 space lesser than ranges
	double* featureVector = new double[rangesSize - 1];

	// for every pair of ranges we need to copy the original
	double* copiedSignal = new double[signalLength];

	double energy = 0;

	// Cleaning up the vector
	for (int i = 0; i < rangesSize - 1; i++) {
		featureVector[i] = 0;
	}

	for (int i = 0; i < rangesSize - 1; i++) {

		// Select start and end ranges
		rangeStart = ranges[i];
		rangeEnd = ranges[i + 1];

		// Create a copy of the signal
		for (int j = 0; j < signalLength; j++) {
			copiedSignal[j] = signal[j];
		}

		// Create the filter
		filter = createBandPassFilter(order, samplingRate, rangeStart, rangeEnd);
		// Apply window
		applyWindow(filter, window, order);

		//############ DEBUG ###############//
		if (ciclo1) {
			setDebugSignal(signal, signalLength);
			setDebugWindow(window, order);
			setDebugFilter(filter, order + 1);
			setDebugCopiedSignal(copiedSignal, signalLength);
		}
		if (ciclo2) {
			setDebugSignal(signal, signalLength);
			setDebugWindow(window, order);
			setDebugFilter(filter, order + 1);
			setDebugCopiedSignal(copiedSignal, signalLength);
		}
		//############ DEBUG ###############//

		// Apply the filter
		convolution(copiedSignal, signalLength, filter, order);

		//############ DEBUG ###############//
		if (ciclo1) {
			setDebugConvolutedCopiedSignal(copiedSignal, signalLength);
			ciclo1 = false;
		}
		if (ciclo2) {
			setDebugConvolutedCopiedSignal(copiedSignal, signalLength);
			ciclo2 = false;
		}
		//############ DEBUG ###############//

		// dispose filter
		delete[] filter;

		// normalize signal
		normalizeData(copiedSignal, signalLength);

		energy = 0;
		for (int j = 0; j < signalLength; j++) {
			// Calculate the energies for each energy interval, apply log to it.
			energy = pow(copiedSignal[j], 2);
			energy = energy == 0 ? 0 : log2(energy);

			// Calculate the sum of all energies for this range
			featureVector[i] += energy;
		}
	}

	//############ DEBUG ###############//
	debugCompara();
	ciclo2 = true;
	//############ DEBUG ###############//

	// Normalize the resulting feature vector
	normalizeData(featureVector, rangesSize - 1);

	// Apply a DCT (Discrete Cosine Transform)
	discreteCosineTransform(featureVector, rangesSize - 1);

	delete[] window;
	delete[] copiedSignal;
	return featureVector;
}

void transformFunction(double* signal, int signalLength, unsigned int samplingRate) {

	// FIXME Obviously something id wrong when the order is lesser
	// The first feature vector value of the first feature vector gets too high
	unsigned int filterOrder = 1001;

	// detectSilences(signal, comprimento_do_sinal);
	// xuxasDevilInvocation(signal, signalLength);
	// addEchoes(signal, signalLength);
	// doAFineAmplification(signal, signalLength);
	// silentHalfOfTheSoundTrack(signal, signalLength);

	std::cout << std::fixed;
	std::cout << std::setprecision(20);

	double* fv = createFeatureVector(signal, signalLength, filterOrder, samplingRate);
	for (int i = 0; i < 13; i++) {
		std::cout << fv[i] << ",";
	}
	std::cout << std::endl;
	delete[] fv;

	//	double* filter = createBandPassFilter(filterOrder, samplingRate, 670, 1000);
	//	double* window = createTriangularWindow(filterOrder);
	//	applyWindow(filter, window, filterOrder);
	//	convolution(signal, signalLength, filter, filterOrder);
	//
	//	delete[] filter;
	//	delete[] window;
}

int main(int i, char* args[]) {
	Wav w;
	w.setCallbackFunction(transformFunction);

	w.read(args[1]);
	w.process();
//	w.write("/tmp/teste.wav");

	for (int k = 2; k < i; k++) {

		w.read(args[k]);
		w.process();
		//w.write("/tmp/teste.wav");

	}
	return 0;
}

