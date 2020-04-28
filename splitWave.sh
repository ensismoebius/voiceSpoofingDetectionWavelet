dialog --title "Welcome guys" --msgbox "This is the auto split script" 5 50
randomNumber=$(printf '%s\n' $(echo "scale=8; $RANDOM/32768" | bc ) | sed  's/\.//g')

historyFile="/tmp/waveSplitHistory"

# Selection dialog on which user must choose where to save the files
directory=$(dialog --title "Select a directory to save splited waves" --dselect "$fileDestinyHist0" 10 70 --stdout) 
echo $directory >> $historyFile


sourceFile=$(dialog --title "Select the source file" --fselect "$PWD" 10 70 --stdout)


echo "$directory"

#sourceFile="/tmp/Voz9.wav"
#
#sox $sourceFile res$randomNumber.wav silence -l 1 0.1 0.7% 1 0.1 0.7% : newfile : restart
#
#mkdir dir$randomNumber
#mv res$randomNumber* dir$randomNumber
#
#for file in dir$randomNumber/*;
#do
#	play $file
#	echo $file
#done
#
#rm -rf dir$randomNumber
	
