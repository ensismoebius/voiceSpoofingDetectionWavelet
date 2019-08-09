#include <bits/types/FILE.h>
#include <string.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iomanip>

// Prototypes
void modifica_dados_brutos(double*, int, unsigned int);
short converte2de8para1de16(unsigned char, unsigned char);
void converte1de16para2de8(short, unsigned char*, unsigned char*);

void normalizeData(double*, int);
double createAlpha(double, double, bool);
double* buildOrthogonalVector(double*, int);

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
	struct {
			unsigned char riff[4]; // what's riff??
			unsigned int len;
	} riffHeader;

	//reads the original file header
	fread(&riffHeader, sizeof(riffHeader), 1, fileReader);

	//writes the processed file header
	fwrite(&riffHeader, sizeof(riffHeader), 1, fileWriter);

	// display header's infos
	std::cout << "\nArquivo do tipo: " << riffHeader.riff[0] << riffHeader.riff[1] << riffHeader.riff[2] << riffHeader.riff[3];
	std::cout << "\nTamanho excluindo header: " << riffHeader.len;

	// Reads the "wave" string ( may be another useless thing form microsoft...)
	unsigned char wave[4];
	fread(&wave, sizeof(wave), 1, fileReader); //////
	fwrite(&wave, sizeof(wave), 1, fileWriter); //////
	std::cout << "\nSub-Tipo: " << wave[0] << wave[1] << wave[2] << wave[3];

	struct {
			unsigned char id[4];
			unsigned int len;
	} riff_chunk;
	fread(&riff_chunk, sizeof(riff_chunk), 1, fileReader);
	fwrite(&riff_chunk, sizeof(riff_chunk), 1, fileWriter);
	std::cout << "\nIdentificador: " << riff_chunk.id[0] << riff_chunk.id[1] << riff_chunk.id[2] << riff_chunk.id[3];
	std::cout << "\nComprimento do chunk apos header: " << riff_chunk.len;

	//////////////////////////////////////////////////////////////////////////////
	struct {
			unsigned short formattag;
			unsigned short numberofchannels;
			unsigned int samplingrate;
			unsigned int avgbytespersecond;
			unsigned short blockalign;
	} wave_chunk;
	fread(&wave_chunk, sizeof(wave_chunk), 1, fileReader);
	fwrite(&wave_chunk, sizeof(wave_chunk), 1, fileWriter);
	if (riff_chunk.len > 16) {
		unsigned char excesso;
		for (unsigned int i = 0; i < riff_chunk.len - 16; i++) {
			fread(&excesso, sizeof(excesso), 1, fileReader);
			fwrite(&excesso, sizeof(excesso), 1, fileWriter);
		}
	}
	std::cout << "\nCategoria do formato: " << wave_chunk.formattag;
	std::cout << "\nNumero de canais: " << wave_chunk.numberofchannels;
	std::cout << "\nTaxa de amostragem: " << wave_chunk.samplingrate;
	std::cout << "\nMedia do num. de bps: " << wave_chunk.avgbytespersecond;
	std::cout << "\nAlinhamento do bloco em bytes: " << wave_chunk.blockalign;

	// The format MUST be PCM!!
	if (wave_chunk.formattag != 1) {
		std::cout << "FORA DO FORMATO PCM...";
		return 0;
	}

	int resolucao = (wave_chunk.avgbytespersecond * 8) / (wave_chunk.numberofchannels * wave_chunk.samplingrate); // pq nao bitssample
	std::cout << "\nResolucao: " << resolucao;

	struct {
			unsigned char data[4];
			unsigned int chunk_size;
	} header_data_chunk;

	fread(&header_data_chunk, sizeof(header_data_chunk), 1, fileReader);
	fwrite(&header_data_chunk, sizeof(header_data_chunk), 1, fileWriter);
	std::cout << "\nIdentificacao: " << header_data_chunk.data[0] << header_data_chunk.data[1] << header_data_chunk.data[2] << header_data_chunk.data[3];
	std::cout << "\nTamanho do chunk de dados: " << header_data_chunk.chunk_size;
	std::cout << "\nNumero de frames para amostrar: " << header_data_chunk.chunk_size / wave_chunk.blockalign;

	int tamanho_da_janela = header_data_chunk.chunk_size / wave_chunk.blockalign;

	std::cout << "\nTamanho da janela: " << tamanho_da_janela;
	if ((resolucao == 8) && (wave_chunk.numberofchannels == 1)) {
		unsigned char waveformdata;
		double* amostras_no_tempo = new double[tamanho_da_janela];
		for (int i = 0; i < tamanho_da_janela; i++) {
			fread(&waveformdata, sizeof(waveformdata), 1, fileReader);
			amostras_no_tempo[i] = (double) waveformdata;
		}
		//modifica_dados_brutos(&amostras_no_tempo[0], tamanho_da_janela, wave_chunk.samplingrate);
		modifica_dados_brutos(&amostras_no_tempo[0], tamanho_da_janela, wave_chunk.samplingrate);

		for (int i = 0; i < tamanho_da_janela; i++) {
			waveformdata = (unsigned char) amostras_no_tempo[i];
			fwrite(&waveformdata, sizeof(waveformdata), 1, fileWriter);
		}
	} else if ((resolucao == 8) && (wave_chunk.numberofchannels == 2)) {
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

		modifica_dados_brutos(&amostras_no_tempo_left[0], tamanho_da_janela, wave_chunk.samplingrate);
		modifica_dados_brutos(&amostras_no_tempo_right[0], tamanho_da_janela, wave_chunk.samplingrate);

		for (int i = 0; i < tamanho_da_janela; i++) {
			waveformdata_left = (unsigned char) amostras_no_tempo_left[i];
			fwrite(&waveformdata_left, sizeof(waveformdata_left), 1, fileWriter);
			waveformdata_right = (unsigned char) amostras_no_tempo_right[i];
			fwrite(&waveformdata_right, sizeof(waveformdata_right), 1, fileWriter);
		}
	} else if ((resolucao == 16) && (wave_chunk.numberofchannels == 1)) {
		unsigned char waveformdata_lsb, waveformdata_msb;
		double* amostras_no_tempo = new double[tamanho_da_janela];
		for (int i = 0; i < tamanho_da_janela; i++) {
			fread(&waveformdata_lsb, sizeof(waveformdata_lsb), 1, fileReader);
			fread(&waveformdata_msb, sizeof(waveformdata_msb), 1, fileReader);
			amostras_no_tempo[i] = (double) converte2de8para1de16(waveformdata_lsb, waveformdata_msb);
		}

		modifica_dados_brutos(&amostras_no_tempo[0], tamanho_da_janela, wave_chunk.samplingrate);

		for (int i = 0; i < tamanho_da_janela; i++) {
			converte1de16para2de8((short) (amostras_no_tempo[i]), &waveformdata_lsb, &waveformdata_msb);
			fwrite(&waveformdata_lsb, sizeof(waveformdata_lsb), 1, fileWriter);
			fwrite(&waveformdata_msb, sizeof(waveformdata_msb), 1, fileWriter);
		}
	} else if ((resolucao == 16) && (wave_chunk.numberofchannels == 2)) {
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

		modifica_dados_brutos(&amostras_no_tempo_left[0], tamanho_da_janela, wave_chunk.samplingrate);
		modifica_dados_brutos(&amostras_no_tempo_right[0], tamanho_da_janela, wave_chunk.samplingrate);

		for (int i = 0; i < tamanho_da_janela; i++) {
			converte1de16para2de8((short) amostras_no_tempo_left[i], &waveformdata_lsb_left, &waveformdata_msb_left);
			converte1de16para2de8((short) amostras_no_tempo_right[i], &waveformdata_lsb_right, &waveformdata_msb_right);
			fwrite(&waveformdata_lsb_left, sizeof(waveformdata_lsb_left), 1, fileWriter);
			fwrite(&waveformdata_msb_left, sizeof(waveformdata_msb_left), 1, fileWriter);
			fwrite(&waveformdata_lsb_right, sizeof(waveformdata_lsb_right), 1, fileWriter);
			fwrite(&waveformdata_msb_right, sizeof(waveformdata_msb_right), 1, fileWriter);
		}
	} else std::cout << "Resolucao ou nmero de canais invalido(s)";

	int c;
	while ((c = getc(fileReader)) != EOF) //termina de gravar os cabe?lho de fim do arquivo wav
		putc(c, fileWriter);

	fclose(fileReader);
	fclose(fileWriter);

}

