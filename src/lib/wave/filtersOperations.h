/**
 * @author André Furlan
 * @email ensismoebius@gmail.com
 * This whole project are under GPLv3, for
 * more information read the license file
 *
 * 2 de abr de 2020
 *
 */

double createAlpha(double samplingRate, double filterMaxFrequency, bool highPass = false);

double* createLowPassFilter(int order, double samplingRate, double filterMaxFrequency);

double* createHighPassFilter(int order, double samplingRate, double filterStartFrequency);

double* createBandPassFilter(int order, double samplingRate, double startFrequency, double finalFrequency);

double* bandStopFilter(int order, double samplingRate, double startFrequency, double finalFrequency);

double* createTriangularWindow(int order);

void applyWindow(double *filter, double *window, int order);
