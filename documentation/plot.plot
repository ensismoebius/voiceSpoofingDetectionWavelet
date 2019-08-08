set style line 1 linecolor rgb '#ff0000' linetype 1 linewidth 2 pointtype 7 pointsize 1.5
set style line 2 linecolor rgb '#00ff00' linetype 2 linewidth 2  pointtype 7 pointsize 1.5
set style line 3 linecolor rgb '#0000ff' linetype 3 linewidth 2  pointtype 7 pointsize 1.5

plot "firstTestData.csv" using 2 title "Voz20" with linespoints linestyle 1, \
"firstTestData.csv" using 3 title "Voz05" with linespoints linestyle 2, \
"firstTestData.csv" using 4 title "Voz01" with linespoints linestyle 3
