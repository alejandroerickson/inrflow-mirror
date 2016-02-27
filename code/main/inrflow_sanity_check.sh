#!/bin/sh

echo "This script will clean your build directory,"
echo "compile INRFlow with the DEBUG flag, and"
echo "run some sanity checks.  Select topologies to test"
echo "by setting flags in this script."
echo "Do you wish to continue? RET / CTRL-c|n|N|q|Q"
read response
if [ "$response" = "n" ] || [ "$response" = "N" ] || [ "$response" = "q" ] || [ "$response" = "Q" ] 
then
    exit
fi

make clean
make all CFLAGS=-DDEBUG

#SWCUBE_FLAG=false
SWCUBE_FLAG=true

#BCUBE_FLAG=false
BCUBE_FLAG=true

#FICONN_FLAG=false
FICONN_FLAG=true

#FATTREE_FLAG=false
FATTREE_FLAG=true

#KNKSTAR_FLAG=false
KNKSTAR_FLAG=true

#DPILLAR_FLAG=false
DPILLAR_FLAG=true

#JELLYFISH_FLAG=false
JELLYFISH_FLAG=true

if [ $SWCUBE_FLAG = true ]
then
    SWCUBE="swcube_3_4 swcube_4_3"
else
    SWCUBE=""
fi

if [ $DPILLAR_FLAG = true ]
then
    DPILLAR="dpillar_6_3 dpillar_6_4"
else
    DPILLAR=""
fi

if [ $BCUBE_FLAG = true ]
then
    BCUBE="bcube_2_8 bcube_3_4"
else
    BCUBE=""
fi
if [ $FICONN_FLAG = true ]
then
    FICONN="ficonn_2_4 ficonn_3_6"
else
    FICONN=""
fi
if [ $FATTREE_FLAG = true ]
then
    FATTREE="fattree_4_3"
else
    FATTREE=""
fi
if [ $KNKSTAR_FLAG = true ]
then
    KNKSTAR="knkstar_2_5_1 knkstar_4_4_2" 
else
    KNKSTAR=""
fi
if [ $JELLYFISH_FLAG = true ]
then
    JELLYFISH="jellyfish_64_8_4_13"
else
    JELLYFISH=""
fi

DPILLAR_ROUTING="dpillar-sp dpillar-sp-rnd dpillar-sp-shd dpillar-mp dpillar-mp-rnd dpillar-mp-shd dpillar-min"

JELLYFISH_ROUTING="jellyfish-sp"

PATTERNS="alltoall manyall2all_50 manyall2allrnd_50 random_100 hotregion_100 butterfly one2all"
PFAILURES="0.0 0.3"

for T in ${SWCUBE}
do
    for F in ${PFAILURES}
    do
				for P in ${PATTERNS}
				do
						./build/bin/inrflow topo=${T} failure_rate=${F}  tpattern=${P} generate_bfs=false compile_latex=false
						if [ "$?" != "0" ]
						then
								echo "INRFlow bad exit code for parameters:"
								echo "topo=${T} failure_rate=${F} tpattern=${P} generate_bfs=false"
								echo "Do you wish to continue? RET / CTRL-c|n|N|q|Q"
								read response	
								if [ "$response" = "n" ] || [ "$response" = "N" ] || [ "$response" = "q" ] || [ "$response" = "Q" ] 
								then
										exit
								fi
						fi
				done
		done
done

for T in ${BCUBE}
do
    for F in ${PFAILURES}
    do
				for P in ${PATTERNS}
				do
						./build/bin/inrflow topo=${T} failure_rate=${F}  tpattern=${P} generate_bfs=false compile_latex=false
						if [ "$?" != "0" ]
						then
								echo "INRFlow bad exit code for parameters:"
								echo "topo=${T} failure_rate=${F} tpattern=${P} generate_bfs=false"
								echo "Do you wish to continue? RET / CTRL-c|n|N|q|Q"
								read response	
								if [ "$response" = "n" ] || [ "$response" = "N" ] || [ "$response" = "q" ] || [ "$response" = "Q" ] 
								then
										exit
								fi
						fi
				done
    done
