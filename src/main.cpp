//void waveletTransformTestFunction() {
//	std::vector<double> signal = { 32, 10, 20, 38, 37, 28, 38, 34, 18, 24, 24, 9, 23, 24, 28, 34 };
//	int length = 16;
//	waveletAnaliticFunction(signal, length, 44100, "ssss");
//}

#include "experiments/01/experiment01.h"

int main(int i, char *args[]) {

	waveletExperiments::perform(args, i);

	return 0;
}
