reset

set output "graph.png"
set terminal png medium size 2048,1024
# set term png transparent truecolor

set grid


set multiplot layout 5,2 title "Digits" font "30"
set bmargin 0

unset key

unset label 1
unset label 2
unset label 3
unset label 4
unset label 5
unset label 6
unset label 7
unset label 8
unset label 9

unset xtics
unset ytics

set style line  1 lt  1 lw 2 pt 3 ps 0.5
set style line  2 lt  2 lw 2 pt 3 ps 0.5
set style line  3 lt  3 lw 2 pt 3 ps 0.5
set style line  4 lt  4 lw 2 pt 3 ps 0.5
set style line  5 lt  5 lw 2 pt 3 ps 0.5
set style line  6 lt  6 lw 2 pt 3 ps 0.5
set style line  7 lt  7 lw 2 pt 3 ps 0.5
set style line  8 lt  8 lw 2 pt 3 ps 0.5
set style line  9 lt  9 lw 2 pt 3 ps 0.5
set style line 10 lt 10 lw 2 pt 3 ps 0.5

set yrange [0:1]


set title "0"
plot "data2019-08-22.csv" using 1 with lines  ls 1,  "data2019-08-22.csv" using 2 with lines  ls 1,  "data2019-08-22.csv" using 3 with lines  ls 1,  "data2019-08-22.csv" using 4 with lines  ls 1,  "data2019-08-22.csv" using 5 with lines  ls 1, "data2019-08-22.csv" using 6 with lines   ls 1  

set title "1"                                                                                                                                                                                                                                                                                       
plot "data2019-08-22.csv" using 7 with lines  ls 2,  "data2019-08-22.csv" using 8 with lines  ls 2,  "data2019-08-22.csv" using 9 with lines  ls 2,  "data2019-08-22.csv" using 10 with lines ls 2,  "data2019-08-22.csv" using 11 with lines ls 2, "data2019-08-22.csv" using 12 with lines  ls 2  

set title "2"                                                                                                                                                                                                                                                                                       
plot "data2019-08-22.csv" using 13 with lines ls 3,  "data2019-08-22.csv" using 14 with lines ls 3,  "data2019-08-22.csv" using 15 with lines ls 3,  "data2019-08-22.csv" using 16 with lines ls 3,  "data2019-08-22.csv" using 17 with lines ls 3, "data2019-08-22.csv" using 18 with lines  ls 3  

set title "3"                                                                                                                                                                                                                                                                                       
plot "data2019-08-22.csv" using 19 with lines ls 4,  "data2019-08-22.csv" using 20 with lines ls 4,  "data2019-08-22.csv" using 21 with lines ls 4,  "data2019-08-22.csv" using 22 with lines ls 4,  "data2019-08-22.csv" using 23 with lines ls 4, "data2019-08-22.csv" using 24 with lines  ls 4  

set title "4"                                                                                                                                                                                                                                                                                       
plot "data2019-08-22.csv" using 25 with lines ls 5,  "data2019-08-22.csv" using 26 with lines ls 5,  "data2019-08-22.csv" using 27 with lines ls 5,  "data2019-08-22.csv" using 28 with lines ls 5,  "data2019-08-22.csv" using 29 with lines ls 5, "data2019-08-22.csv" using 30 with lines  ls 5  

set title "5"                                                                                                                                                                                                                                                                                       
plot "data2019-08-22.csv" using 31 with lines ls 6,  "data2019-08-22.csv" using 32 with lines ls 6,  "data2019-08-22.csv" using 33 with lines ls 6,  "data2019-08-22.csv" using 34 with lines ls 6,  "data2019-08-22.csv" using 35 with lines ls 6, "data2019-08-22.csv" using 36 with lines  ls 6  

set title "6"                                                                                                                                                                                                                                                                                       
plot "data2019-08-22.csv" using 37 with lines ls 7,  "data2019-08-22.csv" using 38 with lines ls 7,  "data2019-08-22.csv" using 39 with lines ls 7,  "data2019-08-22.csv" using 40 with lines ls 7,  "data2019-08-22.csv" using 41 with lines ls 7, "data2019-08-22.csv" using 42 with lines  ls 7  

set title "7"                                                                                                                                                                                                                                                                                       
plot "data2019-08-22.csv" using 43 with lines ls 8,  "data2019-08-22.csv" using 44 with lines ls 8,  "data2019-08-22.csv" using 45 with lines ls 8,  "data2019-08-22.csv" using 46 with lines ls 8,  "data2019-08-22.csv" using 47 with lines ls 8, "data2019-08-22.csv" using 48 with lines  ls 8  

set title "8"                                                                                                                                                                                                                                                                                       
plot "data2019-08-22.csv" using 49 with lines ls 9,  "data2019-08-22.csv" using 50 with lines ls 9,  "data2019-08-22.csv" using 51 with lines ls 9,  "data2019-08-22.csv" using 52 with lines ls 9,  "data2019-08-22.csv" using 53 with lines ls 9, "data2019-08-22.csv" using 54 with lines  ls 9  

set title "9"
plot "data2019-08-22.csv" using 55 with lines ls 10, "data2019-08-22.csv" using 56 with lines ls 10, "data2019-08-22.csv" using 57 with lines ls 10, "data2019-08-22.csv" using 58 with lines ls 10, "data2019-08-22.csv" using 59 with lines ls 10,"data2019-08-22.csv" using 60 with lines  ls 10 

# unset multiplot
