#include "lib/classifiers/SupportVectorMachine.cpp"

int main(int argc, char *args[]) {

	SupportVectorMachine s;

	std::vector<std::vector<double>> trainningInputA = { { 1, 2, 3 }, { 7, 8, 9 }, { 13, 14, 15 } };
	std::vector<std::vector<double>> trainningInputB = { { -1, -2, -3 }, { -7, -8, -9 }, { -13, -14, -15 } };

	s.addTrainningCases(trainningInputA, SupportVectorMachine::POSITIVE);
	s.addTrainningCases(trainningInputB, SupportVectorMachine::NEGATIVE);

	s.train();
}
