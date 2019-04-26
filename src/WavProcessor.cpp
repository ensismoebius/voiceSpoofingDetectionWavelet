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

		void save1Channel8bitsWav(Wav w, unsigned char applyFilter, int tamanho_da_janela, FILE* fileWriter) {
			unsigned char waveformdata;
			if (applyFilter) {
				this->modifica_dados_brutos(w.getAmostrasNoTempo(), tamanho_da_janela, w.getWavChunk()->samplingrate);
			}
			for (int i = 0; i < tamanho_da_janela; i++) {
				waveformdata = (unsigned char) (w.getAmostrasNoTempo()[i]);
				fwrite(&waveformdata, sizeof(waveformdata), 1, fileWriter);
			}
		}

		void save2Channels8bitsWav(Wav w, unsigned char applyFilter, int tamanho_da_janela, FILE* fileWriter) {
			unsigned char waveformdata_right;
			unsigned char waveformdata_left;
			if (applyFilter) {
				this->modifica_dados_brutos(w.getAmostrasNoTempoLeft(), tamanho_da_janela, w.getWavChunk()->samplingrate);
				this->modifica_dados_brutos(w.getAmostrasNoTempoRight(), tamanho_da_janela, w.getWavChunk()->samplingrate);
			}
			for (int i = 0; i < tamanho_da_janela; i++) {
				waveformdata_left = (unsigned char) (w.getAmostrasNoTempoLeft()[i]);
				fwrite(&waveformdata_left, sizeof(waveformdata_left), 1, fileWriter);
				waveformdata_right = (unsigned char) (w.getAmostrasNoTempoRight()[i]);
				fwrite(&waveformdata_right, sizeof(waveformdata_right), 1, fileWriter);
			}
		}

		void save1Channel16bitsWav(Wav w, unsigned char applyFilter, int tamanho_da_janela, FILE* fileWriter) {
			if (applyFilter) {
				this->modifica_dados_brutos(w.getAmostrasNoTempo(), tamanho_da_janela, w.getWavChunk()->samplingrate);
			}
			for (int i = 0; i < tamanho_da_janela; i++) {
				this->converte1de16para2de8((short) ((w.getAmostrasNoTempo()[i])), w.getWaveformDataLsb(), w.getWaveformDataMsb());
				fwrite(w.getWaveformDataLsb(), sizeof(w.getWaveformDataLsb()), 1, fileWriter);
				fwrite(w.getWaveformDataMsb(), sizeof(w.getWaveformDataMsb()), 1, fileWriter);
			}
		}

		void save2Channels16bitsWav(Wav w, unsigned char applyFilter, int tamanho_da_janela, FILE* fileWriter) {
			if (applyFilter) {
				this->modifica_dados_brutos(w.getAmostrasNoTempoLeft(), tamanho_da_janela, w.getWavChunk()->samplingrate);
				this->modifica_dados_brutos(w.getAmostrasNoTempoRight(), tamanho_da_janela, w.getWavChunk()->samplingrate);
			}
			for (int i = 0; i < tamanho_da_janela; i++) {
				this->converte1de16para2de8((short) (w.getAmostrasNoTempoLeft()[i]), w.getWaveformDataLsbLeft(), w.getWaveformDataMsbLeft());
				this->converte1de16para2de8((short) (w.getAmostrasNoTempoRight()[i]), w.getWaveformDataLsbRight(), w.getWaveformDataMsbRight());
				fwrite(w.getWaveformDataLsbLeft(), sizeof(w.getWaveformDataLsbLeft()), 1, fileWriter);
				fwrite(w.getWaveformDataMsbLeft(), sizeof(w.getWaveformDataMsbLeft()), 1, fileWriter);
				fwrite(w.getWaveformDataLsbRight(), sizeof(w.getWaveformDataLsbRight()), 1, fileWriter);
				fwrite(w.getWaveformDataMsbRight(), sizeof(w.getWaveformDataMsbRight()), 1, fileWriter);
			}
		}

		void saveHeaders(Wav w, FILE* fileWriter) {
			// Writes the file header
			fwrite(w.getWavRiff(), sizeof(w.getWavRiff()), 1, fileWriter);
			// Writes the "wave" string ( may be another useless thing form microsoft...)
			fwrite(w.getWave(), sizeof(w.getWave()), 1, fileWriter);
			fwrite(w.getWavRiffChunk(), sizeof(w.getWavRiffChunk()), 1, fileWriter);
			fwrite(w.getWavChunk(), sizeof(w.getWavChunk()), 1, fileWriter);
			//???
			if (w.getWavRiffChunk()->len > 16) {
				for (unsigned int i = 0; i < w.getWavRiffChunk()->len - 16; i++) {
					unsigned char excesso;
					fwrite(&excesso, sizeof(excesso), 1, fileWriter);
					w.setExcesso(excesso);
				}
			}
			fwrite(w.getDataChunk(), sizeof(w.getDataChunk()), 1, fileWriter);
		}

	public:
		char read(Wav w) {

			//file reader
			FILE* fileReader;

			// Opening the file to read
			if ((fileReader = fopen(w.getFilePath(), "rb")) == NULL) {
				return 0;
			}

			//reads the original file header
			headerWavRiff hwr;
			fread(&hwr, sizeof(headerWavRiff), 1, fileReader);
			w.setWavRiff(&hwr);

			unsigned char wave[4];
			fread(&wave, sizeof(wave), 1, fileReader);
			w.setWave(wave);

			headerWavRiffChunk hwrc;
			fread(&hwrc, sizeof(hwrc), 1, fileReader);
			w.setWavRiffChunk(&hwrc);

			headerWavChunk hwc;
			fread(&hwc, sizeof(hwc), 1, fileReader);
			w.setWavChunk(&hwc);

			if (w.getWavRiffChunk()->len > 16) {
				for (unsigned int i = 0; i < w.getWavRiffChunk()->len - 16; i++) {
					unsigned char excesso;
					fread(&excesso, sizeof(excesso), 1, fileReader);
					w.setExcesso(excesso);
				}
			}

			// display header's infos
			std::cout << "\nArquivo do tipo: " << w.getWavRiff()->riff[0] << w.getWavRiff()->riff[1] << w.getWavRiff()->riff[2] << w.getWavRiff()->riff[3];
			std::cout << "\nTamanho excluindo header: " << w.getWavRiff()->riff;
			std::cout << "\nSub-Tipo: " << w.getWave()[0] << w.getWave()[1] << w.getWave()[2] << w.getWave()[3];
			std::cout << "\nIdentificador: " << w.getWavRiffChunk()->id[0] << w.getWavRiffChunk()->id[1] << w.getWavRiffChunk()->id[2] << w.getWavRiffChunk()->id[3];
			std::cout << "\nComprimento do chunk apos header: " << w.getWavRiffChunk()->len;
			std::cout << "\nCategoria do formato: " << w.getWavChunk()->formattag;
			std::cout << "\nTaxa de amostragem: " << w.getWavChunk()->samplingrate;
			std::cout << "\nNumero de canais: " << w.getWavChunk()->numberofchannels;
			std::cout << "\nMedia do num. de bps: " << w.getWavChunk()->avgbytespersecond;
			std::cout << "\nAlinhamento do bloco em bytes: " << w.getWavChunk()->blockalign;

			// The format MUST be PCM!!
			if (w.getWavChunk()->formattag != 1) {
				std::cout << "FORA DO FORMATO PCM...";
				return 0;
			}

			headerDataChunk hdc;
			fread(&hdc, sizeof(hdc), 1, fileReader);
			w.setDataChunk(&hdc);

			int resolucao = (w.getWavChunk()->avgbytespersecond * 8) / (w.getWavChunk()->numberofchannels * w.getWavChunk()->samplingrate); // pq nao bitssample
			int tamanho_da_janela = w.getDataChunk()->chunk_size / w.getWavChunk()->blockalign;

			// display data info
			std::cout << "\nResolucao: " << resolucao;
			std::cout << "\nIdentificacao: " << w.getDataChunk()->data[0] << w.getDataChunk()->data[1] << w.getDataChunk()->data[2] << w.getDataChunk()->data[3];
			std::cout << "\nTamanho do chunk de dados: " << w.getDataChunk()->chunk_size;
			std::cout << "\nNumero de frames para amostrar: " << w.getDataChunk()->chunk_size / w.getWavChunk()->blockalign;
			std::cout << "\nTamanho da janela: " << tamanho_da_janela;

			if ((resolucao == 8) && (w.getWavChunk()->numberofchannels == 1)) {

				unsigned char waveformdata;
				double* amostras_no_tempo = new double[tamanho_da_janela];
				for (int i = 0; i < tamanho_da_janela; i++) {
					fread(&waveformdata, sizeof(waveformdata), 1, fileReader);
					amostras_no_tempo[i] = (double) waveformdata;
				}

				w.setAmostrasNoTempo(amostras_no_tempo);

			} else if ((resolucao == 8) && (w.getWavChunk()->numberofchannels == 2)) {

				unsigned char waveformdata_right;
				unsigned char waveformdata_left;
				double* amostras_no_tempo_left = new double[tamanho_da_janela];
				double* amostras_no_tempo_right = new double[tamanho_da_janela];
				for (int i = 0; i < tamanho_da_janela; i++) {
					fread(&waveformdata_left, sizeof(waveformdata_left), 1, fileReader);
					fread(&waveformdata_right, sizeof(waveformdata_right), 1, fileReader);

					amostras_no_tempo_right[i] = (double) waveformdata_right;
					amostras_no_tempo_left[i] = (double) waveformdata_left;
				}

				w.setAmostrasNoTempoLeft(amostras_no_tempo_left);
				w.setAmostrasNoTempoRight(amostras_no_tempo_right);

			} else if ((resolucao == 16) && (w.getWavChunk()->numberofchannels == 1)) {

				unsigned char waveformdata_lsb, waveformdata_msb;
				double* amostras_no_tempo = new double[tamanho_da_janela];
				for (int i = 0; i < tamanho_da_janela; i++) {
					fread(&waveformdata_lsb, sizeof(waveformdata_lsb), 1, fileReader);
					fread(&waveformdata_msb, sizeof(waveformdata_msb), 1, fileReader);
					amostras_no_tempo[i] = (double) converte2de8para1de16(waveformdata_lsb, waveformdata_msb);
				}

				w.setWaveformDataLsb(&waveformdata_lsb);
				w.setWaveformDataMsb(&waveformdata_msb);
				w.setAmostrasNoTempo(amostras_no_tempo);

			} else if ((resolucao == 16) && (w.getWavChunk()->numberofchannels == 2)) {

				unsigned char waveformdata_lsb_left, waveformdata_lsb_right, waveformdata_msb_left, waveformdata_msb_right;
				double* amostras_no_tempo_left = new double[tamanho_da_janela];
				double* amostras_no_tempo_right = new double[tamanho_da_janela];
				for (int i = 0; i < tamanho_da_janela; i++) {
					fread(&waveformdata_lsb_left, sizeof(waveformdata_lsb_left), 1, fileReader);
					fread(&waveformdata_msb_left, sizeof(waveformdata_msb_left), 1, fileReader);
					fread(&waveformdata_lsb_right, sizeof(waveformdata_lsb_right), 1, fileReader);
					fread(&waveformdata_msb_right, sizeof(waveformdata_msb_right), 1, fileReader);
					amostras_no_tempo_left[i] = (double) converte2de8para1de16(waveformdata_lsb_left, waveformdata_msb_left);
					amostras_no_tempo_right[i] = (double) converte2de8para1de16(waveformdata_lsb_right, waveformdata_msb_right);
				}

				w.setWaveformDataLsbLeft(&waveformdata_lsb_left);
				w.setWaveformDataMsbLeft(&waveformdata_msb_left);
				w.setWaveformDataLsbRight(&waveformdata_lsb_right);
				w.setWaveformDataMsbRight(&waveformdata_msb_right);

				w.setAmostrasNoTempoLeft(amostras_no_tempo_left);
				w.setAmostrasNoTempoRight(amostras_no_tempo_right);

			} else std::cout << "Resolucao ou nmero de canais invalido(s)";

			int c;
			while ((c = getc(fileReader)) != EOF)
				;

			w.setC(c);

			fclose(fileReader);

			return 1;
		}
		char save(Wav w, unsigned char applyFilter = 0) {

			if (strlen(w.getFilePath()) == 0) return 0;

			// file writer
			FILE* fileWriter;

			// Open the file
			if ((fileWriter = fopen(w.getFilePath(), "wb")) == NULL) return 0;

			// Writes the file header
			this->saveHeaders(w, fileWriter);

			int resolucao = (w.getWavChunk()->avgbytespersecond * 8) / (w.getWavChunk()->numberofchannels * w.getWavChunk()->samplingrate); // pq nao bitssample

			int tamanho_da_janela = w.getDataChunk()->chunk_size / w.getWavChunk()->blockalign;

			if ((resolucao == 8) && (w.getWavChunk()->numberofchannels == 1)) {
				this->save1Channel8bitsWav(w, applyFilter, tamanho_da_janela, fileWriter);
			} else if ((resolucao == 8) && (w.getWavChunk()->numberofchannels == 2)) {
				this->save2Channels8bitsWav(w, applyFilter, tamanho_da_janela, fileWriter);
			} else if ((resolucao == 16) && (w.getWavChunk()->numberofchannels == 1)) {
				this->save1Channel16bitsWav(w, applyFilter, tamanho_da_janela, fileWriter);
			} else if ((resolucao == 16) && (w.getWavChunk()->numberofchannels == 2)) {
				this->save2Channels16bitsWav(w, applyFilter, tamanho_da_janela, fileWriter);
			} else std::cout << "Resolucao ou numero de canais invalido(s)";

			int c = w.getC();
			putc(c, fileWriter);

			fclose(fileWriter);

			return 1;
		}

};
#endif /* WavProcessor_class_ */

