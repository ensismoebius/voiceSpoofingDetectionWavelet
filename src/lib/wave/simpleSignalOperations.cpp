/**
 * @author André Furlan
 * @email ensismoebius@gmail.com
 * This whole project are under GPLv3, for
 * more information read the license file
 *
 * 2 de abr de 2020
 *
 */

#include <cmath>
#include <vector>

std::vector<long double> amdf(std::vector<long double> vector)
{
	unsigned int resultIndex = -1;
	unsigned int N = vector.size();
	std::vector<long double> result(N);

	for (unsigned int k = 0; k < N; k++)
	{
		resultIndex++;
		for (unsigned int n = 0; n < N - k; n++)
			result[resultIndex] += std::abs(vector[n] - vector[n + k]);
	}
	return result;
}

unsigned int findFZeroPeriodSamples(std::vector<long double> vector)
{

	long double m = vector[0];
	unsigned int period = 0;
	unsigned int index = 0;

	for (unsigned int i = 1; i < vector.size(); i++)
		if (m > vector[i]) m = vector[i];

	while (vector[index] != m)
		index++;

	do
	{
		period++;
		index++;
	} while (vector[index] != m);

	return period;
}

void doAFineAmplification(double *signal, int signalLength)
{
	double highestSignal = 0;

	// find the highest signal
	for (int i = 0; i < signalLength; ++i)
	{
		double value = std::abs(signal[i]);

		if (value > highestSignal) highestSignal = value;

	}

	double multiplicationRatio = 32767 / highestSignal;

	for (int i = 0; i < signalLength; ++i)
	{
		signal[i] *= multiplicationRatio;
	}
}

void silentHalfOfTheSoundTrack(double *signal, int signalLength)
{
	int middleSignalIndex = signalLength / 2;

	for (int i = middleSignalIndex; i < signalLength; ++i)
	{
		signal[i] = 0;
	}
}

void xuxasDevilInvocation(double *signal, int signalLength)
{
	int middleSignalIndex = signalLength / 2;
	int tempVar;

	for (int i = middleSignalIndex; i < signalLength; ++i)
	{
		tempVar = signal[i];
		signal[i] = signal[signalLength - i - 1];
		signal[signalLength - i - 1] = tempVar;
	}
}

void halfVolume(double *signal, int signalLength)
{

	for (int i = 0; i < signalLength; ++i)
	{
		signal[i] *= .5;
	}
}

void addEchoes(double *signal, int signalLength)
{

	// the "time" sound get to bounce and back
	int bouncingTime = 100000;

	// Iterate over all values
	for (int i = 0; i < signalLength; ++i)
	{

		// while the data starts to end we decrease
		// the bouncing time to avoid access data
		// outside the array
		if (i + bouncingTime - 1 == signalLength)
		{
			bouncingTime--;
		}

		// we have to wait the bouncingTime before start echoing
		if (i > bouncingTime - 1)
		{

			// the resulting signal are going to be
			// the average of the current signal
			// plus 80% of the previous signal
			signal[i] = (signal[i - bouncingTime] * .8 + signal[i]) / 2;
		}
	}
}