done

for T in ${FICONN}
do
    for F in ${PFAILURES}
    do
				for P in ${PATTERNS}
				do
						./build/bin/inrflow topo=${T} failure_rate=${F}  tpattern=${P} generate_bfs=false compile_latex=false
						if [ "$?" != "0" ]
						then
								echo "INRFlow bad exit code for parameters:"
								echo "topo=${T} failure_rate=${F} tpattern=${P} generate_bfs=false"
								echo "Do you wish to continue? RET / CTRL-c|n|N|q|Q"
								read response	
								if [ "$response" = "n" ] || [ "$response" = "N" ] || [ "$response" = "q" ] || [ "$response" = "Q" ] 
								then
										exit
								fi
						fi
				done
    done
done

for T in ${FATTREE}
do
    for F in ${PFAILURES}
    do
				for P in ${PATTERNS}
				do
						./build/bin/inrflow topo=${T} failure_rate=${F}  tpattern=${P} generate_bfs=false compile_latex=false
						if [ "$?" != "0" ]
						then
								echo "INRFlow bad exit code for parameters:"
								echo "topo=${T} failure_rate=${F} tpattern=${P} generate_bfs=false"
								echo "Do you wish to continue? RET / CTRL-c|n|N|q|Q"
								read response	
								if [ "$response" = "n" ] || [ "$response" = "N" ] || [ "$response" = "q" ] || [ "$response" = "Q" ] 
								then
										exit
								fi
						fi
				done
    done
done

for T in ${KNKSTAR}
do
    for F in ${PFAILURES}
    do
				for P in ${PATTERNS}
				do
						./build/bin/inrflow topo=${T} failure_rate=${F}  tpattern=${P} generate_bfs=false compile_latex=false
						if [ "$?" != "0" ]
						then
								echo "INRFlow bad exit code for parameters:"
								echo "topo=${T} failure_rate=${F} tpattern=${P} generate_bfs=false"
								echo "Do you wish to continue? RET / CTRL-c|n|N|q|Q"
								read response	
								if [ "$response" = "n" ] || [ "$response" = "N" ] || [ "$response" = "q" ] || [ "$response" = "Q" ] 
								then
										exit
								fi
						fi
				done
    done
done


for T in ${JELLYFISH}
do
    for R in ${JELLYFISH_ROUTING}
    do
				for F in ${PFAILURES}
				do
						for P in ${PATTERNS}
						do
								./build/bin/inrflow topo=${T} routing=${R} failure_rate=${F}  tpattern=${P} generate_bfs=false compile_latex=false
								if [ "$?" != "0" ]
								then
										echo "INRFlow bad exit code for parameters:"
										echo "topo=${T} routing=${R} failure_rate=${F} tpattern=${P} generate_bfs=false"
										echo "Do you wish to continue? RET / CTRL-c|n|N|q|Q"
										read response	
										if [ "$response" = "n" ] || [ "$response" = "N" ] || [ "$response" = "q" ] || [ "$response" = "Q" ] 
										then
												exit
										fi
								fi
						done
				done
    done
done


for T in ${DPILLAR}
do
    for R in ${DPILLAR_ROUTING}
    do
				for F in ${PFAILURES}
				do
						for P in ${PATTERNS}
						do
								./build/bin/inrflow topo=${T} routing=${R} failure_rate=${F}  tpattern=${P} generate_bfs=false compile_latex=false
								if [ "$?" != "0" ]
								then
										echo "INRFlow bad exit code for parameters:"
										echo "topo=${T} routing=${R} failure_rate=${F} tpattern=${P} generate_bfs=false"
										echo "Do you wish to continue? RET / CTRL-c|n|N|q|Q"
										read response	
										if [ "$response" = "n" ] || [ "$response" = "N" ] || [ "$response" = "q" ] || [ "$response" = "Q" ] 
										then
												exit
										fi
								fi
						done
				done
    done
done


