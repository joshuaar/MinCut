###Installation
the make command creates mcPar and mcSer, which take graphs piped as input. See runfiles/ for graph formatting

    make

###Usage
See example script jobScript.sh or run the snippet below from the project directory:

    export OMP_NUM_THREADS=12
    cat runfiles/30_30.txt | ./mcPar
    cat runfiles/50_50.txt | ./mcPar
    cat runfiles/70_70.txt | ./mcPar 