short converte2de8para1de16(unsigned char lsb, unsigned char msb) {
	return (((msb & 0x80) >> 7) * (32768) + ((msb & 0x40) >> 6) * (16384) + ((msb & 0x20) >> 5) * (8192) + ((msb & 0x10) >> 4) * (4096) + ((msb & 0x08) >> 3) * (2048) + ((msb & 0x04) >> 2) * (1024) + ((msb & 0x02) >> 1) * (512) + ((msb & 0x01)) * (256) + ((lsb & 0x80) >> 7) * (128) + ((lsb & 0x40) >> 6) * (64) + ((lsb & 0x20) >> 5) * (32) + ((lsb & 0x10) >> 4) * (16) + ((lsb & 0x08) >> 3) * (8) + ((lsb & 0x04) >> 2) * (4) + ((lsb & 0x02) >> 1) * (2) + (lsb & 0x01));
}

void converte1de16para2de8(short resultado, unsigned char* lsb, unsigned char* msb) {
	*lsb = (((resultado & 0x0080) >> 7) * (128) + ((resultado & 0x0040) >> 6) * (64) + ((resultado & 0x0020) >> 5) * (32) + ((resultado & 0x0010) >> 4) * (16) + ((resultado & 0x0008) >> 3) * (8) + ((resultado & 0x0004) >> 2) * (4) + ((resultado & 0x0002) >> 1) * (2) + (resultado & 0x0001));
	*msb = (((resultado & 0x8000) >> 15) * (128) + ((resultado & 0x4000) >> 14) * (64) + ((resultado & 0x2000) >> 13) * (32) + ((resultado & 0x1000) >> 12) * (16) + ((resultado & 0x0800) >> 11) * (8) + ((resultado & 0x0400) >> 10) * (4) + ((resultado & 0x0200) >> 9) * (2) + ((resultado & 0x0100) >> 8));
}

