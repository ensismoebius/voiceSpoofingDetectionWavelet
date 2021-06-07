/**
 * @author André Furlan <ensismoebius@gmail.com>
 *
 * Based on the original code of:
 * @author Rodrigo Capobianco Guido <guido@ieee.org>
 *
 * This whole project are under GPLv3, for
 * more information read the license file
 *
 * 8 de ago de 2019
 */
#ifndef SRC_LIB_WAV_H_
#define SRC_LIB_WAV_H_

#include <cstdint>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

class Wav
{
	private:
		// signal properties
		struct
		{
				/* RIFF Chunk Descriptor */
				std::uint8_t RIFF[4];        // RIFF Header Magic header
				std::uint32_t chunkSize;      // RIFF Chunk Size
				std::uint8_t WAVE[4];        // WAVE Header

				/* "fmt" sub-chunk */
				std::uint8_t fmt[4];         // FMT header
				std::uint32_t subchunk1Size;  // Size of the fmt chunk
				std::uint16_t audioFormat;    // Audio format 1=PCM,6=mulaw,7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
				std::uint16_t numOfChan;      // Number of channels 1=Mono 2=Sterio
				std::uint32_t samplingrate;  // Sampling Frequency in Hz
				std::uint32_t bytesPerSec;    // bytes per second
				std::uint16_t blockAlign;     // 2=16-bit mono, 4=16-bit stereo
				std::uint16_t bitsPerSample;  // Number of bits per sample

				/* "data" sub-chunk */
				std::uint8_t subchunk2ID[4]; // "data"  string
				std::uint32_t subchunk2Size;  // Sampled data length
		} headers;

		// another signal properties
		int amountOfData;
		int waveResolution;

		// signal data
		std::vector<long double> data;
		std::vector<long double> dataLeft;
		std::vector<long double> dataRight;

		// path of file containing the signal
		std::string path;

		// callback function (applied on data)
		void (*callbackFunction)(std::vector<long double> &signal, int &signalLength, unsigned int samplingRate, std::string path);

	public:
		/**
		 * Process the data
		 */
		void process();

		/**
		 * Read the wav file
		 * @param path
		 */
		void read(std::string path);

		/**
		 * Write a wav file
		 * @param path
		 */
		void write(std::string path);

		/**
		 * Returns the path of file containing the signal
		 * @return path
		 */
		std::string getPath() const;

		/**
		 * Returns the raw wav data (monolitic wav)
		 * @return data
		 */
		std::vector<long double> getData() const;

		/**
		 * Returns the raw wav data (left channel wav)
		 * @return data
		 */
		std::vector<long double> getDataLeft() const;

		/**
		 * Returns the raw wav data (right channel wav)
		 * @return data
		 */
		std::vector<long double> getDataRight() const;

		/**
		 * The function witch will manipulate the wav data
		 * @param callbackFunction
		 */
		void setCallbackFunction(void (*callbackFunction)(std::vector<long double> &signal, int &signalLength, unsigned int samplingRate, std::string path));

	private:
		short convert2of8to1of16(unsigned char lsb, unsigned char msb);
		void convert1of16to2of8(short result, unsigned char *lsb, unsigned char *msb);
		void readWaveData(std::ifstream &ifs);
		void readWaveHeaders(std::ifstream &ifs);
		inline void write8Res1Channel(std::ofstream &ofs);
		inline void write8Res2Channel(std::ofstream &ofs);
		inline void write16Res1Channel(std::ofstream &ofs);
		inline void write16Res2Channel(std::ofstream &ofs);
		inline void read8Res1Channel(std::ifstream &ifs);
		inline void read8Res2Channel(std::ifstream &ifs);
		inline void read16Res1Channel(std::ifstream &ifs);
		inline void read16Res2Channel(std::ifstream &ifs);
		void clearVectors();
		void resetMetaData();
};
#endif /* SRC_LIB_WAV_H_ */
