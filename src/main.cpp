/*
 * main.cpp
 *
 *  Created on: 26 de abr de 2019
 *      Author: ensis
 */

#include "Wav.cpp"
#include "WavProcessor.cpp"

int main(int i, char* arrProgramArguments[]) {

	WavProcessor wp = WavProcessor();
	Wav* w = wp.read(arrProgramArguments[1]);

	const char wavpath[255] = "/tmp/teste.wav";

	strcpy(w->filePath, wavpath);
	wp.save(w);
}

