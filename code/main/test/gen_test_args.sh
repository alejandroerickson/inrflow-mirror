#!/bin/bash

#author: Alejandro Erickson
#
# Use this file only for sanity checks.  If you want to generate a .args file use gen_args.sh
#
# generates one inrflow job per line.  toggle topologies, patterns,
# and pfailures below.  pipe jobs to ./gnu_parallel_test.sh or
# ./parallel_test.sh (not robust, but works without gnu parallel) or
# ./serial_test.sh

		#SWCUBE_FLAG=false
		SWCUBE_FLAG=true

		#BCUBE_FLAG=false
		BCUBE_FLAG=true

		#FICONN_FLAG=false
		FICONN_FLAG=true

		#GDCFICONN_FLAG=false
		GDCFICONN_FLAG=true

		#GDCFICONNL2_FLAG=false
		GDCFICONNL2_FLAG=true

		#GDCFICONNL3_FLAG=false
		GDCFICONNL3_FLAG=true

		#FATTREE_FLAG=false
		FATTREE_FLAG=true

		#KNKSTAR_FLAG=false
		KNKSTAR_FLAG=true

		#DPILLAR_FLAG=false
		DPILLAR_FLAG=true

		#JELLYFISH_FLAG=false
		JELLYFISH_FLAG=true

		#TORUS_FLAG=false
		TORUS_FLAG=true

		#THINTREE_FLAG=false
		THINTREE_FLAG=true

		PATTERNS="random_10000"
		PFAILURES="0.0 0.2"

if [ $SWCUBE_FLAG = true ]
then
		SWCUBE="swcube_3_4 swcube_4_3"
else
		SWCUBE=""
fi
for T in ${SWCUBE}
do
		for F in ${PFAILURES}
		do
				for P in ${PATTERNS}
				do
						echo "topo=${T} failure_rate=${F} tpattern=${P} generate_bfs=false compile_latex=true placement=sequential"
				done
		done
done

if [ $BCUBE_FLAG = true ]
then
		BCUBE="bcube_2_8 bcube_3_4"
else
		BCUBE=""
fi
for T in ${BCUBE}
do
		for F in ${PFAILURES}
		do
				for P in ${PATTERNS}
				do
						echo "topo=${T} failure_rate=${F} tpattern=${P} generate_bfs=false compile_latex=false placement=sequential"
						
				done
		done
done

if [ $FICONN_FLAG = true ]
then
		FICONN="ficonn_2_8 ficonn_3_6"
else
		FICONN=""
fi
for T in ${FICONN}
do
		for F in ${PFAILURES}
		do
				for P in ${PATTERNS}
				do
						echo "topo=${T} failure_rate=${F} tpattern=${P} generate_bfs=false compile_latex=false placement=sequential"
				done
		done
done

if [ $GDCFICONNL3_FLAG = true ]
then
		GDCFICONNL3="gdcficonn_3_4_2 gdcficonn_3_4_0 gdcficonn_3_4_1"
else
		GDCFICONNL3=""
fi
GDCFICONNL3_ROUTING="gdcficonn-proxy_0_2_3 gdcficonn-proxy_1_2_3"

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
    GDCFICONNL2="gdcficonn_2_8_0 gdcficonn_2_8_1 gdcficonn_2_10_2 gdcficonn_2_7_0 gdcficonn_2_7_1 gdcficonn_2_8_2"
else
		GDCFICONNL2=""
fi
GDCFICONNL2_ROUTING="gdcficonn-proxy_0_2 gdcficonn-proxy_1_2"

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

if [ $GDCFICONN_FLAG = true ]
then
    GDCFICONN="gdcficonn_0_20_0 gdcficonn_0_20_1 gdcficonn_0_20_2 gdcficonn_1_20_0 gdcficonn_1_20_1 gdcficonn_1_20_2 gdcficonn_2_4_0 gdcficonn_2_4_1 gdcficonn_2_4_2 gdcficonn_2_8_0 gdcficonn_2_8_1 gdcficonn_2_8_2 gdcficonn_3_4_0 gdcficonn_3_4_1 gdcficonn_3_6_2"
else
		GDCFICONN=""
fi
GDCFICONN_ROUTING="gdcficonn-dim gdcficonn-proxy_0"

for T in ${GDCFICONN}
do
		for R in ${GDCFICONN_ROUTING}
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

if [ $FATTREE_FLAG = true ]
then
    FATTREE="fattree_4_3"
else
    FATTREE=""
fi
for T in ${FATTREE}
do
    for F in ${PFAILURES}
    do
				for P in ${PATTERNS}
				do
								echo "topo=${T} failure_rate=${F} tpattern=${P} generate_bfs=false compile_latex=false placement=sequential"
						
				done
    done
done

if [ $KNKSTAR_FLAG = true ]
then
    KNKSTAR="knkstar_2_5_1 knkstar_4_4_2" 
else
    KNKSTAR=""
fi
for T in ${KNKSTAR}
do
    for F in ${PFAILURES}
    do
				for P in ${PATTERNS}
				do
						echo "topo=${T} failure_rate=${F} tpattern=${P} generate_bfs=false compile_latex=false placement=sequential"
						
			done
    done
done


if [ $JELLYFISH_FLAG = true ]
then
    JELLYFISH="jellyfish_64_8_4_13"
else
    JELLYFISH=""
fi
JELLYFISH_ROUTING="jellyfish-sp"
for T in ${JELLYFISH}
do
    for R in ${JELLYFISH_ROUTING}
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


if [ $DPILLAR_FLAG = true ]
then
    DPILLAR="dpillar_6_3 dpillar_6_4"
else
    DPILLAR=""
fi
DPILLAR_ROUTING="dpillar-sp dpillar-sp-rnd dpillar-sp-shd dpillar-mp dpillar-mp-rnd dpillar-mp-shd dpillar-min"
for T in ${DPILLAR}
do
    for R in ${DPILLAR_ROUTING}
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



if [ $TORUS_FLAG = true ]
then
    TORUS="torus_4_4 torus_100"
else
    TORUS=""
fi
for T in ${TORUS}
do
				for F in ${PFAILURES}
				do
						for P in ${PATTERNS}
						do
								echo "topo=${T} failure_rate=${F} tpattern=${P} generate_bfs=false compile_latex=false placement=sequential"
						done
				done
done

if [ $THINTREE_FLAG = true ]
then
    THINTREE="thintree_4_2_5"
else
    THINTREE=""
fi
for T in ${THINTREE}
do
				for F in ${PFAILURES}
				do
						for P in ${PATTERNS}
						do
								echo "topo=${T} failure_rate=${F} tpattern=${P} generate_bfs=false compile_latex=false placement=sequential"
						done
				done
done
