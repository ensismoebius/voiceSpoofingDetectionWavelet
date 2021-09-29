#include <vector>
#include <cassert>
#include <gtest/gtest.h>
#include "SupportVectorMachine.h"

classifiers::SupportVectorMachine s;

std::vector<std::vector<long double>> trainingInputA;
std::vector<std::vector<long double>> trainingInputB;

void initialize(int argc, char *args[])
{
	trainingInputA = {{1, 2, 3}, {7, 8, 9}, {13, 14, 15}};
	trainingInputB = {{-1, -2, -3}, {-7, -8, -9}, {-13, -14, -15}};

	s.addTrainingCases(trainingInputA, classifiers::SupportVectorMachine::POSITIVE);
	s.addTrainingCases(trainingInputB, classifiers::SupportVectorMachine::NEGATIVE);
	s.train();
}

TEST(SupportVectorMachine, classification)
{
	ASSERT_EQ(s.evaluate({4, 5, 6}), classifiers::SupportVectorMachine::POSITIVE);
	ASSERT_EQ(s.evaluate({4, -5, -1}), classifiers::SupportVectorMachine::NEGATIVE);
	ASSERT_EQ(s.evaluate({-4, -5, -6}), classifiers::SupportVectorMachine::NEGATIVE);
}

int main(int argc, char *argv[])
{
	initialize(argc, argv);
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