void doAFineAmplification(double* signal, int signalLength) {
	double highestSignal = 0;

	// find the highest signal
	for (int i = 0; i < signalLength; ++i) {
		double value = abs(signal[i]);

		if (value > highestSignal) highestSignal = value;

	}

	// TODO I don't know why but 32767 distorts the sound A LOT
	double multiplicationRatio = 32767 / highestSignal;

	for (int i = 0; i < signalLength; ++i) {
		signal[i] *= multiplicationRatio;
	}
}

void silentHalfOfTheSoundTrack(double* signal, int signalLength) {
	int middleSignalIndex = signalLength / 2;

	for (int i = middleSignalIndex; i < signalLength; ++i) {
		signal[i] = 0;
	}
}

void xuxasDevilInvocation(double* signal, int signalLength) {
	int middleSignalIndex = signalLength / 2;
	int tempVar;

	for (int i = middleSignalIndex; i < signalLength; ++i) {
		tempVar = signal[i];
		signal[i] = signal[signalLength - i - 1];
		signal[signalLength - i - 1] = tempVar;
	}
}

void halfVolume(double* signal, int signalLength) {

	for (int i = 0; i < signalLength; ++i) {
		signal[i] *= .5;
	}
}

void addEchoes(double* signal, int signalLength) {

	// the "time" sound get to bounce and back
	int bouncingTime = 100000;

	// Iterate over all values
	for (int i = 0; i < signalLength; ++i) {

		// while the data starts to end we decrease
		// the bouncing time to avoid access data
		// outside the array
		if (i + bouncingTime - 1 == signalLength) {
			bouncingTime--;
		}

		// we have to wait the bouncingTime before start echoing
		if (i > bouncingTime - 1) {

			// the resulting signal are going to be
			// the average of the current signal
			// plus 80% of the previous signal
			signal[i] = (signal[i - bouncingTime] * .8 + signal[i]) / 2;
		}
	}
}

void convolution(double* data, int dataLength, double* inverseDFTFilter, int inverseDFTFilterLength) {

	// holds the final convoluted data
	double* convolutedSignal = new double[dataLength + inverseDFTFilterLength - 1];

	// the the convoluted data index
	int convIndex = 0;

	// Iterates over filter
	for (int filterIndex = 0; filterIndex < inverseDFTFilterLength; ++filterIndex) {

		// this guarantee the digit shift
		convIndex = filterIndex;

		// do the the math
		for (int dataIndex = 0; dataIndex < dataLength; dataIndex++) {
			convolutedSignal[convIndex] += data[dataIndex] * inverseDFTFilter[filterIndex];
			convIndex++;
		}
	}

	for (int i = 0; i < dataLength; i++) {
		data[i] = convolutedSignal[i];
	}
}

