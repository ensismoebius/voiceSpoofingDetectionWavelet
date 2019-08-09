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

#include <bits/types/FILE.h>
#include <iostream>
#include <stdexcept>

// Declaring the struct for the file header
typedef struct {
		unsigned char riff[4]; // what's riff??
		unsigned int len;
} riffHeader;

typedef struct {
		unsigned char id[4];
		unsigned int len;
} riffChunk;

typedef struct {
		unsigned short formattag;
		unsigned short numberofchannels;
		unsigned int samplingrate;
		unsigned int avgbytespersecond;
		unsigned short blockalign;
} waveChunk;

typedef struct {
		unsigned char data[4];
		unsigned int chunk_size;
} dataChunk;

class Wav {

	private:
		char* path;

		// header data
		unsigned char waveWord[4];
		riffHeader headerWithRiff;
		riffChunk headerWithChunk;

		waveChunk chunkOfWave;
		int waveResolution;
		unsigned char excess;

		dataChunk dataAboutTheData;
		int amountOfData;

		// signal data
		double* data;
		double* dataLeft;
		double* dataRight;

		void (*transformationFunction)(double* signal, int signalLength, unsigned int samplingRate);

	public:
		Wav(char path[255]) {
			this->path = path;

			// File reader
			FILE* fileReader;

			// Opening the file
			if (((fileReader = fopen(path, "rb")) == NULL)) {
				std::cout << "No such file: " << path;

				// Closing file
				fclose(fileReader);

				delete fileReader;

				throw std::runtime_error("No such file!");
				return;
			}

			// Reads the file headers
			readWaveHeaders(fileReader);

			// Reads actual data
			readWaveData(fileReader);

			// Closing file
			fclose(fileReader);
			delete fileReader;
		}

