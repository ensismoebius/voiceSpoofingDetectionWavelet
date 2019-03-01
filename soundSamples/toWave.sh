#!/bin/bash
###############################################################
#                                                             #
#          Converts your 3gpp files to wav files              #
#                                                             #
###############################################################

# Create a file wich will contains the convertion command lines
touch c

# Iterates over all *.3gpp files
while read file;
do
	# Writes the convesion commands to c file
	
	## Converts to wave file
	echo ffmpeg -i $file -acodec pcm_s16le ${file%.*}.wav -loglevel panic >> c

	## The command above do shitty conversion so we need to fix the file headers with the sox command
	echo sox --ignore-length ${file%.*}.wav ${file%.*}_.wav >> c

	## We have to have 44100 bit sampling and 16 bit depth
	echo sox -r 44100 -b 16 ${file%.*}_.wav ${file%.*}_16.wav >> c

	## Erasing bad wav file and non 16 bits wav file
	echo rm ${file%.*}.wav ${file%.*}_.wav >> c

	## Renaming the converted file to the original name
	echo mv ${file%.*}_16.wav ${file%.*}.wav >> c
done <<< $(ls | grep .3gpp);

# Execute the c file
bash c
rm c
