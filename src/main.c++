#include<math.h>
#include<stdio.h>
#include<string.h>
#include<iostream>

// Prototypes
void modifica_dados_brutos(double*, long, unsigned long);
short converte2de8para1de16(unsigned char, unsigned char);
void converte1de16para2de8(short, unsigned char*, unsigned char*);

int main(int i, char* arrProgramArguments[]) {

	//file reader
	FILE* fileReader;

	//file writer
	FILE* fileWriter;

	//Building the name of the processed file: 200 is the maximum file length
	char processedFilePath[200];
	processedFilePath[0] = '\0'; // Adding an string terminator for cout
	strcat(&processedFilePath[0], arrProgramArguments[1]); // copying the file name
	strcat(&processedFilePath[0], ".transformados_modificados_e_destransformados.wav"); // appending a suffix to filename

	// Opening the file to read and the file to write,
	// if one or another fails just stop the program
	if (((fileReader = fopen(arrProgramArguments[1], "rb")) == NULL) || (((fileWriter = fopen(processedFilePath, "wb")) == NULL))) {
		std::cout << "Arquivo nao existe ou nao pode ser aberto";
		std::cout << "\n\arrProgramArguments\arrProgramArguments";
		return 0;
	}

	// Declaring the struct for the file header
	// TODO Ask why an struct to read a file and how it interacts with fread
	struct {
			unsigned char riff[4]; // what's riff??
			unsigned long len;
	} fileHeader;

	//reads the original file header
	fread(&fileHeader, sizeof(fileHeader), 1, fileReader);

	//writes the processed file header
	fwrite(&fileHeader, sizeof(fileHeader), 1, fileWriter);

	// dsiplay header's infos
	std::cout << "\nArquivo do tipo: " << fileHeader.riff[0] << fileHeader.riff[1] << fileHeader.riff[2] << fileHeader.riff[3];
	std::cout << "\nTamanho excluindo header: " << fileHeader.len;


	unsigned char wave[4];
	fread(&wave, sizeof(wave), 1, fileReader); //////
	fwrite(&wave, sizeof(wave), 1, fileWriter); //////
	std::cout << "\nSub-Tipo: " << wave[0] << wave[1] << wave[2] << wave[3];

	///////////////////////////////////////////////////////////////////////////
	struct {
			unsigned char id[4];
			unsigned long len;
	} riff_chunk;
	fread(&riff_chunk, sizeof(riff_chunk), 1, fileReader);
	fwrite(&riff_chunk, sizeof(riff_chunk), 1, fileWriter);
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
	fread(&wave_chunk, sizeof(wave_chunk), 1, fileReader);
	fwrite(&wave_chunk, sizeof(wave_chunk), 1, fileWriter);
	if (riff_chunk.len > 16) {
		unsigned char excesso;
		for (int i = 0; i < riff_chunk.len - 16; i++) {
			fread(&excesso, sizeof(excesso), 1, fileReader);
			fwrite(&excesso, sizeof(excesso), 1, fileWriter);
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

		fread(&header_data_chunk, sizeof(header_data_chunk), 1, fileReader);
		fwrite(&header_data_chunk, sizeof(header_data_chunk), 1, fileWriter);
		std::cout << "\nIdentificacao: " << header_data_chunk.data[0] << header_data_chunk.data[1] << header_data_chunk.data[2] << header_data_chunk.data[3];
		std::cout << "\nTamanho do chunk de dados: " << header_data_chunk.chunk_size;
		std::cout << "\nNumero de frames para amostrar: " << header_data_chunk.chunk_size / wave_chunk.blockalign;

		long tamanho_da_janela = header_data_chunk.chunk_size / wave_chunk.blockalign;

		std::cout << "\nTamanho da janela: " << tamanho_da_janela;
		if ((resolucao == 8) && (wave_chunk.numberofchannels == 1)) {
			unsigned char waveformdata;
			double* amostras_no_tempo = new double[tamanho_da_janela];
			for (long i = 0; i < tamanho_da_janela; i++) {
				fread(&waveformdata, sizeof(waveformdata), 1, fileReader);
				amostras_no_tempo[i] = (double) waveformdata;
			}

			modifica_dados_brutos(&amostras_no_tempo[0], tamanho_da_janela, wave_chunk.samplingrate);

			for (long i = 0; i < tamanho_da_janela; i++) {
				waveformdata = (unsigned char) amostras_no_tempo[i];
				fwrite(&waveformdata, sizeof(waveformdata), 1, fileWriter);
			}
		} else if ((resolucao == 8) && (wave_chunk.numberofchannels == 2)) {
			unsigned char waveformdata_right;
			unsigned char waveformdata_left;
			double* amostras_no_tempo_left = new double[tamanho_da_janela];
			double* amostras_no_tempo_right = new double[tamanho_da_janela];
			for (long i = 0; i < tamanho_da_janela; i++) {
				fread(&waveformdata_left, sizeof(waveformdata_left), 1, fileReader);
				fread(&waveformdata_right, sizeof(waveformdata_right), 1, fileReader);

				amostras_no_tempo_right[i] = (double) waveformdata_right;
				amostras_no_tempo_left[i] = (double) waveformdata_left;
			}

			modifica_dados_brutos(&amostras_no_tempo_left[0], tamanho_da_janela, wave_chunk.samplingrate);
			modifica_dados_brutos(&amostras_no_tempo_right[0], tamanho_da_janela, wave_chunk.samplingrate);

			for (long i = 0; i < tamanho_da_janela; i++) {
				waveformdata_left = (unsigned char) amostras_no_tempo_left[i];
				fwrite(&waveformdata_left, sizeof(waveformdata_left), 1, fileWriter);
				waveformdata_right = (unsigned char) amostras_no_tempo_right[i];
				fwrite(&waveformdata_right, sizeof(waveformdata_right), 1, fileWriter);
			}
		} else if ((resolucao == 16) && (wave_chunk.numberofchannels == 1)) {
			unsigned char waveformdata_lsb, waveformdata_msb;
			double* amostras_no_tempo = new double[tamanho_da_janela];
			for (long i = 0; i < tamanho_da_janela; i++) {
				fread(&waveformdata_lsb, sizeof(waveformdata_lsb), 1, fileReader);
				fread(&waveformdata_msb, sizeof(waveformdata_msb), 1, fileReader);
				amostras_no_tempo[i] = (double) converte2de8para1de16(waveformdata_lsb, waveformdata_msb);
			}

			modifica_dados_brutos(&amostras_no_tempo[0], tamanho_da_janela, wave_chunk.samplingrate);

			for (long i = 0; i < tamanho_da_janela; i++) {
				converte1de16para2de8((short) (amostras_no_tempo[i]), &waveformdata_lsb, &waveformdata_msb);
				fwrite(&waveformdata_lsb, sizeof(waveformdata_lsb), 1, fileWriter);
				fwrite(&waveformdata_msb, sizeof(waveformdata_msb), 1, fileWriter);
			}
		} else if ((resolucao == 16) && (wave_chunk.numberofchannels == 2)) {
			unsigned char waveformdata_lsb_left, waveformdata_lsb_right, waveformdata_msb_left, waveformdata_msb_right;
			double* amostras_no_tempo_left = new double[tamanho_da_janela];
			double* amostras_no_tempo_right = new double[tamanho_da_janela];
			for (long i = 0; i < tamanho_da_janela; i++) {
				fread(&waveformdata_lsb_left, sizeof(waveformdata_lsb_left), 1, fileReader);
				fread(&waveformdata_msb_left, sizeof(waveformdata_msb_left), 1, fileReader);
				fread(&waveformdata_lsb_right, sizeof(waveformdata_lsb_right), 1, fileReader);
				fread(&waveformdata_msb_right, sizeof(waveformdata_msb_right), 1, fileReader);
				amostras_no_tempo_left[i] = (double) converte2de8para1de16(waveformdata_lsb_left, waveformdata_msb_left);
				amostras_no_tempo_right[i] = (double) converte2de8para1de16(waveformdata_lsb_right, waveformdata_msb_right);
			}

			modifica_dados_brutos(&amostras_no_tempo_left[0], tamanho_da_janela, wave_chunk.samplingrate);
			modifica_dados_brutos(&amostras_no_tempo_right[0], tamanho_da_janela, wave_chunk.samplingrate);

			for (long i = 0; i < tamanho_da_janela; i++) {
				converte1de16para2de8((short) amostras_no_tempo_left[i], &waveformdata_lsb_left, &waveformdata_msb_left);
				converte1de16para2de8((short) amostras_no_tempo_right[i], &waveformdata_lsb_right, &waveformdata_msb_right);
				fwrite(&waveformdata_lsb_left, sizeof(waveformdata_lsb_left), 1, fileWriter);
				fwrite(&waveformdata_msb_left, sizeof(waveformdata_msb_left), 1, fileWriter);
				fwrite(&waveformdata_lsb_right, sizeof(waveformdata_lsb_right), 1, fileWriter);
				fwrite(&waveformdata_msb_right, sizeof(waveformdata_msb_right), 1, fileWriter);
			}
		} else std::cout << "Resolucao ou nmero de canais invalido(s)";

		unsigned int c;
		while ((c = getc(fileReader)) != EOF) //termina de gravar os cabe?lho de fim do arquivo wav
			putc(c, fileWriter);

	} else std::cout << "FORA DO FORMATO PCM...";
	fclose(fileReader);
	fclose(fileWriter);

}

short converte2de8para1de16(unsigned char lsb, unsigned char msb) {
	return (((msb & 0x80) >> 7) * (32768) + ((msb & 0x40) >> 6) * (16384) + ((msb & 0x20) >> 5) * (8192) + ((msb & 0x10) >> 4) * (4096) + ((msb & 0x08) >> 3) * (2048) + ((msb & 0x04) >> 2) * (1024) + ((msb & 0x02) >> 1) * (512) + ((msb & 0x01)) * (256) + ((lsb & 0x80) >> 7) * (128)
			+ ((lsb & 0x40) >> 6) * (64) + ((lsb & 0x20) >> 5) * (32) + ((lsb & 0x10) >> 4) * (16) + ((lsb & 0x08) >> 3) * (8) + ((lsb & 0x04) >> 2) * (4) + ((lsb & 0x02) >> 1) * (2) + (lsb & 0x01));
}

void converte1de16para2de8(short resultado, unsigned char* lsb, unsigned char* msb) {
	*lsb = (((resultado & 0x0080) >> 7) * (128) + ((resultado & 0x0040) >> 6) * (64) + ((resultado & 0x0020) >> 5) * (32) + ((resultado & 0x0010) >> 4) * (16) + ((resultado & 0x0008) >> 3) * (8) + ((resultado & 0x0004) >> 2) * (4) + ((resultado & 0x0002) >> 1) * (2) + (resultado & 0x0001));
	*msb = (((resultado & 0x8000) >> 15) * (128) + ((resultado & 0x4000) >> 14) * (64) + ((resultado & 0x2000) >> 13) * (32) + ((resultado & 0x1000) >> 12) * (16) + ((resultado & 0x0800) >> 11) * (8) + ((resultado & 0x0400) >> 10) * (4) + ((resultado & 0x0200) >> 9) * (2)
			+ ((resultado & 0x0100) >> 8));
}

void modifica_dados_brutos(double* sinal, long comprimento_do_sinal, unsigned long taxa_de_amostragem) {
}
