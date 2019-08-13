/**
 * @author André Furlan
 * @email ensismoebius@gmail.com
 * This whole project are under GPLv3, for
 * more information read the license file
 *
 * 13 de ago de 2019
 *
 */
static bool ciclo1 = true;
static bool ciclo2 = false;

static int comparacoes = 0;

static double* signal1 = 0;
static int signalSize1 = 0;
static double* signal2 = 0;
static int signalSize2 = 0;

static double* window1 = 0;
static int windowSize1 = 0;
static double* window2 = 0;
static int windowSize2 = 0;

static double* filter1 = 0;
static int filterSize1 = 0;
static double* filter2 = 0;
static int filterSize2 = 0;

static double* copiedSignal1 = 0;
static int copiedSignalSize1 = 0;
static double* copiedSignal2 = 0;
static int copiedSignalSize2 = 0;

static double* convolutedCopiedSignal1 = 0;
static int convolutedCopiedSignalSize1 = 0;
static double* convolutedCopiedSignal2 = 0;
static int convolutedCopiedSignalSize2 = 0;

static void setDebugSignal(double* data, int size) {

	if (ciclo1) {
		signalSize1 = size;
		signal1 = new double[size];
		for (int i = 0; i < size; i++) {
			signal1[i] = data[i];
		}
	}

	if (ciclo2) {
		signalSize2 = size;
		signal2 = new double[size];
		for (int i = 0; i < size; i++) {
			signal2[i] = data[i];
		}
	}
}
static void setDebugWindow(double* data, int size) {

	if (ciclo1) {
		windowSize1 = size;
		window1 = new double[size];
		for (int i = 0; i < size; i++) {
			window1[i] = data[i];
		}
	}

	if (ciclo2) {
		windowSize2 = size;
		window2 = new double[size];
		for (int i = 0; i < size; i++) {
			window2[i] = data[i];
		}
	}
}
static void setDebugFilter(double* data, int size) {

	if (ciclo1) {
		filterSize1 = size;
		filter1 = new double[size];
		for (int i = 0; i < size; i++) {
			filter1[i] = data[i];
		}
	}
	if (ciclo2) {
		filterSize2 = size;
		filter2 = new double[size];
		for (int i = 0; i < size; i++) {
			filter2[i] = data[i];
		}
	}
}
static void setDebugCopiedSignal(double* data, int size) {

	if (ciclo1) {
		copiedSignalSize1 = size;
		copiedSignal1 = new double[size];
		for (int i = 0; i < size; i++) {
			copiedSignal1[i] = data[i];
		}
	}
	if (ciclo2) {
		copiedSignalSize2 = size;
		copiedSignal2 = new double[size];
		for (int i = 0; i < size; i++) {
			copiedSignal2[i] = data[i];
		}
	}
}
static void setDebugConvolutedCopiedSignal(double* data, int size) {

	if (ciclo1) {
		convolutedCopiedSignalSize1 = size;
		convolutedCopiedSignal1 = new double[size];
		for (int i = 0; i < size; i++) {
			convolutedCopiedSignal1[i] = data[i];
		}
	}
	if (ciclo2) {
		convolutedCopiedSignalSize2 = size;
		convolutedCopiedSignal2 = new double[size];
		for (int i = 0; i < size; i++) {
			convolutedCopiedSignal2[i] = data[i];
		}
	}
}

static bool debugCompara() {

	comparacoes++;

	if (comparacoes < 2) {
		return false;
	}

	if (signalSize1 != signalSize2) {
		return false;
	}

	for (int i = 0; i < signalSize1; i++) {
		if (signal1[i] != signal2[i]) {
			return false;
		}
	}

	if (windowSize1 != windowSize2) {
		return false;
	}

	for (int i = 0; i < windowSize1; i++) {
		if (window1[i] != window2[i]) {
			return false;
		}
	}

	if (filterSize1 != filterSize2) {
		return false;
	}

	for (int i = 0; i < filterSize1; i++) {
		if (filter1[i] != filter2[i]) {
			return false;
		}
	}

	if (copiedSignalSize1 != copiedSignalSize2) {
		return false;
	}

	for (int i = 0; i < copiedSignalSize1; i++) {
		if (copiedSignal1[i] != copiedSignal2[i]) {
			return false;
		}
	}

	if (convolutedCopiedSignalSize1 != convolutedCopiedSignalSize2) {
		return false;
	}

	for (int i = 0; i < convolutedCopiedSignalSize1; i++) {
		if (convolutedCopiedSignal1[i] != convolutedCopiedSignal1[i]) {
			return false;
		}
	}

	return true;
}

