/*
 * wavStructures.h
 *
 *  Created on: 26 de abr de 2019
 *      Author: ensis
 */

#ifndef SRC_WAVSTRUCTURES_H_
#define SRC_WAVSTRUCTURES_H_

typedef struct hwr {
		unsigned char riff[4]; // what's riff??
		unsigned int len;
} headerWavRiff;

typedef struct hwrc {
		unsigned char id[4];
		unsigned int len;
} headerWavRiffChunk;

typedef struct hwc {
		unsigned short formattag;
		unsigned short numberofchannels;
		unsigned int samplingrate;
		unsigned int avgbytespersecond;
		unsigned short blockalign;
} headerWavChunk;

typedef struct hdc {
		unsigned char data[4];
		unsigned int chunk_size;
} headerDataChunk;

#endif /* SRC_WAVSTRUCTURES_H_ */
