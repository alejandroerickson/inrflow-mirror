#!/bin/bash

#author: Alejandro Erickson
#
# generates one inrflow job per line.  toggle topologies, patterns,
# and pfailures below.  pipe jobs to ./gnu_parallel_test.sh or
# ./parallel_test.sh (not robust, but works without gnu parallel) or
# ./serial_test.sh

# if

# fi

# while getopts ":a:" opt; do
# 		case $opt in
# 				a)

# done

#GDCFICONNL2_FLAG=false
GDCFICONNL2_FLAG=true

#GDCFICONNL3_FLAG=false
GDCFICONNL3_FLAG=true

PATTERNS="alltoall" ###" manyall2all_50 manyall2allrnd_50 random_100 hotregion_100 butterfly one2all"
PFAILURES="0.0 0.1"

if [ $GDCFICONNL3_FLAG = true ]
then
		GDCFICONNL3="gdcficonn_3_4_2 gdcficonn_3_4_0 gdcficonn_3_4_1 gdcficonn_3_10_2"
else
		GDCFICONNL3=""
fi
GDCFICONNL3_ROUTING="gdcficonn-dim gdcficonn-proxy_0_2 gdcficonn-proxy_0_3 gdcficonn-proxy_0_2_3 gdcficonn-proxy_1_2 gdcficonn-proxy_1_3 gdcficonn-proxy_1_2_3"

for T in ${GDCFICONNL3}
do
		for R in ${GDCFICONNL3_ROUTING}
		do
				for F in ${PFAILURES}
				do
						for P in ${PATTERNS}
						do
								echo "topo=${T} failure_rate=${F} routing=${R} tpattern=${P} generate_bfs=false compile_latex=false placement=sequential"
								
						done
				done
		done
done

if [ $GDCFICONNL2_FLAG = true ]
then
    GDCFICONNL2="gdcficonn_2_18_0 gdcficonn_2_18_1 gdcficonn_2_36_2"
else
		GDCFICONNL2=""
fi
GDCFICONNL2_ROUTING="gdcficonn-dim gdcficonn-proxy_0_2 gdcficonn-proxy_1_2"

for T in ${GDCFICONNL2}
do
		for R in ${GDCFICONNL2_ROUTING}
		do
				for F in ${PFAILURES}
				do
						for P in ${PATTERNS}
						do
								echo "topo=${T} failure_rate=${F} routing=${R} tpattern=${P} generate_bfs=false compile_latex=false placement=sequential"
								
						done
				done
		done
done
