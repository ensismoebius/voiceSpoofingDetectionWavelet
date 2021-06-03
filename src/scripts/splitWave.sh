
dialog --title "Welcome guys" --msgbox "This is the auto split script" 5 50

randomNumber=$(printf '%s\n' $(echo "scale=8; $RANDOM/32768" | bc ) | sed  's/\.//g')
historyFile="/tmp/waveSplitHistory"

# This dialog shows the more recent destiny chosen
content=$(cat -n $historyFile | tac)
t="dialog --title 'Shortcuts' --menu 'Choose a recent destiny directory - Press esc to skip' 0 0 0
${content}
--stdout"

# Execute the dialog and retrieve the option
option=$(eval $t)

# If no option was chosen the show the file browser 
if [ "$option" == "" ]
then
	# Selection dialog on which user must choose where to save the files
	fileDestiny=$(dialog --title "Select a directory to save splited waves" --dselect "$PWD" 10 70 --stdout) 

	# update the recent files history
	echo $fileDestiny >> $historyFile
else
	fileDestiny=$(echo "$content" | tail -n$option | head -n1 | awk '{print $2}')
fi

# If no arguments was given to this script then ask the user which file to process
if [ "$1" == "" ]
then
	sourceFile=$(dialog --title "Select the source file" --fselect "$PWD" 10 70 --stdout)
else
	sourceFile=$1
fi

# Play first 2 seconds of the audio in order to user known the noise level
dialog --infobox "Listen..." 5 40
play -q $sourceFile trim 00:00 00:03 2> /dev/null

# This dialog let you choose noise levels
noiseTorelance=$(dialog --title 'Noise levels' --menu 'Now choose a noise level:' 0 0 0 \
0.1	silentPlace \
0.6	notSoSilentPlace \
1	noisyPlaceLevel01 \
1.5	noisyPlaceLevel02 \
2	veryNoisyPalce \
3	veryVeryNoisyPlace \
--stdout)

# This dialog asks if the sound comes from zippy or slow speakers
speechVel=$(dialog --title 'Speech velocity' --menu 'Choose:' 0 0 0 \
0.08	'very fast' \
0.1	fast \
0.2	regular \
0.3	slow \
--stdout)

# Separate audio using the silences within
sox $sourceFile res$randomNumber.wav silence -l 1 ${speechVel} ${noiseTorelance}% 1 ${speechVel} ${noiseTorelance}% : newfile : restart

# Stores the newly created files in an directory
mkdir dir$randomNumber
mv res$randomNumber* dir$randomNumber

# Interactive stores the files in chosen by user places
dialog --infobox "Separation has begun!" 5 40
for file in dir$randomNumber/*;
do
	play -q $file 2> /dev/null

	moveTo=$(dialog --title "Select a directory to this split" --dselect "$fileDestiny" 10 70 --stdout) 
	
	if [ "$moveTo" == "" ]
	then
		dialog --infobox "$file ignored" 5 40
	else
		dialog --infobox "Moving file $file to $moveTo" 5 40
		fileDestiny=$moveTo
		mkdir -p $moveTo

		# If the file already exists asks for overwrite or append or to hear it again 
		if [[ -f $moveTo/$(basename $sourceFile) ]]
		then
			# Emulates a do while loop
			over=3
			while [ "$over" == "3" ]
			do
				over=$(dialog --title 'File already exist!!!' --menu 'Choose an action:' 0 0 0 \
				1	Overwrite \
				2	Append \
				3	Hear \
				--stdout)

				# Plays the audio
				if [ "$over" == "3" ]
				then
					play -q $moveTo/$(basename $sourceFile) 2> /dev/null
				fi
			done

			# Overwrite file
			if [ "$over" == "1" ]
			then
				mv $file $moveTo/$(basename $sourceFile)
			fi

			# Append to file
			if [ "$over" == "2" ]
			then
				mv $moveTo/$(basename $sourceFile) $moveTo/temp.wav
				sox $moveTo/temp.wav $file $moveTo/$(basename $sourceFile)
				rm $moveTo/temp.wav
			fi
		else
			mv $file $moveTo/$(basename $sourceFile)
		fi
	fi
done

rm -rf dir$randomNumber	
clear
