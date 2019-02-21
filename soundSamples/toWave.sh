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
	#Writes the convesion commands to c file
	
	## Converts to wave file
	echo ffmpeg -i $file -acodec pcm_u8 ${file%.*}.wav -loglevel panic >> c

	## The command above do shitty conversion so we need to fix the file headers with the sox command
	echo sox --ignore-length ${file%.*}.wav ${file%.*}_.wav >> c

	## Erasing bad wav file
	echo rm ${file%.*}.wav >> c

	## Renaming the converted file to the original name
	echo mv ${file%.*}_.wav ${file%.*}.wav >> c
done <<< $(ls | grep .3gpp);

# Execute the c file
bash c
rm c
