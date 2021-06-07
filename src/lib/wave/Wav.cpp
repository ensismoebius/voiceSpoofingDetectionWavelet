/**
 * @author André Furlan <ensismoebius@gmail.com>
 *
 * Based on the original code of:
 * @author Rodrigo Capobianco Guido <guido@ieee.org>
 *
 * This whole project are under GPLv3, for
 * more information read the license file
 *
 * 8 de ago de 2019
 */
#include <cstdint>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include "Wav.h"

void Wav::process()
{
	if (callbackFunction == nullptr) return;

	int resPlusCha = waveResolution * 10 + this->headers.numOfChan;

	switch (resPlusCha)
	{
		case 82:
		case 162:
			(*callbackFunction)(this->dataLeft, amountOfData, this->headers.samplingrate, this->path);
			(*callbackFunction)(this->dataRight, amountOfData, this->headers.samplingrate, this->path);
			break;
		case 81:
		case 161:
			(*callbackFunction)(this->data, amountOfData, this->headers.samplingrate, this->path);
			break;
		default:
			throw std::runtime_error("Invalid number of channels and/or resolution");
			break;
	}
}

void Wav::read(std::string path)
{
	this->clearVectors();
	this->resetMetaData();

	this->path = path;

	std::ifstream ifs;
	ifs.open(path, std::ios::in | std::ios::binary);

	if (ifs.rdstate())
	{
		throw std::runtime_error("Reading the file " + path + " failed");
	}

	// Reads the file headers
	readWaveHeaders(ifs);
	// Reads actual data
	readWaveData(ifs);
	ifs.clear();
	ifs.close();
}

void Wav::write(std::string path)
{
	std::ofstream ofs;
	ofs.open(path, std::ios::out | std::ios::binary);

	if (!ofs.is_open())
	{
		std::cout << "Cannot open file: " << path;
		throw std::runtime_error("Impossible to open the file!");
		return;
	}

	int resPlusCha = waveResolution * 10 + this->headers.numOfChan;

	switch (resPlusCha)
	{
		case 81:
			write8Res1Channel(ofs);
			break;
		case 82:
			write8Res2Channel(ofs);
			break;
		case 161:
			write16Res1Channel(ofs);
			break;
		case 162:
			write16Res2Channel(ofs);
			break;
		default:
			ofs.close();
			throw std::runtime_error("Invalid number of channels and/or resolution");
			break;
	}

	ofs.close();
}

std::vector<long double> Wav::getData() const
{
	return data;
}

std::vector<long double> Wav::getDataLeft() const
{
	return dataLeft;
}

std::vector<long double> Wav::getDataRight() const
{
	return dataRight;
}

std::string Wav::getPath() const
{
	return path;
}

void Wav::setCallbackFunction(void (*callbackFunction)(std::vector<long double> &signal, int &signalLength, unsigned int samplingRate, std::string path))
{
	this->callbackFunction = callbackFunction;
}

short Wav::convert2of8to1of16(unsigned char lsb, unsigned char msb)
{
	return (((msb & 0x80) >> 7) * (32768) + ((msb & 0x40) >> 6) * (16384) + ((msb & 0x20) >> 5) * (8192) + ((msb & 0x10) >> 4) * (4096) + ((msb & 0x08) >> 3) * (2048) + ((msb & 0x04) >> 2) * (1024) + ((msb & 0x02) >> 1) * (512) + ((msb & 0x01)) * (256) + ((lsb & 0x80) >> 7) * (128) + ((lsb & 0x40) >> 6) * (64) + ((lsb & 0x20) >> 5) * (32) + ((lsb & 0x10) >> 4) * (16) + ((lsb & 0x08) >> 3) * (8) + ((lsb & 0x04) >> 2) * (4) + ((lsb & 0x02) >> 1) * (2) + (lsb & 0x01));
}