void detectSilences(double* signal, int signalLength) {
	double lowestSignal = 0;
	int tolerance = 10;

	// find the highest signal
	for (int i = 0; i < signalLength; ++i) {
		double value = abs(signal[i]);

		if (value < lowestSignal) lowestSignal = value;

	}

	for (int i = 0; i < signalLength; ++i) {

		if (signal[i] <= lowestSignal + tolerance && signal[i] >= lowestSignal - tolerance) {
			std::cout << "\nSilence in the room!!!!" << signal[i];
		}
	}
}

double createAlpha(double samplingRate, double filterMaxFrequency, bool highPass = false) {

	double alpha = M_PI * filterMaxFrequency / (samplingRate / 2);

	if (highPass) {
		return M_PI - alpha;
	}

	return alpha;
}

void normalizeData(double* signal, int comprimento_do_sinal) {

	double sum = 0;

	for (int i = 0; i < comprimento_do_sinal; ++i) {
		sum += signal[i];
	}

	for (int i = 0; i < comprimento_do_sinal; ++i) {
		signal[i] /= sum;
	}

}

double* createLowPassFilter(int order, double samplingRate, double filterMaxFrequency) {

	// Order MUST be odd
	if (order % 2 == 0) {
		return 0;
	}

	double* filter = new double[order + 1];

	//Calculating the alpha
	double alpha = createAlpha(samplingRate, filterMaxFrequency);

	double halfOrderSize = (double) (order / 2.0);

	for (int n = 0; n <= order; ++n) {

		filter[n] = sin(alpha * (n - halfOrderSize)) / (M_PI * (n - halfOrderSize));
	}

	normalizeData(filter, order + 1);

	return filter;
}

double* createHighPassFilter(int order, double samplingRate, double filterStartFrequency) {

	// Order MUST be odd
	if (order % 2 == 0) {
		return 0;
	}

	// Filter holder
	double* filter = new double[order + 1];

	//Calculating the alpha for high pass filter
	double alpha = createAlpha(samplingRate, filterStartFrequency, true);

	double halfOrderSize = (double) (order / 2.0);

	// Calculate low pass filter
	for (int n = 0; n <= order; ++n) {
		filter[n] = sin(alpha * (n - halfOrderSize)) / (M_PI * (n - halfOrderSize));
	}

	// normalizing data
	normalizeData(filter, order + 1);

	// Builds the orthogonal vector
	// and return the final result (high pass filter)
	return buildOrthogonalVector(filter, order + 1);
}

double* buildOrthogonalVector(double* originalVector, int vectorSize) {

	double* finalResult = new double[vectorSize];

	int middleSignalIndex = vectorSize / 2;
	double tempVar;
	double inverter = 1.0;

	for (int i = middleSignalIndex; i < vectorSize; ++i) {
		tempVar = originalVector[i];

		finalResult[i] = originalVector[vectorSize - i - 1] * (-inverter);
		finalResult[vectorSize - i - 1] = tempVar * inverter;
		inverter *= -1.0;
	}

	return finalResult;
}

double *bandPassFilter(int order, double samplingRate, double startFrequency, double finalFrequency) {

	// Order MUST be odd
	if (order % 2 == 0) {
		return 0;
	}

	double* lowPassMax = createLowPassFilter(order, samplingRate, finalFrequency);
	double* lowPassMin = createLowPassFilter(order, samplingRate, startFrequency);

	normalizeData(lowPassMax, order + 1);
	normalizeData(lowPassMin, order + 1);

	for (int i = 0; i < order + 1; i++) {
		lowPassMax[i] = lowPassMax[i] - lowPassMin[i];
	}

	return lowPassMax;
}

double *bandStopFilter(int order, double samplingRate, double startFrequency, double finalFrequency) {

	// Order MUST be odd
	if (order % 2 == 0) {
		return 0;
	}

	double* highPass = createHighPassFilter(order, samplingRate, startFrequency);
	double* lowPass = createLowPassFilter(order, samplingRate, finalFrequency);

	normalizeData(highPass, order + 1);
	normalizeData(lowPass, order + 1);

	for (int i = 0; i < order + 1; i++) {
		lowPass[i] = lowPass[i] + highPass[i];
	}

	return lowPass;
}

double *createTriangularWindow(int order) {

	// order plus 1 is the amount of items
	double* w = new double[order + 1];

	// The reference point is amount of items divided by 2
	double referencePoint = order / 2.0;

	int n = 0;
	for (; n <= referencePoint; n++) {
		w[n] = 2.0 * n / order;
	}

	for (; n <= order; n++) {
		w[n] = 2.0 - 2.0 * n / order;
	}
	return w;
}

