/**
 * @author André Furlan
 * @email ensismoebius@gmail.com
 * This whole project are under GPLv3, for
 * more information read the license file
 *
 * 8 de ago de 2019
 *
 */
#ifndef SRC_WAV_C_
#define SRC_WAV_C_

#include <bits/stdint-uintn.h>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

typedef struct WAV_HEADER {
		/* RIFF Chunk Descriptor */
		uint8_t RIFF[4];        // RIFF Header Magic header
		uint32_t chunkSize;      // RIFF Chunk Size
		uint8_t WAVE[4];        // WAVE Header

		/* "fmt" sub-chunk */
		uint8_t fmt[4];         // FMT header
		uint32_t subchunk1Size;  // Size of the fmt chunk
		uint16_t audioFormat;    // Audio format 1=PCM,6=mulaw,7=alaw,     257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
		uint16_t numOfChan;      // Number of channels 1=Mono 2=Sterio
		uint32_t samplingrate;  // Sampling Frequency in Hz
		uint32_t bytesPerSec;    // bytes per second
		uint16_t blockAlign;     // 2=16-bit mono, 4=16-bit stereo
		uint16_t bitsPerSample;  // Number of bits per sample

		/* "data" sub-chunk */
		uint8_t subchunk2ID[4]; // "data"  string
		uint32_t subchunk2Size;  // Sampled data length
} wav_hdr;

//// Declaring the struct for the file header
//typedef struct {
//		unsigned char riff[4]; // what's riff??
//		unsigned int len;
//} riffHeader;
//
//typedef struct {
//		unsigned char id[4];
//		unsigned int len;
//} riffChunk;
//
//typedef struct {
//		unsigned short formattag;
//		unsigned short numberofchannels;
//		unsigned int samplingrate;
//		unsigned int avgbytespersecond;
//		unsigned short blockalign;
//} waveChunk;
//
//typedef struct {
//		unsigned char data[4];
//		unsigned int chunk_size;
//} dataChunk;

class Wav {

	private:
		std::string path;

		// header data
		wav_hdr headers;
//		unsigned char waveWord[4];
//		riffHeader headerWithRiff;
//		riffChunk headerWithChunk;
//
//		waveChunk chunkOfWave;
		int waveResolution;
//		unsigned char excess;
//
//		dataChunk dataAboutTheData;
		int amountOfData;

		// signal data
		double* data;
		double* dataLeft;
		double* dataRight;

		void (*transformationFunction)(double* signal, int signalLength, unsigned int samplingRate);

	public:
		Wav(std::string path) {
			this->path = path;

			std::ifstream ifs;

			ifs.open(path, std::ios::in | std::ios::binary);

			// Reads the file headers
			readWaveHeaders(ifs);

			// Reads actual data
			readWaveData(ifs);

			ifs.close();
		}

