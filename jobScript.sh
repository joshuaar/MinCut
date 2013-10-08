#!/bin/bash
#PBS -l nodes=1:ppn=12
#PBS -N mcPar
#PBS -j oe
#PBS -o $PBS_JOBID
#PBS -l walltime=0:30:00
module load openmpi/1.7.2-intel-13.0 

cd
$PBS_O_WORKDIR

export OMP_NUM_THREADS=12
mc=mcPar

cat runfiles/30_30.txt|./$(mc)
cat runfiles/50_50.txt|./$(mc)   
cat runfiles/70_70.txt|./$(mc)    
cat runfiles/90_90.txt|./$(mc)
cat runfiles/10_10.txt|./$(mc)     
cat runfiles/100_100.txt|./$(mc)
cat runfiles/20_20.txt|./$(mc)
cat runfiles/40_40.txt|./$(mc)
cat runfiles/60_60.txt|./$(mc)
cat runfiles/80_80.txt|./$(mc)

