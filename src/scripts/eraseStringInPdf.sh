for file in $(ls | grep .pdf);
do
	pdftk $file output u_$file uncompress;
	rm $file;
	sed -i 's/$1//g' u_$file; 
	pdftk u_$file output $file compress;
	rm u_$file;
done

