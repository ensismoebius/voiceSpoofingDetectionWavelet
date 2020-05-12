#include "lib/classifiers/SupportVectorMachine.cpp"

int main(int argc, char *args[]) {

	SupportVectorMachine s;

	std::vector<std::vector<double>> trainningInputA = { { 1, 2, 3, 4, 5, 6 }, { 7, 8, 9, 10, 11, 12 }, { 13, 14, 15, 16, 17, 18 } };
	std::vector<std::vector<double>> trainningInputB = { { -1, -2, -3, -4, -5, -6 }, { -7, -8, -9, -10, -11, -12 }, { -13, -14, -15, -16, -17, -18 } };

	s.addTrainningCases(trainningInputA, SupportVectorMachine::POSITIVE);
	s.addTrainningCases(trainningInputB, SupportVectorMachine::NEGATIVE);

	s.train();
}
