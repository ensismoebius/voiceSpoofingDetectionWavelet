#!/bin/bash
echo  > c
while read file;
do
	echo ffmpeg -i $file -acodec pcm_u8 ${file%.*}.wav -loglevel panic >> c
done <<< $(ls | grep .3gpp);

bash c
rm c
