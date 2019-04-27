/*
 * WavProcessor.cpp
 *
 *  Created on: 26 de abr de 2019
 *      Author: ensis
 */
#ifndef WavProcessor_class_
#define WavProcessor_class_

#include <bits/types/FILE.h>
#include <cstdio>
#include <cstring>
#include <iostream>

#include "wavStructures.h"
#include "Wav.cpp"

/**
 * Process, read and write wav files
 */
class WavProcessor {

	private:

		// filtering block
		void (*modifica_dados_brutos)(double* signal, int comprimento_do_sinal, unsigned int taxa_de_amostragem);

		// auxiliary functions block
		short converte2de8para1de16(unsigned char lsb, unsigned char msb) {
			return (((msb & 0x80) >> 7) * (32768) + ((msb & 0x40) >> 6) * (16384) + ((msb & 0x20) >> 5) * (8192) + ((msb & 0x10) >> 4) * (4096) + ((msb & 0x08) >> 3) * (2048) + ((msb & 0x04) >> 2) * (1024) + ((msb & 0x02) >> 1) * (512) + ((msb & 0x01)) * (256) + ((lsb & 0x80) >> 7) * (128) + ((lsb & 0x40) >> 6) * (64) + ((lsb & 0x20) >> 5) * (32) + ((lsb & 0x10) >> 4) * (16) + ((lsb & 0x08) >> 3) * (8) + ((lsb & 0x04) >> 2) * (4) + ((lsb & 0x02) >> 1) * (2) + (lsb & 0x01));
		}

		void converte1de16para2de8(short resultado, unsigned char* lsb, unsigned char* msb) {
			*lsb = (((resultado & 0x0080) >> 7) * (128) + ((resultado & 0x0040) >> 6) * (64) + ((resultado & 0x0020) >> 5) * (32) + ((resultado & 0x0010) >> 4) * (16) + ((resultado & 0x0008) >> 3) * (8) + ((resultado & 0x0004) >> 2) * (4) + ((resultado & 0x0002) >> 1) * (2) + (resultado & 0x0001));
			*msb = (((resultado & 0x8000) >> 15) * (128) + ((resultado & 0x4000) >> 14) * (64) + ((resultado & 0x2000) >> 13) * (32) + ((resultado & 0x1000) >> 12) * (16) + ((resultado & 0x0800) >> 11) * (8) + ((resultado & 0x0400) >> 10) * (4) + ((resultado & 0x0200) >> 9) * (2) + ((resultado & 0x0100) >> 8));
		}

		void save1Channel8bitsWav(Wav* w, unsigned char applyFilter, FILE* fileWriter) {
			unsigned char waveformdata;
			int tamanho_da_janela = w->dataChunk->chunk_size / w->wavChunk->blockalign;

			if (applyFilter) {
				this->modifica_dados_brutos(w->amostrasNoTempo, tamanho_da_janela, w->wavChunk->samplingrate);
			}
			for (int i = 0; i < tamanho_da_janela; i++) {
				waveformdata = (unsigned char) w->amostrasNoTempo[i];
				fwrite(&waveformdata, sizeof(waveformdata), 1, fileWriter);
			}
		}

		void save2Channels8bitsWav(Wav* w, unsigned char applyFilter, FILE* fileWriter) {
			unsigned char waveformdata_right;
			unsigned char waveformdata_left;
			int tamanho_da_janela = w->dataChunk->chunk_size / w->wavChunk->blockalign;
			if (applyFilter) {
				this->modifica_dados_brutos(w->amostrasNoTempoLeft, tamanho_da_janela, w->wavChunk->samplingrate);
				this->modifica_dados_brutos(w->amostrasNoTempoRight, tamanho_da_janela, w->wavChunk->samplingrate);
			}
			for (int i = 0; i < tamanho_da_janela; i++) {
				waveformdata_left = (unsigned char) (w->amostrasNoTempoLeft[i]);
				fwrite(&waveformdata_left, sizeof(waveformdata_left), 1, fileWriter);
				waveformdata_right = (unsigned char) (w->amostrasNoTempoRight[i]);
				fwrite(&waveformdata_right, sizeof(waveformdata_right), 1, fileWriter);
			}
		}

