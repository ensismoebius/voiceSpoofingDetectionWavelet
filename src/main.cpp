/*
 * main.cpp
 *
 *  Created on: 26 de abr de 2019
 *      Author: ensis
 */

#include "Wav.cpp"
#include "WavProcessor.cpp"

int main(int i, char* arrProgramArguments[]) {

	Wav w = Wav();
	w.setFilePath(arrProgramArguments[1]);

	WavProcessor wp = WavProcessor();
	wp.read(w);

}

