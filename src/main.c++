#include<stdio.h>
#include<math.h>
#include<string.h>
#include<iostream>
//---------------------------------------
int main(int i, char* n[]) {
	void modifica_dados_brutos(double*, long, unsigned long);
	short converte2de8para1de16(unsigned char, unsigned char);
	void converte1de16para2de8(short, unsigned char*, unsigned char*);
	FILE* fr;
	FILE* fw;
	char arquivo_wav_resultante[200];
	arquivo_wav_resultante[0] = '\0';
	strcat(&arquivo_wav_resultante[0], n[1]);
	strcat(&arquivo_wav_resultante[0], ".transformados_modificados_e_destransformados.wav");

	if (((fr = fopen(n[1], "rb")) != NULL) && (((fw = fopen(arquivo_wav_resultante, "wb")) != NULL))) {
		struct {
				unsigned char riff[4];
				unsigned long len;
		} riff_header;
		fread(&riff_header, sizeof(riff_header), 1, fr);
		fwrite(&riff_header, sizeof(riff_header), 1, fw);
		std::cout << "\nArquivo do tipo: " << riff_header.riff[0] << riff_header.riff[1] << riff_header.riff[2] << riff_header.riff[3];
		std::cout << "\nTamanho excluindo header: " << riff_header.len;

		///////////////////////////////////////////////////////////////////////////

		unsigned char wave[4];
		fread(&wave, sizeof(wave), 1, fr); //////
		fwrite(&wave, sizeof(wave), 1, fw); //////
		std::cout << "\nSub-Tipo: " << wave[0] << wave[1] << wave[2] << wave[3];

		///////////////////////////////////////////////////////////////////////////
		struct {
				unsigned char id[4];
				unsigned long len;
		} riff_chunk;
		fread(&riff_chunk, sizeof(riff_chunk), 1, fr);
		fwrite(&riff_chunk, sizeof(riff_chunk), 1, fw);
		std::cout << "\nIdentificador: " << riff_chunk.id[0] << riff_chunk.id[1] << riff_chunk.id[2] << riff_chunk.id[3];
		std::cout << "\nComprimento do chunk apos header: " << riff_chunk.len;

		//////////////////////////////////////////////////////////////////////////////
		struct {
				unsigned short formattag;
				unsigned short numberofchannels;
				unsigned long samplingrate;
				unsigned long avgbytespersecond;
				unsigned short blockalign;
		} wave_chunk;
		fread(&wave_chunk, sizeof(wave_chunk), 1, fr);
		fwrite(&wave_chunk, sizeof(wave_chunk), 1, fw);
		if (riff_chunk.len > 16) {
			unsigned char excesso;
			for (int i = 0; i < riff_chunk.len - 16; i++) {
				fread(&excesso, sizeof(excesso), 1, fr);
				fwrite(&excesso, sizeof(excesso), 1, fw);
			}
		}
		std::cout << "\nCategoria do formato: " << wave_chunk.formattag;
		std::cout << "\nNumero de canais: " << wave_chunk.numberofchannels;
		std::cout << "\nTaxa de amostragem: " << wave_chunk.samplingrate;
		std::cout << "\nMedia do num. de bps: " << wave_chunk.avgbytespersecond;
		std::cout << "\nAlinhamento do bloco em bytes: " << wave_chunk.blockalign;

		//////////////////////////////////////////////////////////////////////////////

		if (wave_chunk.formattag == 1) //PCM
				{
			int resolucao = (wave_chunk.avgbytespersecond * 8) / (wave_chunk.numberofchannels * wave_chunk.samplingrate); // pq nao bitssample
			std::cout << "\nResolucao: " << resolucao;

			struct {
					unsigned char data[4];
					unsigned long chunk_size;
			} header_data_chunk;

			fread(&header_data_chunk, sizeof(header_data_chunk), 1, fr);
			fwrite(&header_data_chunk, sizeof(header_data_chunk), 1, fw);
			std::cout << "\nIdentificacao: " << header_data_chunk.data[0] << header_data_chunk.data[1] << header_data_chunk.data[2] << header_data_chunk.data[3];
			std::cout << "\nTamanho do chunk de dados: " << header_data_chunk.chunk_size;
			std::cout << "\nNumero de frames para amostrar: " << header_data_chunk.chunk_size / wave_chunk.blockalign;

			long tamanho_da_janela = header_data_chunk.chunk_size / wave_chunk.blockalign;

			std::cout << "\nTamanho da janela: " << tamanho_da_janela;
			if ((resolucao == 8) && (wave_chunk.numberofchannels == 1)) {
				unsigned char waveformdata;
				double* amostras_no_tempo = new double[tamanho_da_janela];
				for (long i = 0; i < tamanho_da_janela; i++) {
					fread(&waveformdata, sizeof(waveformdata), 1, fr);
					amostras_no_tempo[i] = (double) waveformdata;
				}

				modifica_dados_brutos(&amostras_no_tempo[0], tamanho_da_janela, wave_chunk.samplingrate);

				for (long i = 0; i < tamanho_da_janela; i++) {
					waveformdata = (unsigned char) amostras_no_tempo[i];
					fwrite(&waveformdata, sizeof(waveformdata), 1, fw);
				}
			} else if ((resolucao == 8) && (wave_chunk.numberofchannels == 2)) {
				unsigned char waveformdata_right;
				unsigned char waveformdata_left;
				double* amostras_no_tempo_left = new double[tamanho_da_janela];
				double* amostras_no_tempo_right = new double[tamanho_da_janela];
				for (long i = 0; i < tamanho_da_janela; i++) {
					fread(&waveformdata_left, sizeof(waveformdata_left), 1, fr);
					fread(&waveformdata_right, sizeof(waveformdata_right), 1, fr);

					amostras_no_tempo_right[i] = (double) waveformdata_right;
					amostras_no_tempo_left[i] = (double) waveformdata_left;
				}

				modifica_dados_brutos(&amostras_no_tempo_left[0], tamanho_da_janela, wave_chunk.samplingrate);
				modifica_dados_brutos(&amostras_no_tempo_right[0], tamanho_da_janela, wave_chunk.samplingrate);

				for (long i = 0; i < tamanho_da_janela; i++) {
					waveformdata_left = (unsigned char) amostras_no_tempo_left[i];
					fwrite(&waveformdata_left, sizeof(waveformdata_left), 1, fw);
					waveformdata_right = (unsigned char) amostras_no_tempo_right[i];
					fwrite(&waveformdata_right, sizeof(waveformdata_right), 1, fw);
				}
			} else if ((resolucao == 16) && (wave_chunk.numberofchannels == 1)) {
				unsigned char waveformdata_lsb, waveformdata_msb;
				double* amostras_no_tempo = new double[tamanho_da_janela];
				for (long i = 0; i < tamanho_da_janela; i++) {
					fread(&waveformdata_lsb, sizeof(waveformdata_lsb), 1, fr);
					fread(&waveformdata_msb, sizeof(waveformdata_msb), 1, fr);
					amostras_no_tempo[i] = (double) converte2de8para1de16(waveformdata_lsb, waveformdata_msb);
				}

				modifica_dados_brutos(&amostras_no_tempo[0], tamanho_da_janela, wave_chunk.samplingrate);

				for (long i = 0; i < tamanho_da_janela; i++) {
					converte1de16para2de8((short) (amostras_no_tempo[i]), &waveformdata_lsb, &waveformdata_msb);
					fwrite(&waveformdata_lsb, sizeof(waveformdata_lsb), 1, fw);
					fwrite(&waveformdata_msb, sizeof(waveformdata_msb), 1, fw);
				}
			} else if ((resolucao == 16) && (wave_chunk.numberofchannels == 2)) {
				unsigned char waveformdata_lsb_left, waveformdata_lsb_right, waveformdata_msb_left, waveformdata_msb_right;
				double* amostras_no_tempo_left = new double[tamanho_da_janela];
				double* amostras_no_tempo_right = new double[tamanho_da_janela];
				for (long i = 0; i < tamanho_da_janela; i++) {
					fread(&waveformdata_lsb_left, sizeof(waveformdata_lsb_left), 1, fr);
					fread(&waveformdata_msb_left, sizeof(waveformdata_msb_left), 1, fr);
					fread(&waveformdata_lsb_right, sizeof(waveformdata_lsb_right), 1, fr);
					fread(&waveformdata_msb_right, sizeof(waveformdata_msb_right), 1, fr);
					amostras_no_tempo_left[i] = (double) converte2de8para1de16(waveformdata_lsb_left, waveformdata_msb_left);
					amostras_no_tempo_right[i] = (double) converte2de8para1de16(waveformdata_lsb_right, waveformdata_msb_right);
				}

				modifica_dados_brutos(&amostras_no_tempo_left[0], tamanho_da_janela, wave_chunk.samplingrate);
				modifica_dados_brutos(&amostras_no_tempo_right[0], tamanho_da_janela, wave_chunk.samplingrate);

				for (long i = 0; i < tamanho_da_janela; i++) {
					converte1de16para2de8((short) amostras_no_tempo_left[i], &waveformdata_lsb_left, &waveformdata_msb_left);
					converte1de16para2de8((short) amostras_no_tempo_right[i], &waveformdata_lsb_right, &waveformdata_msb_right);
					fwrite(&waveformdata_lsb_left, sizeof(waveformdata_lsb_left), 1, fw);
					fwrite(&waveformdata_msb_left, sizeof(waveformdata_msb_left), 1, fw);
					fwrite(&waveformdata_lsb_right, sizeof(waveformdata_lsb_right), 1, fw);
					fwrite(&waveformdata_msb_right, sizeof(waveformdata_msb_right), 1, fw);
				}
			} else std::cout << "Resolucao ou nmero de canais invalido(s)";

			unsigned int c;
			while ((c = getc(fr)) != EOF) //termina de gravar os cabe?lho de fim do arquivo wav
				putc(c, fw);

		} else std::cout << "FORA DO FORMATO PCM...";
		fclose(fr);
		fclose(fw);

	} else std::cout << "Arquivo nao existe ou nao pode ser aberto";
	std::cout << "\n\n\n";
}
//--------------------------------------------------------
short converte2de8para1de16(unsigned char lsb, unsigned char msb) {
	return (((msb & 0x80) >> 7) * (32768) + ((msb & 0x40) >> 6) * (16384) + ((msb & 0x20) >> 5) * (8192) + ((msb & 0x10) >> 4) * (4096) + ((msb & 0x08) >> 3) * (2048) + ((msb & 0x04) >> 2) * (1024) + ((msb & 0x02) >> 1) * (512) + ((msb & 0x01)) * (256) + ((lsb & 0x80) >> 7) * (128)
			+ ((lsb & 0x40) >> 6) * (64) + ((lsb & 0x20) >> 5) * (32) + ((lsb & 0x10) >> 4) * (16) + ((lsb & 0x08) >> 3) * (8) + ((lsb & 0x04) >> 2) * (4) + ((lsb & 0x02) >> 1) * (2) + (lsb & 0x01));
}
//-------------------------------------------------------
void converte1de16para2de8(short resultado, unsigned char* lsb, unsigned char* msb) {
	*lsb = (((resultado & 0x0080) >> 7) * (128) + ((resultado & 0x0040) >> 6) * (64) + ((resultado & 0x0020) >> 5) * (32) + ((resultado & 0x0010) >> 4) * (16) + ((resultado & 0x0008) >> 3) * (8) + ((resultado & 0x0004) >> 2) * (4) + ((resultado & 0x0002) >> 1) * (2) + (resultado & 0x0001));
	*msb = (((resultado & 0x8000) >> 15) * (128) + ((resultado & 0x4000) >> 14) * (64) + ((resultado & 0x2000) >> 13) * (32) + ((resultado & 0x1000) >> 12) * (16) + ((resultado & 0x0800) >> 11) * (8) + ((resultado & 0x0400) >> 10) * (4) + ((resultado & 0x0200) >> 9) * (2)
			+ ((resultado & 0x0100) >> 8));
}
//----------------------------------------------------------
void modifica_dados_brutos(double* sinal, long comprimento_do_sinal, int taxa_de_amostragem) {
//esta  a rea de trabalho
//o terceiro parmetro no precisa ser usado nas experincias no domnio do tempo. Usaremos para trabalhar com filtros...
}
//-------------------------------------------------------------
