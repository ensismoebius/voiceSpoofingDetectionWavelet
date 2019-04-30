/*
 * gnuplotCalls.cpp
 *
 *  Created on: 30 de abr de 2019
 *      Author: ensis
 */

#include <stdlib.h>
#include <string.h>
#include <cstdio>
#include <iostream>

void showInParaconsistentPlane(char textMode, double certaintyDegree_G1, double contradictionDegree_G2) {
	char mode[19] = "";
	char buffer[1000] = "";

	if (textMode) {
		strcpy(mode, "set terminal dumb;");
	} else {
		strcpy(mode, "");
	}
	sprintf(buffer, "gnuplot -p -e \"%s set border linewidth 1.5; unset tics; unset border; set label 1 'Falsehood' at -1,0; set label 2 'Truth' at 1,0; set label 3 'Ambiguity' at 0,1; set label 4 'Indefinition' at 0,-1; set xzeroaxis; set yzeroaxis; set size ratio 1; set nokey; unset border; set object circle at %f,%f size .02; plot [-1:1][-1:1] x+1,x-1,1-x,-1-x\"", mode, certaintyDegree_G1, contradictionDegree_G2);
	std::cout << buffer << std::endl;
	system(buffer);
}

