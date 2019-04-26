/*
 * Wav.cpp
 *
 *  Created on: 26 de abr de 2019
 *      Author: ensis
 */
#ifndef Wav_class_
#define Wav_class_

#include <cstdio>

#include "wavStructures.h"

/**
 * Contains the wav file data
 */
class Wav {
	private:
		/**
		 * Wav file path
		 */
		char* filePath;

		/**
		 * The "wave" string
		 */
		unsigned char* wave;

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
		unsigned char* waveformDataLsb;

		/**
		 * msb for wav mono
		 */
		unsigned char* waveformDataMsb;

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
		unsigned char* waveformDataLsbLeft;

		/**
		 * right lsb for wav stereo
		 */
		unsigned char* waveformDataLsbRight;

		/**
		 * left msb for wav stereo
		 */
		unsigned char* waveformDataMsbLeft;

		/**
		 * right msb for wav stereo
		 */
		unsigned char* waveformDataMsbRight;

		// header structs block

		headerWavRiff* wavRiff;
		headerWavChunk* wavChunk;
		headerDataChunk* dataChunk;
		headerWavRiffChunk* wavRiffChunk;

	public:
		Wav() {
			// Adding an string terminator for cout
			this->filePath = NULL;
			this->wave = NULL;

			this->c = 0;
			this->excesso = '\0';
			this->amostrasNoTempo = NULL;
			this->amostrasNoTempoLeft = NULL;
			this->amostrasNoTempoRight = NULL;

			this->waveformDataLsb = NULL;
			this->waveformDataMsb = NULL;

			this->waveformDataLsbLeft = NULL;
			this->waveformDataLsbRight = NULL;
			this->waveformDataMsbLeft = NULL;
			this->waveformDataMsbRight = NULL;

			this->wavRiff = NULL;
			this->wavChunk = NULL;
			this->dataChunk = NULL;
			this->wavRiffChunk = NULL;
		}

		double* getAmostrasNoTempo() {
			return amostrasNoTempo;
		}

		void setAmostrasNoTempo(double* amostrasNoTempo) {
			this->amostrasNoTempo = amostrasNoTempo;
		}

		double* getAmostrasNoTempoLeft() {
			return amostrasNoTempoLeft;
		}

		void setAmostrasNoTempoLeft(double* amostrasNoTempoLeft) {
			this->amostrasNoTempoLeft = amostrasNoTempoLeft;
		}

		double* getAmostrasNoTempoRight() {
			return amostrasNoTempoRight;
		}

		void setAmostrasNoTempoRight(double* amostrasNoTempoRight) {
			this->amostrasNoTempoRight = amostrasNoTempoRight;
		}

		unsigned int getC() {
			return c;
		}

		void setC(unsigned int c) {
			this->c = c;
		}

		unsigned char getExcesso() {
			return excesso;
		}

		void setExcesso(unsigned char excesso) {
			this->excesso = excesso;
		}

		char* getFilePath() {
			return filePath;
		}

		void setFilePath(char* filePath) {
			this->filePath = filePath;
		}

		unsigned char* getWaveformDataLsb() {
			return waveformDataLsb;
		}

		void setWaveformDataLsb(unsigned char* waveformDataLsb) {
			this->waveformDataLsb = waveformDataLsb;
		}

		unsigned char* getWaveformDataLsbLeft() {
			return waveformDataLsbLeft;
		}

		void setWaveformDataLsbLeft(unsigned char* waveformDataLsbLeft) {
			this->waveformDataLsbLeft = waveformDataLsbLeft;
		}

		unsigned char* getWaveformDataLsbRight() {
			return waveformDataLsbRight;
		}

		void setWaveformDataLsbRight(unsigned char* waveformDataLsbRight) {
			this->waveformDataLsbRight = waveformDataLsbRight;
		}

		unsigned char* getWaveformDataMsb() {
			return waveformDataMsb;
		}

		void setWaveformDataMsb(unsigned char* waveformDataMsb) {
			this->waveformDataMsb = waveformDataMsb;
		}

		unsigned char* getWaveformDataMsbLeft() {
			return waveformDataMsbLeft;
		}

		void setWaveformDataMsbLeft(unsigned char* waveformDataMsbLeft) {
			this->waveformDataMsbLeft = waveformDataMsbLeft;
		}

		unsigned char* getWaveformDataMsbRight() {
			return waveformDataMsbRight;
		}

		void setWaveformDataMsbRight(unsigned char* waveformDataMsbRight) {
			this->waveformDataMsbRight = waveformDataMsbRight;
		}

		headerDataChunk* getDataChunk() {
			return dataChunk;
		}

		void setDataChunk(headerDataChunk* dataChunk) {
			this->dataChunk = dataChunk;
		}

		const headerWavChunk* getWavChunk() {
			return wavChunk;
		}

		void setWavChunk(headerWavChunk* wavChunk) {
			this->wavChunk = wavChunk;
		}

		headerWavRiff* getWavRiff() {
			return wavRiff;
		}

		void setWavRiff(headerWavRiff* wavRiff) {
			this->wavRiff = wavRiff;
		}

		headerWavRiffChunk* getWavRiffChunk() {
			return wavRiffChunk;
		}

		void setWavRiffChunk(headerWavRiffChunk* wavRiffChunk) {
			this->wavRiffChunk = wavRiffChunk;
		}

		unsigned char* getWave() const {
			return wave;
		}

		void setWave(unsigned char* wave) {
			this->wave = wave;
		}
};
#endif /* Wav_class_ */

