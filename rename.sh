t=$(ls soundSamples/separated/playback/Voz?/*/? | sed -z 's/:\n/\//g' | sed -z 's/\n\n/\n/g')
for file in $t
do
	fileName=$(echo $file | cut -d'/' -f4); 
	lang=$(echo $file | cut -d'/' -f5); 
	digit=$(echo $file | cut -d'/' -f6); 

	mkdir -p soundSamples/separated/playback/$lang/$digit

	mv $file soundSamples/separated/playback/$lang/$digit/$fileName.wav
done