void Wav::convert1of16to2of8(short result, unsigned char *lsb, unsigned char *msb)
{
	*lsb = (((result & 0x0080) >> 7) * (128) + ((result & 0x0040) >> 6) * (64) + ((result & 0x0020) >> 5) * (32) + ((result & 0x0010) >> 4) * (16) + ((result & 0x0008) >> 3) * (8) + ((result & 0x0004) >> 2) * (4) + ((result & 0x0002) >> 1) * (2) + (result & 0x0001));
	*msb = (((result & 0x8000) >> 15) * (128) + ((result & 0x4000) >> 14) * (64) + ((result & 0x2000) >> 13) * (32) + ((result & 0x1000) >> 12) * (16) + ((result & 0x0800) >> 11) * (8) + ((result & 0x0400) >> 10) * (4) + ((result & 0x0200) >> 9) * (2) + ((result & 0x0100) >> 8));
}

void Wav::readWaveData(std::ifstream &ifs)
{
	int resPlusCha = waveResolution * 10 + this->headers.numOfChan;

	switch (resPlusCha)
	{
		case 81:
			read8Res1Channel(ifs);
			break;
		case 82:
			read8Res2Channel(ifs);
			break;
		case 161:
			read16Res1Channel(ifs);
			break;
		case 162:
			read16Res2Channel(ifs);
			break;
		default:
			ifs.close();
			throw std::runtime_error("Invalid number of channels and/or resolution");
			break;
	}
}

void Wav::readWaveHeaders(std::ifstream &ifs)
{
	ifs.seekg(0, std::ios::beg);
	ifs.read((char*) &this->headers, sizeof(this->headers));

	if (this->headers.audioFormat != 1)
	{
		throw std::runtime_error(this->path + " not in PCM format!");
		return;
	}

	waveResolution = (this->headers.bytesPerSec * 8) / (this->headers.numOfChan * this->headers.samplingrate);
	amountOfData = this->headers.chunkSize / this->headers.blockAlign;
}

inline void Wav::write8Res1Channel(std::ofstream &ofs)
{
	ofs.write((char*) (&this->headers), sizeof(this->headers));

	unsigned char waveformdata;

	for (int i = 0; i < amountOfData; i++)
	{
		waveformdata = (unsigned char) (this->data.at(i));
		ofs.write((char*) (&waveformdata), sizeof(waveformdata));
	}
}

inline void Wav::write8Res2Channel(std::ofstream &ofs)
{
	ofs.write((char*) (&this->headers), sizeof(this->headers));

	unsigned char waveformdata_right, waveformdata_left;

	for (int i = 0; i < amountOfData; i++)
	{
		waveformdata_left = (unsigned char) (this->dataLeft.at(i));
		ofs.write((char*) (&waveformdata_left), sizeof(waveformdata_left));
		waveformdata_right = (unsigned char) (this->dataRight.at(i));
		ofs.write((char*) (&waveformdata_right), sizeof(waveformdata_right));
	}
}

inline void Wav::write16Res1Channel(std::ofstream &ofs)
{
	ofs.write((char*) (&this->headers), sizeof(this->headers));

	unsigned char waveformdata_lsb, waveformdata_msb;

	for (int i = 0; i < amountOfData; i++)
	{
		convert1of16to2of8((short) ((this->data.at(i))), &waveformdata_lsb, &waveformdata_msb);
		ofs.write((char*) (&waveformdata_lsb), sizeof(waveformdata_lsb));
		ofs.write((char*) (&waveformdata_msb), sizeof(waveformdata_msb));
	}
}

inline void Wav::write16Res2Channel(std::ofstream &ofs)
{
	ofs.write((char*) &this->headers, sizeof(this->headers));

	unsigned char waveformdata_lsb_left, waveformdata_lsb_right, waveformdata_msb_left, waveformdata_msb_right;

	for (int i = 0; i < amountOfData; i++)
	{
		convert1of16to2of8((short) (this->dataLeft.at(i)), &waveformdata_lsb_left, &waveformdata_msb_left);
		convert1of16to2of8((short) (this->dataRight.at(i)), &waveformdata_lsb_right, &waveformdata_msb_right);
		ofs.write((char*) (&waveformdata_lsb_left), sizeof(waveformdata_lsb_left));
		ofs.write((char*) (&waveformdata_msb_left), sizeof(waveformdata_msb_left));
		ofs.write((char*) (&waveformdata_lsb_right), sizeof(waveformdata_lsb_right));
		ofs.write((char*) (&waveformdata_msb_right), sizeof(waveformdata_msb_right));
	}
}

