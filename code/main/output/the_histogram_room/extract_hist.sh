#!/bin/bash

# Extract from file '$1' the histogram with headers '$2', and strip out comments

if [ $# -ne 2 ]; then
		>&2 echo Expect 2 arguments: file and key "(e.g., h|p|f|r|s|d)"
		exit
fi

KEY=$2
COUNT=1
LINE_COUNT=0
FLAG="0"
while read A; do
		((LINE_COUNT++))
		if (echo ${A} | egrep "^ *h" > /dev/null); then
				for hk in ${A}; do
						if [ "${hk}" =  "${KEY}i" ]; then
								FLAG="1"
								break
						else
								((COUNT++))
						fi
				done
				break
		fi
done < $1

if [ $FLAG = "0" ]
then
		>&2 echo ERROR: the key ${KEY} is not present
		exit
fi

((COUNT++))
WC=`wc -l < $1`
HIST_LINES=$(( WC - LINE_COUNT))
tail -$HIST_LINES $1 | tr -s ' ' | cut -d ' ' -f ${COUNT}-$((COUNT + 1)) | grep -v nan