		void transformAndSaveWaveData(const char path[255]) {

			bool processed = false;

			// File reader
			FILE* fileWriter;

			// Opening the file
			if (((fileWriter = fopen(path, "wb")) == NULL)) {
				std::cout << "Impossible to open " << path;
				throw std::runtime_error("Impossible to open the file!");
				return;
			}

			if ((waveResolution == 8) && (chunkOfWave.numberofchannels == 1)) {
				if (transformationFunction != 0) {
					(*transformationFunction)(data, amountOfData, chunkOfWave.samplingrate);
				}

				unsigned char waveformdata;

				for (int i = 0; i < amountOfData; i++) {
					waveformdata = (unsigned char) data[i];
					fwrite(&waveformdata, sizeof(waveformdata), 1, fileWriter);
				}

				processed = true;
			}
			if ((waveResolution == 8) && (chunkOfWave.numberofchannels == 2)) {
				if (transformationFunction != 0) {
					(*transformationFunction)(dataLeft, amountOfData, chunkOfWave.samplingrate);
					(*transformationFunction)(dataRight, amountOfData, chunkOfWave.samplingrate);
				}

				unsigned char waveformdata_right;
				unsigned char waveformdata_left;

				for (int i = 0; i < amountOfData; i++) {
					waveformdata_left = (unsigned char) dataLeft[i];
					fwrite(&waveformdata_left, sizeof(waveformdata_left), 1, fileWriter);
					waveformdata_right = (unsigned char) dataRight[i];
					fwrite(&waveformdata_right, sizeof(waveformdata_right), 1, fileWriter);
				}

				processed = true;
			}
			if ((waveResolution == 16) && (chunkOfWave.numberofchannels == 1)) {
				if (transformationFunction != 0) {
					(*transformationFunction)(data, amountOfData, chunkOfWave.samplingrate);
				}

				unsigned char waveformdata_lsb, waveformdata_msb;

				for (int i = 0; i < amountOfData; i++) {
					converte1de16para2de8((short) (data[i]), &waveformdata_lsb, &waveformdata_msb);
					fwrite(&waveformdata_lsb, sizeof(waveformdata_lsb), 1, fileWriter);
					fwrite(&waveformdata_msb, sizeof(waveformdata_msb), 1, fileWriter);
				}

				processed = true;
			}
			if ((waveResolution == 16) && (chunkOfWave.numberofchannels == 2)) {
				if (transformationFunction != 0) {
					(*transformationFunction)(dataLeft, amountOfData, chunkOfWave.samplingrate);
					(*transformationFunction)(dataRight, amountOfData, chunkOfWave.samplingrate);
				}

				unsigned char waveformdata_lsb_left, waveformdata_lsb_right, waveformdata_msb_left, waveformdata_msb_right;

				for (int i = 0; i < amountOfData; i++) {
					converte1de16para2de8((short) dataLeft[i], &waveformdata_lsb_left, &waveformdata_msb_left);
					converte1de16para2de8((short) dataRight[i], &waveformdata_lsb_right, &waveformdata_msb_right);
					fwrite(&waveformdata_lsb_left, sizeof(waveformdata_lsb_left), 1, fileWriter);
					fwrite(&waveformdata_msb_left, sizeof(waveformdata_msb_left), 1, fileWriter);
					fwrite(&waveformdata_lsb_right, sizeof(waveformdata_lsb_right), 1, fileWriter);
					fwrite(&waveformdata_msb_right, sizeof(waveformdata_msb_right), 1, fileWriter);
				}

				processed = true;
			}

			if (!processed) {
				throw std::runtime_error("Invalid number of channels and/or resolution");
				return;
			}

			// Closing file
			fclose(fileWriter);
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

		char* getPath() const {
			return path;
		}

		void setPath(char* path) {
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

		void readWaveData(FILE* fileReader) {

			if ((waveResolution == 8) && (chunkOfWave.numberofchannels == 1)) {
				unsigned char waveformdata;
				data = new double[amountOfData];
				for (int i = 0; i < amountOfData; i++) {
					fread(&waveformdata, sizeof(waveformdata), 1, fileReader);
					data[i] = (double) (waveformdata);
				}
				if (transformationFunction != 0) {
					(*transformationFunction)(data, amountOfData, chunkOfWave.samplingrate);
				}
				return;
			}
			if ((waveResolution == 8) && (chunkOfWave.numberofchannels == 2)) {
				unsigned char waveformdata_right;
				unsigned char waveformdata_left;
				dataLeft = new double[amountOfData];
				dataRight = new double[amountOfData];
				for (int i = 0; i < amountOfData; i++) {
					fread(&waveformdata_left, sizeof(waveformdata_left), 1, fileReader);
					fread(&waveformdata_right, sizeof(waveformdata_right), 1, fileReader);
					dataLeft[i] = (double) (waveformdata_right);
					dataRight[i] = (double) (waveformdata_left);
				}
				return;
			}
			if ((waveResolution == 16) && (chunkOfWave.numberofchannels == 1)) {
				unsigned char waveformdata_lsb, waveformdata_msb;
				data = new double[amountOfData];
				for (int i = 0; i < amountOfData; i++) {
					fread(&waveformdata_lsb, sizeof(waveformdata_lsb), 1, fileReader);
					fread(&waveformdata_msb, sizeof(waveformdata_msb), 1, fileReader);
					data[i] = (double) (converte2de8para1de16(waveformdata_lsb, waveformdata_msb));

					if (i % 10000000 == 0) {
						std::cout << (double) i / (double) amountOfData << "%\n";
					}
				}
				return;
			}
			if ((waveResolution == 16) && (chunkOfWave.numberofchannels == 2)) {
				unsigned char waveformdata_lsb_left, waveformdata_lsb_right, waveformdata_msb_left, waveformdata_msb_right;
				dataLeft = new double[amountOfData];
				dataRight = new double[amountOfData];
				for (int i = 0; i < amountOfData; i++) {
					fread(&waveformdata_lsb_left, sizeof(waveformdata_lsb_left), 1, fileReader);
					fread(&waveformdata_msb_left, sizeof(waveformdata_msb_left), 1, fileReader);
					fread(&waveformdata_lsb_right, sizeof(waveformdata_lsb_right), 1, fileReader);
					fread(&waveformdata_msb_right, sizeof(waveformdata_msb_right), 1, fileReader);
					dataLeft[i] = (double) (converte2de8para1de16(waveformdata_lsb_left, waveformdata_msb_left));
					dataRight[i] = (double) (converte2de8para1de16(waveformdata_lsb_right, waveformdata_msb_right));
				}
				return;
			}
			std::cout << "Invalid number of channels and/or resolution";
		}

		void readWaveHeaders(FILE* fileReader) {

			//reads the original file header
			fread(&headerWithRiff, sizeof(riffHeader), 1, fileReader);
			fread(&waveWord, sizeof(waveWord), 1, fileReader);
			fread(&headerWithChunk, sizeof(riffChunk), 1, fileReader);
			fread(&chunkOfWave, sizeof(waveChunk), 1, fileReader);
			if (headerWithRiff.len > 16) {
				for (unsigned int i = 0; i < headerWithRiff.len - 16; i++) {
					fread(&excess, sizeof(excess), 1, fileReader);
				}
			}
			// The format MUST be PCM!!
			if (chunkOfWave.formattag != 1) {
				throw std::runtime_error("Not in PCM format!");
				return;
			}
			// Why not bit sample?
			waveResolution = (chunkOfWave.avgbytespersecond * 8) / (chunkOfWave.numberofchannels * chunkOfWave.samplingrate);

			//TODO delete!!!!
			struct {
					unsigned char data[4];
					unsigned int chunk_size;
			} header_data_chunk;
			fread(&header_data_chunk, sizeof(header_data_chunk), 1, fileReader);
			//TODO delete!!!

			fread(&dataAboutTheData, sizeof(dataAboutTheData), 1, fileReader);
			amountOfData = dataAboutTheData.chunk_size / chunkOfWave.blockalign;
		}
};
#endif /* SRC_WAV_C_ */
