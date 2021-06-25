/**
 * @author André Furlan
 * @email ensismoebius@gmail.com
 * This whole project are under GPLv3, for
 * more information read the license file
 *
 * 2 de abr de 2020
 *
 */

#ifndef SRC_LIB_SIMPLESIGNALOPERATIONS_H_
#define SRC_LIB_SIMPLESIGNALOPERATIONS_H_

#include <vector>

/**
 * Average Magnitude Difference Function
 * @param vector
 * @return amdf vector
 */
std::vector<long double> amdf(std::vector<long double> vector);

/**
 * Returns the amount of samples in order to
 * calculate the first formant of the signal
 * @param vector
 * @return integer
 */
unsigned int findFZeroPeriodSamples(std::vector<long double> vector);

void doAFineAmplification(double *signal, int signalLength);

void silentHalfOfTheSoundTrack(double *signal, int signalLength);

void xuxasDevilInvocation(double *signal, int signalLength);

void halfVolume(double *signal, int signalLength);

void addEchoes(double *signal, int signalLength);

#endif SRC_LIB_SIMPLESIGNALOPERATIONS_H_