void applyWindow(double* filter, double *window, int order) {
	do {
		filter[order] *= window[order];
	} while (order--);
}

void discreteCosineTransform(double* vector, long vectorLength) {

	long N = vectorLength;
	double sum;
	double multi;
	double* F = new double[vectorLength];

	for (int u = 0; u < N; u++) {

		sum = 0.0;
		multi = u == 0 ? 1.0 / sqrt(2.0) : 1.0;

		for (int i = 0; i < N; i++) {
			sum += multi * cos(((M_PI * u) / (2.0 * N)) * (2.0 * i + 1)) * vector[u];
		}
		F[u] = sum;
	}

	double maior = F[1];

	for (int i = 2; i < N; i++) {

		if (F[i] > maior) {
			maior = F[i];
		}
	}

	for (int i = 1; i < N; i++) {
		F[i] /= maior;
	}

	for (int i = 0; i < N; i++) {
		vector[i] = F[i];
	}

	delete[] F;
}

double* createFeatureVector(double* signal, int signalLength, int order, double samplingRate) {
	// Ranges for MEL scale
	double ranges[14] = { 20, 160, 394, 670, 1000, 1420, 1900, 2450, 3120, 4000, 5100, 6600, 9000, 14000 };

	double sum = 0;
	int rangesSize = 14;
	double rangeEnd = 0;
	double rangeStart = 0;

	normalizeData(signal, signalLength);

	// Ranges for BARK scale
	//double ranges[25] = { 20, 100, 200, 300, 400, 510, 630, 770, 920, 1080, 1270, 1480, 1720, 2000, 2320, 2700, 3150, 3700, 4400, 5300, 6400, 7700, 9500, 12000, 15500 };

	double* filter = new double[order];
	double* window = createTriangularWindow(order);
	double* featureVector = new double[rangesSize - 1];
	double* copiedSignal = new double[signalLength];

	for (int i = 0; i < rangesSize - 1; i++) {

		// Select start and end ranges
		rangeStart = ranges[i];
		rangeEnd = ranges[i + 1];

		// Create the filter
		filter = bandPassFilter(order, samplingRate, rangeStart, rangeEnd);

		// Apply window
		applyWindow(filter, window, order);

		// Create a copy of the signal
		for (int j = 0; j < signalLength; j++) {
			copiedSignal[j] = signal[j];
		}

		// Apply the filter
		convolution(copiedSignal, signalLength, filter, order);

		for (int j = 0; j < signalLength; j++) {
			// Calculate the energies for each energy interval, apply log to it.
			double v = pow(copiedSignal[j], 2);
			v = v == 0 ? 0 : log2(v);
			featureVector[i] += v;

			// Calculate the sum of all energies
			sum += v;
		}

	}

	// Normalize the resulting feature vector
	for (int i = 0; i < rangesSize - 1; i++) {
		featureVector[i] = featureVector[i] / sum;
	}

	//	double teste[8] = { 100, 100, 100, 100, 100, 100, 100, 100 };
	//	discreteCosineTransform(teste, 8);

	// Apply a DCT (Discrete Cosine Transform)
	discreteCosineTransform(featureVector, rangesSize);

	delete[] filter;
	delete[] window;
	delete[] copiedSignal;

	return featureVector;
}

void modifica_dados_brutos(double* signal, int signalLength, unsigned int taxa_de_amostragem) {
	//detectSilences(signal, comprimento_do_sinal);
	//xuxasDevilInvocation(signal, comprimento_do_sinal);
	//addEchoes(signal, comprimento_do_sinal);

	//doAFineAmplification(signal, comprimento_do_sinal);

	//silentHalfOfTheSoundTrack(signal, comprimento_do_sinal);

	unsigned int samplingRate = 44100;
	unsigned int filterOrder = 27;

	std::cout << std::fixed;
	std::cout << std::setprecision(11);
	std::cout << std::endl;
	double* fv = createFeatureVector(signal, signalLength, filterOrder, samplingRate);
	for (int i = 0; i < 14; i++) {
		std::cout << fv[i] << " ";
	}
	std::cout << std::endl;

	normalizeData(signal, signalLength);
	double* window = createTriangularWindow(filterOrder);
	double* filter = bandPassFilter(filterOrder, 44100, 50000, 100000);
	applyWindow(filter, window, filterOrder);

	convolution(signal, signalLength, filter, filterOrder);
}

