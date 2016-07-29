#!/bin/bash

ARGUMENTS="output_file.dat input_file.dat key1 [key2 key3...]"

if [ $# -le 2 ] || [ $1 = "help" ] || [ $1 = "h" ] || [ $1 = "-h" ]
then
    echo "Arguments: ${ARGUMENTS}"
    echo "This script sorts input_file.dat on the given 'key', chosen from the input file's header, and writes the output to output_file.dat"
    exit
fi


#echo "$@"
#echo "$#"
output="$1"
input="$2"
shift 2
#echo "$@"
#echo "$#"

TMP="_tmp"

#echo $output
#echo $input

SORTCOMMAND="sort -n"
header=`head -1 ${input}`
echo $header > ${TMP}
for i in "$@"
do
    KEY="$i"
    shift
    echo "$KEY"
    COUNT=1
    for header_key in ${header}
    do
	if [ "${header_key}" = "${KEY}" ]
	then
	    break
	else
	    let COUNT=COUNT+1
	fi
    done
    SORTCOMMAND="${SORTCOMMAND} -nk${COUNT},${COUNT}"
done
echo ${SORTCOMMAND}
SORTCOMMAND="(tail -n +2 ${input} | ${SORTCOMMAND}) >> ${TMP}"
echo "Sorting with command: ${SORTCOMMAND}"
eval ${SORTCOMMAND}
cat ${TMP} > ${output}
echo "Output:"
cat ${output}