inline void Wav::read8Res1Channel(std::ifstream &ifs)
{
	unsigned char waveformdata;
	this->data.resize(amountOfData, 0);
	for (int i = 0; i < amountOfData; i++)
	{
		ifs.read((char*) (&waveformdata), sizeof(waveformdata));
		this->data.at(i) = (double) ((waveformdata));
	}
}

inline void Wav::read8Res2Channel(std::ifstream &ifs)
{
	unsigned char waveformdata_right;
	unsigned char waveformdata_left;
	this->dataLeft.resize(amountOfData, 0);
	this->dataRight.resize(amountOfData, 0);
	for (int i = 0; i < amountOfData; i++)
	{
		ifs.read((char*) (&waveformdata_left), sizeof(waveformdata_left));
		ifs.read((char*) (&waveformdata_right), sizeof(waveformdata_right));
		this->dataLeft.at(i) = (double) ((waveformdata_right));
		this->dataRight.at(i) = (double) ((waveformdata_left));
	}
}

inline void Wav::read16Res1Channel(std::ifstream &ifs)
{
	unsigned char waveformdata_lsb, waveformdata_msb;
	this->data.resize(amountOfData, 0);
	for (int i = 0; i < amountOfData; i++)
	{
		ifs.read((char*) (&waveformdata_lsb), sizeof(waveformdata_lsb));
		ifs.read((char*) (&waveformdata_msb), sizeof(waveformdata_msb));
		this->data.at(i) = (double) ((convert2of8to1of16(waveformdata_lsb, waveformdata_msb)));
	}
}

inline void Wav::read16Res2Channel(std::ifstream &ifs)
{
	unsigned char waveformdata_lsb_left, waveformdata_lsb_right, waveformdata_msb_left, waveformdata_msb_right;
	this->dataLeft.resize(amountOfData, 0);
	this->dataRight.resize(amountOfData, 0);
	for (int i = 0; i < amountOfData; i++)
	{
		ifs.read((char*) (&waveformdata_lsb_left), sizeof(waveformdata_lsb_left));
		ifs.read((char*) (&waveformdata_msb_left), sizeof(waveformdata_msb_left));
		ifs.read((char*) (&waveformdata_lsb_right), sizeof(waveformdata_lsb_right));
		ifs.read((char*) (&waveformdata_msb_right), sizeof(waveformdata_msb_right));
		this->dataLeft.at(i) = (double) ((convert2of8to1of16(waveformdata_lsb_left, waveformdata_msb_left)));
		this->dataRight.at(i) = (double) ((convert2of8to1of16(waveformdata_lsb_right, waveformdata_msb_right)));
	}
}

void Wav::clearVectors()
{
	this->data.clear();
	this->dataLeft.clear();
	this->dataRight.clear();
}

void Wav::resetMetaData()
{
	this->amountOfData = 0;
	this->waveResolution = 0;
	/* RIFF Chunk Descriptor */
	this->headers.RIFF[0] = '\0';
	this->headers.RIFF[1] = '\0';
	this->headers.RIFF[2] = '\0';
	this->headers.RIFF[3] = '\0';
	this->headers.chunkSize = 0;
	this->headers.WAVE[0] = '\0';
	this->headers.WAVE[1] = '\0';
	this->headers.WAVE[2] = '\0';
	this->headers.WAVE[3] = '\0';
	/* "fmt" sub-chunk */
	this->headers.fmt[0] = '\0';
	this->headers.fmt[1] = '\0';
	this->headers.fmt[2] = '\0';
	this->headers.fmt[3] = '\0';
	this->headers.subchunk1Size = 0;
	this->headers.audioFormat = 0;
	this->headers.numOfChan = 0;
	this->headers.samplingrate = 0;
	this->headers.bytesPerSec = 0;
	this->headers.blockAlign = 0;
	this->headers.bitsPerSample = 0;
	/* "data" sub-chunk */
	this->headers.subchunk2ID[0] = '\0';
	this->headers.subchunk2ID[1] = '\0';
	this->headers.subchunk2ID[2] = '\0';
	this->headers.subchunk2ID[3] = '\0';
	this->headers.subchunk2Size = 0;
}
