#include <iostream>
#include "lib/classifiers/SupportVectorMachine.cpp"

void test(int argc, char *args[]) {

	classifiers::SupportVectorMachine s;

	std::vector<std::vector<double>> trainningInputA = { { 1, 2, 3 }, { 7, 8, 9 }, { 13, 14, 15 } };
	std::vector<std::vector<double>> trainningInputB = { { -1, -2, -3 }, { -7, -8, -9 }, { -13, -14, -15 } };

	s.addTrainningCases(trainningInputA, classifiers::SupportVectorMachine::POSITIVE);
	s.addTrainningCases(trainningInputB, classifiers::SupportVectorMachine::NEGATIVE);
	s.train();

	if (s.evaluate( { -4, -5, -6 }) == classifiers::SupportVectorMachine::POSITIVE) {
		std::cout << "Positivo" << std::endl;
	} else {
		std::cout << "Negativo" << std::endl;
	}

	if (s.evaluate( { 4, 5, 6 }) == classifiers::SupportVectorMachine::POSITIVE) {
		std::cout << "Positivo" << std::endl;
	} else {
		std::cout << "Negativo" << std::endl;
	}

	if (s.evaluate( { 4, -5, -1 }) == classifiers::SupportVectorMachine::POSITIVE) {
		std::cout << "Positivo" << std::endl;
	} else {
		std::cout << "Negativo" << std::endl;
	}
}