		void save1Channel16bitsWav(Wav* w, unsigned char applyFilter, FILE* fileWriter) {

			int tamanho_da_janela = w->dataChunk->chunk_size / w->wavChunk->blockalign;

			if (applyFilter) {
				this->modifica_dados_brutos(w->amostrasNoTempo, tamanho_da_janela, w->wavChunk->samplingrate);
			}
			for (int i = 0; i < tamanho_da_janela; i++) {

				this->converte1de16para2de8((short) w->amostrasNoTempo[i], &w->waveformDataLsb, &w->waveformDataMsb);

				fwrite(&w->waveformDataLsb, sizeof(w->waveformDataLsb), 1, fileWriter);
				fwrite(&w->waveformDataMsb, sizeof(w->waveformDataMsb), 1, fileWriter);
			}
		}

		void save2Channels16bitsWav(Wav* w, unsigned char applyFilter, FILE* fileWriter) {

			int tamanho_da_janela = w->dataChunk->chunk_size / w->wavChunk->blockalign;

			if (applyFilter) {
				this->modifica_dados_brutos(w->amostrasNoTempoLeft, tamanho_da_janela, w->wavChunk->samplingrate);
				this->modifica_dados_brutos(w->amostrasNoTempoRight, tamanho_da_janela, w->wavChunk->samplingrate);
			}
			for (int i = 0; i < tamanho_da_janela; i++) {
				this->converte1de16para2de8((short) w->amostrasNoTempoLeft[i], &w->waveformDataLsbLeft, &w->waveformDataMsbLeft);
				this->converte1de16para2de8((short) w->amostrasNoTempoRight[i], &w->waveformDataLsbRight, &w->waveformDataMsbRight);
				fwrite(&w->waveformDataLsbLeft, sizeof(w->waveformDataLsbLeft), 1, fileWriter);
				fwrite(&w->waveformDataMsbLeft, sizeof(w->waveformDataMsbLeft), 1, fileWriter);
				fwrite(&w->waveformDataLsbRight, sizeof(w->waveformDataLsbRight), 1, fileWriter);
				fwrite(&w->waveformDataMsbRight, sizeof(w->waveformDataMsbRight), 1, fileWriter);
			}
		}

		void saveHeaders(Wav* w, FILE* fileWriter) {
			// Writes the file header
			fwrite(w->wavRiff, sizeof(headerWavRiff), 1, fileWriter);

			fwrite(&w->wave, sizeof(char[4]), 1, fileWriter);
			fwrite(w->wavRiffChunk, sizeof(headerWavRiffChunk), 1, fileWriter);
			fwrite(w->wavChunk, sizeof(headerWavChunk), 1, fileWriter);
			//???
			if (w->wavRiffChunk->len > 16) {
				for (int i = 0; w->wavRiffChunk->len - 16; i++) {
					fwrite(&w->excesso, sizeof(w->excesso), 1, fileWriter);
				}
			}
			fwrite(w->dataChunk, sizeof(headerDataChunk), 1, fileWriter);
		}

