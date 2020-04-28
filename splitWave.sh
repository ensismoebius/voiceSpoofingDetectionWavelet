dialog --title "Welcome guys" --msgbox "This is the auto split script" 5 50

randomNumber=$(printf '%s\n' $(echo "scale=8; $RANDOM/32768" | bc ) | sed  's/\.//g')
historyFile="/tmp/waveSplitHistory"

# This dialog shows the more recent destiny choosen
content=$(cat -n $historyFile | tac)
t="dialog --title 'Shortcuts' --menu 'Choose a recent destiny directory - Press esc to skip' 0 0 0
${content}
--stdout"

# Execute the dialog and retrieve the option
option=$(eval $t)

# If no option was choosen the show the file browser 
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


# Separate audio using the silences within
sox $sourceFile res$randomNumber.wav silence -l 1 0.1 0.7% 1 0.1 0.7% : newfile : restart

# Stores the newly created files in an directory
mkdir dir$randomNumber
mv res$randomNumber* dir$randomNumber


# Interactive stores the files in choosen by user places
for file in dir$randomNumber/*;
do
	play $file

	moveTo=$(dialog --title "Select a directory to this split" --dselect "$fileDestiny" 10 70 --stdout) 
	
	if [ "$moveTo" == "" ]
	then
		echo "ignoring $file"
	else
		echo "Moving file $file to $moveTo"
		fileDestiny=$moveTo
		mkdir -p $moveTo
		mv $file $moveTo/$sourceFile
	fi
done

rm -rf dir$randomNumber	
