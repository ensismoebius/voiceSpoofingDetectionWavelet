for csv in $(ls *.csv)
do 
	lines=$(cat $csv | sed 's/[Best|Worst].*//g' | tr '\n' ' ' | sed 's/[[:blank:]]/ /g')

	echo $lines | while read matrices; do
		BTP=$(echo $matrices | cut -d' ' -f1)
		BFP=$(echo $matrices | cut -d' ' -f2)
		BFN=$(echo $matrices | cut -d' ' -f3)
		BTN=$(echo $matrices | cut -d' ' -f4)

		WTP=$(echo $matrices | cut -d' ' -f5)
		WFP=$(echo $matrices | cut -d' ' -f6)
		WFN=$(echo $matrices | cut -d' ' -f7)
		WTN=$(echo $matrices | cut -d' ' -f8)

name=$(echo $csv | sed 's/classifier//g' | sed 's/Euclidian/Euclidiano/g' | sed 's/\.csv//g' | sed 's/_/ /g')
filename=$(echo $csv | sed 's/\.csv//g')
cat << EOF | sed "s/__TP__/$BTP/g" | sed "s/__FP__/$BFP/g" | sed "s/__FN__/$BFN/g" | sed "s/__TN__/$BTN/g" > $filename.tex
\begin{table}
\newcommand{\mc}[3]{\multicolumn{#1}{#2}{#3}}
\definecolor{tcB}{rgb}{0.447059,0.74902,0.266667}
\definecolor{tcC}{rgb}{0,0,0}
\definecolor{tcD}{rgb}{0,0.4,0.701961}
\definecolor{tcA}{rgb}{0.65098,0.65098,0.65098}
\begin{center}
	\begin{tabular}{ccc}
		% use packages: color,colortbl
		\mc{1}{l}{} & \mc{1}{>{\columncolor{tcA}}c}{\textbf{Verdadeiro}} & \mc{1}{>{\columncolor{tcA}}c}{\textbf{Falso}}\\\\

		\mc{1}{>{\columncolor{tcA}}r}{\textbf{Verdadeiro}} & \mc{1}{>{\columncolor{tcB}}c}{\textcolor{tcC}{__TP__}} & \mc{1}{>{\columncolor{tcD}}c}{\textcolor{tcC}{__FP__}}\\\\

		\mc{1}{>{\columncolor{tcA}}r}{\textbf{Falso}} & \mc{1}{>{\columncolor{tcD}}c}{\textcolor{tcC}{__FN__}} & \mc{1}{>{\columncolor{tcB}}c}{\textcolor{tcC}{__TN__}}
	\end{tabular}
	\caption{Tabela de confusão para classificador$name\\%}
	\label{tab:$filename}
\end{center}
\end{table}
EOF
	done

done
#	true	False
#true	TP	FP
#false	FN	TN

