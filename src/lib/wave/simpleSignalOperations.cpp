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