		void transformAndSaveWaveData(std::string path) {
			bool processed = false;

			std::ofstream ofs;
			ofs.open(path, std::ios::out | std::ios::binary);

			if (!ofs.is_open()) {
				std::cout << "Cannot open file: " << path;
				throw std::runtime_error("Impossible to open the file!");
				return;
			}

			if ((waveResolution == 8) && (this->headers.numOfChan == 1)) {
				if (transformationFunction != 0) {
					(*transformationFunction)(data, amountOfData, this->headers.samplingrate);
				}

				ofs.write((char*) &this->headers, sizeof(this->headers));

				unsigned char waveformdata;
				for (int i = 0; i < amountOfData; i++) {
					waveformdata = (unsigned char) data[i];
					ofs.write((char*) &waveformdata, sizeof(waveformdata));
				}

				processed = true;
			}
			if ((waveResolution == 8) && (this->headers.numOfChan == 2)) {
				if (transformationFunction != 0) {
					(*transformationFunction)(dataLeft, amountOfData, this->headers.samplingrate);
					(*transformationFunction)(dataRight, amountOfData, this->headers.samplingrate);
				}

				ofs.write((char*) &this->headers, sizeof(this->headers));

				unsigned char waveformdata_right, waveformdata_left;
				for (int i = 0; i < amountOfData; i++) {
					waveformdata_left = (unsigned char) dataLeft[i];
					ofs.write((char*) &waveformdata_left, sizeof(waveformdata_left));

					waveformdata_right = (unsigned char) dataRight[i];
					ofs.write((char*) &waveformdata_right, sizeof(waveformdata_right));
				}

				processed = true;
			}
			if ((waveResolution == 16) && (this->headers.numOfChan == 1)) {
				if (transformationFunction != 0) {
					(*transformationFunction)(data, amountOfData, this->headers.samplingrate);
				}

				ofs.write((char*) &this->headers, sizeof(this->headers));

				unsigned char waveformdata_lsb, waveformdata_msb;
				for (int i = 0; i < amountOfData; i++) {
					converte1de16para2de8((short) (data[i]), &waveformdata_lsb, &waveformdata_msb);
					ofs.write((char*) &waveformdata_lsb, sizeof(waveformdata_lsb));
					ofs.write((char*) &waveformdata_msb, sizeof(waveformdata_msb));
				}

				processed = true;
			}
			if ((waveResolution == 16) && (this->headers.numOfChan == 2)) {
				if (transformationFunction != 0) {
					(*transformationFunction)(dataLeft, amountOfData, this->headers.samplingrate);
					(*transformationFunction)(dataRight, amountOfData, this->headers.samplingrate);
				}

				ofs.write((char*) &this->headers, sizeof(this->headers));

				unsigned char waveformdata_lsb_left, waveformdata_lsb_right, waveformdata_msb_left, waveformdata_msb_right;
				for (int i = 0; i < amountOfData; i++) {
					converte1de16para2de8((short) dataLeft[i], &waveformdata_lsb_left, &waveformdata_msb_left);
					converte1de16para2de8((short) dataRight[i], &waveformdata_lsb_right, &waveformdata_msb_right);

					ofs.write((char*) &waveformdata_lsb_left, sizeof(waveformdata_lsb_left));
					ofs.write((char*) &waveformdata_msb_left, sizeof(waveformdata_msb_left));
					ofs.write((char*) &waveformdata_lsb_right, sizeof(waveformdata_lsb_right));
					ofs.write((char*) &waveformdata_msb_right, sizeof(waveformdata_msb_right));
				}

				processed = true;
			}

			if (!processed) {
				throw std::runtime_error("Invalid number of channels and/or resolution");
				return;
			}

			ofs.close();
		}

		double* getData() const {
			return data;
		}

		double* getDataLeft() const {
			return dataLeft;
		}

		double* getDataRight() const {
			return dataRight;
		}

		std::string getPath() const {
			return path;
		}

		void setPath(std::string path) {
			this->path = path;
		}
		void setTransformationFunction(void (*transformationFunction)(double* signal, int signalLength, unsigned int samplingRate)) {
			this->transformationFunction = transformationFunction;
		}

	private:

		short converte2de8para1de16(unsigned char lsb, unsigned char msb) {
			return (((msb & 0x80) >> 7) * (32768) + ((msb & 0x40) >> 6) * (16384) + ((msb & 0x20) >> 5) * (8192) + ((msb & 0x10) >> 4) * (4096) + ((msb & 0x08) >> 3) * (2048) + ((msb & 0x04) >> 2) * (1024) + ((msb & 0x02) >> 1) * (512) + ((msb & 0x01)) * (256) + ((lsb & 0x80) >> 7) * (128) + ((lsb & 0x40) >> 6) * (64) + ((lsb & 0x20) >> 5) * (32) + ((lsb & 0x10) >> 4) * (16) + ((lsb & 0x08) >> 3) * (8) + ((lsb & 0x04) >> 2) * (4) + ((lsb & 0x02) >> 1) * (2) + (lsb & 0x01));
		}

