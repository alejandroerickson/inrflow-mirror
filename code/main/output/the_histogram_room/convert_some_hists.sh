#!/bin/bash


KEY=$1
NS=$2
BINS=$3
if [ $# -eq 4 ]
then
    MAXBAR=$4
else
    MAXBAR="50"
fi

>&2 cat<<EOF

This script takes 3-4 arguments.  key 'n|s' nbins|bin_size [maxbar].  You have entered
$KEY $BINS $MAXBAR

Generate binned histograms from files *hist.dat with the key ${KEY},
create files *hist.dat.${KEY}.  ASCII histograms are output to STDOUT .

You must have scripts extract_hist.sh and histogram-tools.py in this
directory.

EOF

if [ $# -ne 3 ] && [ $# -ne 4 ]
then
    >&2 echo ERROR: wrong number of arguments.
    exit
fi

>&2 echo "Continue? RET / n | N | q | Q"

read response
if [ "$response" = "n" ] || [ "$response" = "N" ] || [ "$response" = "q" ] || [ "$response" = "Q" ] 
then
    exit
fi

for A in *hist.dat; do
    ./extract_hist.sh "${A}" $1 > hist.tmp
    echo "${A}.${KEY}"
    python histogram-tools.py hist.tmp "${NS}" "${BINS}" "graphical" "${MAXBAR}"
    python histogram-tools.py hist.tmp "${NS}" "${BINS}" "numeric" > "${A}.${KEY}"
done
