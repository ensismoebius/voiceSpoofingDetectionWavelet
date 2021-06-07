/**
 * @author André Furlan
 * @email ensismoebius@gmail.com
 * This whole project are under GPLv3, for
 * more information read the license file
 *
 * 2 de abr de 2020
 *
 */
#ifndef SRC_LIB_FILTEROPERATIONS_H_
#define SRC_LIB_FILTEROPERATIONS_H_

/**
 * Create alpha value
 * @param samplingRate
 * @param filterMaxFrequency
 * @param highPass
 * @return alpha
 */
long double createAlpha(double samplingRate, double filterMaxFrequency, bool highPass = false);

/**
 * Create lowpass filter
 * @param order
 * @param samplingRate
 * @param filterMaxFrequency
 * @return lowpass filter
 */
long double* createLowPassFilter(int order, double samplingRate, double filterMaxFrequency);

/**
 * Create highpass filter
 * @param order
 * @param samplingRate
 * @param filterStartFrequency
 * @return highpass filter
 */
long double* createHighPassFilter(int order, double samplingRate, double filterStartFrequency);

/**
 * Create bandpass filter
 * @param order
 * @param samplingRate
 * @param startFrequency
 * @param finalFrequency
 * @return bandpass filter
 */
long double* createStopBandFilter(int order, double samplingRate, double startFrequency, double finalFrequency);

/**
 * Create bandstop filter
 * @param order
 * @param samplingRate
 * @param startFrequency
 * @param finalFrequency
 * @return bandstop filter
 */
long double* bandStopFilter(int order, double samplingRate, double startFrequency, double finalFrequency);

/**
 * Create a window for signals
 * @param order
 * @return window
 */
long double* createTriangularWindow(int order);

/**
 * Apply window
 * @param filter
 * @param window
 * @param order
 */
void applyWindow(long double *filter, long double *window, int order);

#endif
