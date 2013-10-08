CC=g++
mcPar: MinCut.cpp mcSer
	$(CC) MinCut.cpp -fopenmp -o mcPar
mcSer: MinCutSer.cpp
	$(CC) MinCutSer.cpp -fopenmp -o mcSer
