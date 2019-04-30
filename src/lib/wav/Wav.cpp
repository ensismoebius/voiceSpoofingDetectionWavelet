/*
 * Wav.cpp
 *
 *  Created on: 26 de abr de 2019
 *      Author: ensis
 */
#ifndef Wav_class_
#define Wav_class_

#include "wavStructures.h"

/**
 * Contains the wav file data
 */
class Wav {
	private:
	public:
		/**
		 * Wav file path
		 */
		char filePath[255];

		/**
		 * The "wave" string
		 */
		char wave[4];

		/**
		 * ????
		 */
		unsigned int c;

		/**
		 * ?????
		 */
		unsigned char excesso;

		// wav mono block

		/**
		 * For wav mono
		 */
		double* amostrasNoTempo;

		/**
		 * lsb for wav mono
		 */
		unsigned char waveformDataLsb;

		/**
		 * msb for wav mono
		 */
		unsigned char waveformDataMsb;

		// wav stereo block

		/**
		 * For wav stereo
		 */
		double* amostrasNoTempoLeft;

		/**
		 * For wav stereo
		 */
		double* amostrasNoTempoRight;

		/**
		 * left lsb for wav stereo
		 */
		unsigned char waveformDataLsbLeft;

		/**
		 * right lsb for wav stereo
		 */
		unsigned char waveformDataLsbRight;

		/**
		 * left msb for wav stereo
		 */
		unsigned char waveformDataMsbLeft;

		/**
		 * right msb for wav stereo
		 */
		unsigned char waveformDataMsbRight;

		// header structs block
		headerWavRiff* wavRiff;
		headerWavChunk* wavChunk;
		headerDataChunk* dataChunk;
		headerWavRiffChunk* wavRiffChunk;

		Wav(unsigned int qtdeAmostrasNoTempo) {

			this->c = 0;
			this->excesso = '0';
			this->waveformDataLsb = '0';
			this->waveformDataMsb = '0';

			this->waveformDataLsbLeft = '0';
			this->waveformDataLsbRight = '0';

			this->waveformDataMsbLeft = '0';
			this->waveformDataMsbRight = '0';

			this->amostrasNoTempo = new double[qtdeAmostrasNoTempo];
			this->amostrasNoTempoLeft = new double[qtdeAmostrasNoTempo];
			this->amostrasNoTempoRight = new double[qtdeAmostrasNoTempo];

			this->wavRiff = new headerWavRiff;
			this->wavChunk = new headerWavChunk;
			this->dataChunk = new headerDataChunk;
			this->wavRiffChunk = new headerWavRiffChunk;
		}
};
#endif /* Wav_class_ */