		void converte1de16para2de8(short resultado, unsigned char* lsb, unsigned char* msb) {
			*lsb = (((resultado & 0x0080) >> 7) * (128) + ((resultado & 0x0040) >> 6) * (64) + ((resultado & 0x0020) >> 5) * (32) + ((resultado & 0x0010) >> 4) * (16) + ((resultado & 0x0008) >> 3) * (8) + ((resultado & 0x0004) >> 2) * (4) + ((resultado & 0x0002) >> 1) * (2) + (resultado & 0x0001));
			*msb = (((resultado & 0x8000) >> 15) * (128) + ((resultado & 0x4000) >> 14) * (64) + ((resultado & 0x2000) >> 13) * (32) + ((resultado & 0x1000) >> 12) * (16) + ((resultado & 0x0800) >> 11) * (8) + ((resultado & 0x0400) >> 10) * (4) + ((resultado & 0x0200) >> 9) * (2) + ((resultado & 0x0100) >> 8));
		}

		void readWaveData(std::ifstream &ifs) {

			if ((waveResolution == 8) && (this->headers.numOfChan == 1)) {
				unsigned char waveformdata;
				data = new double[amountOfData];
				for (int i = 0; i < amountOfData; i++) {
					ifs.read((char*) &waveformdata, sizeof(waveformdata));
					data[i] = (double) (waveformdata);
				}
				return;
			}
			if ((waveResolution == 8) && (this->headers.numOfChan == 2)) {
				unsigned char waveformdata_right;
				unsigned char waveformdata_left;
				dataLeft = new double[amountOfData];
				dataRight = new double[amountOfData];
				for (int i = 0; i < amountOfData; i++) {
					ifs.read((char*) &waveformdata_left, sizeof(waveformdata_left));
					ifs.read((char*) &waveformdata_right, sizeof(waveformdata_right));
					dataLeft[i] = (double) (waveformdata_right);
					dataRight[i] = (double) (waveformdata_left);
				}
				return;
			}
			if ((waveResolution == 16) && (this->headers.numOfChan == 1)) {
				unsigned char waveformdata_lsb, waveformdata_msb;
				data = new double[amountOfData];
				for (int i = 0; i < amountOfData; i++) {
					ifs.read((char*) &waveformdata_lsb, sizeof(waveformdata_lsb));
					ifs.read((char*) &waveformdata_msb, sizeof(waveformdata_msb));
					data[i] = (double) (converte2de8para1de16(waveformdata_lsb, waveformdata_msb));
				}
				return;
			}
			if ((waveResolution == 16) && (this->headers.numOfChan == 2)) {
				unsigned char waveformdata_lsb_left, waveformdata_lsb_right, waveformdata_msb_left, waveformdata_msb_right;
				dataLeft = new double[amountOfData];
				dataRight = new double[amountOfData];
				for (int i = 0; i < amountOfData; i++) {
					ifs.read((char*) &waveformdata_lsb_left, sizeof(waveformdata_lsb_left));
					ifs.read((char*) &waveformdata_msb_left, sizeof(waveformdata_msb_left));
					ifs.read((char*) &waveformdata_lsb_right, sizeof(waveformdata_lsb_right));
					ifs.read((char*) &waveformdata_msb_right, sizeof(waveformdata_msb_right));
					dataLeft[i] = (double) (converte2de8para1de16(waveformdata_lsb_left, waveformdata_msb_left));
					dataRight[i] = (double) (converte2de8para1de16(waveformdata_lsb_right, waveformdata_msb_right));
				}
				return;
			}
			std::cout << "Invalid number of channels and/or resolution";
		}

		void readWaveHeaders(std::ifstream &ifs) {

			ifs.seekg(0, std::ios::beg);
			ifs.read((char*) &this->headers, sizeof(this->headers));

			if (this->headers.audioFormat != 1) {
				throw std::runtime_error("Not in PCM format!");
				return;
			}

			waveResolution = (this->headers.bytesPerSec * 8) / (this->headers.numOfChan * this->headers.samplingrate);
			amountOfData = this->headers.chunkSize / this->headers.blockAlign;
		}
};
#endif /* SRC_WAV_C_ */