	public:
		Wav* read(char* filepath) {

			//file reader
			FILE* fileReader;

			// Opening the file to read
			if ((fileReader = fopen(filepath, "rb")) == NULL) return NULL;

			unsigned char excesso;

			// headers

			char wave[4];
			headerWavRiff hwr;
			headerWavChunk hwc;
			headerDataChunk hwdc;
			headerWavRiffChunk hwrc;

			// the order is important!!!!
			fread(&hwr, sizeof(headerWavRiff), 1, fileReader);
			fread(&wave, sizeof(wave), 1, fileReader);
			fread(&hwrc, sizeof(headerWavRiffChunk), 1, fileReader);
			fread(&hwc, sizeof(headerWavChunk), 1, fileReader);

			if (hwrc.len > 16) {
				for (unsigned int i = 0; i < hwrc.len - 16; i++) {
					fread(&excesso, sizeof(excesso), 1, fileReader);
				}
			}

			// The format MUST be PCM!!
			if (hwc.formattag != 1) {
				std::cout << "FORA DO FORMATO PCM...";
				return 0;
			}

			fread(&hwdc, sizeof(headerDataChunk), 1, fileReader);

			int resolucao = (hwc.avgbytespersecond * 8) / (hwc.numberofchannels * hwc.samplingrate);
			int tamanho_da_janela = hwdc.chunk_size / hwc.blockalign;

			// If initial conditions are OK create the instance
			Wav* w = new Wav(tamanho_da_janela);
			w->dataChunk = &hwdc;
			w->wavChunk = &hwc;
			w->wavRiff = &hwr;
			w->wavRiffChunk = &hwrc;
			strcpy(w->wave, wave);

			if (hwc.numberofchannels == 1) {
				unsigned char waveformdata;

				for (int i = 0; i < tamanho_da_janela; i++) {
					fread(&waveformdata, sizeof(waveformdata), 1, fileReader);
					w->amostrasNoTempo[i] = (double) waveformdata;
				}

			} else if (hwc.numberofchannels == 2) {

				unsigned char waveformdata_right;
				unsigned char waveformdata_left;
				for (int i = 0; i < tamanho_da_janela; i++) {
					fread(&waveformdata_left, sizeof(waveformdata_left), 1, fileReader);
					fread(&waveformdata_right, sizeof(waveformdata_right), 1, fileReader);

					w->amostrasNoTempoLeft[i] = (double) waveformdata_right;
					w->amostrasNoTempoLeft[i] = (double) waveformdata_left;
				}

			} else if (hwc.numberofchannels == 1) {

				for (int i = 0; i < tamanho_da_janela; i++) {
					fread(&w->waveformDataLsb, sizeof(w->waveformDataLsb), 1, fileReader);
					fread(&w->waveformDataMsb, sizeof(w->waveformDataMsb), 1, fileReader);
					w->amostrasNoTempo[i] = (double) converte2de8para1de16(w->waveformDataLsb, w->waveformDataMsb);
				}
			} else if (hwc.numberofchannels == 2) {

				for (int i = 0; i < tamanho_da_janela; i++) {
					fread(&w->waveformDataLsbLeft, sizeof(w->waveformDataLsbLeft), 1, fileReader);
					fread(&w->waveformDataMsbLeft, sizeof(w->waveformDataMsbLeft), 1, fileReader);
					fread(&w->waveformDataLsbRight, sizeof(w->waveformDataLsbRight), 1, fileReader);
					fread(&w->waveformDataMsbRight, sizeof(w->waveformDataMsbRight), 1, fileReader);
					w->amostrasNoTempoLeft[i] = (double) converte2de8para1de16(w->waveformDataLsbLeft, w->waveformDataMsbLeft);
					w->amostrasNoTempoRight[i] = (double) converte2de8para1de16(w->waveformDataLsbRight, w->waveformDataMsbRight);
				}

			} else {
				std::cout << "Resolucao ou numero de canais invalido(s)";
			}

			fclose(fileReader);

			// display header's infos
			std::cout << "\nArquivo do tipo: " << hwr.riff[3];
			std::cout << "\nTamanho excluindo header: " << hwr.len;
			std::cout << "\nSub-Tipo: " << wave[0] << wave[1] << wave[2] << wave[3];
			std::cout << "\nIdentificador: " << hwrc.id[3];
			std::cout << "\nComprimento do chunk apos header: " << hwrc.len;
			std::cout << "\nCategoria do formato: " << hwc.formattag;
			std::cout << "\nTaxa de amostragem: " << hwc.samplingrate;
			std::cout << "\nNumero de canais: " << hwc.numberofchannels;
			std::cout << "\nMedia do num. de bps: " << hwc.avgbytespersecond;
			std::cout << "\nAlinhamento do bloco em bytes: " << hwc.blockalign;
			// display data info
			std::cout << "\nResolucao: " << resolucao;
			std::cout << "\nIdentificacao: " << hwdc.data[0] << hwdc.data[1] << hwdc.data[2] << hwdc.data[3];
			std::cout << "\nTamanho do chunk de dados: " << hwdc.chunk_size;
			std::cout << "\nNumero de frames para amostrar: " << hwc.blockalign;
			std::cout << "\nTamanho da janela: " << tamanho_da_janela;

			return w;
		}

		char save(Wav* w, unsigned char applyFilter = 0) {

			if (strlen(w->filePath) == 0) return 0;

			// file writer
			FILE* fileWriter;

			// Open the file
			if ((fileWriter = fopen(w->filePath, "wb")) == NULL) return 0;

			// Writes the file header
			this->saveHeaders(w, fileWriter);

			if (w->wavChunk->numberofchannels == 1) {
				this->save1Channel8bitsWav(w, applyFilter, fileWriter);
			} else if (w->wavChunk->numberofchannels == 2) {
				this->save2Channels8bitsWav(w, applyFilter, fileWriter);
			} else if (w->wavChunk->numberofchannels == 1) {
				this->save1Channel16bitsWav(w, applyFilter, fileWriter);
			} else if (w->wavChunk->numberofchannels == 2) {
				this->save2Channels16bitsWav(w, applyFilter, fileWriter);
			} else std::cout << "Resolucao ou numero de canais invalido(s)";

			putc(w->c, fileWriter);
			fclose(fileWriter);

			return 1;
		}

};
#endif /* WavProcessor_class_ */

