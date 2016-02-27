#!/bin/bash
#
#
#

ARGUMENTS="-o output_file.dat input_file1.dat [ input_file2.dat ...]"
if [ $# -lt 3 ] || [ $1 = "help" ] || [ $1 = "h" ] || [ $1 = "-h" ]
then
    echo "Arguments: ${ARGUMENTS}"
    echo "This script collates the rows of the input files and, under the assumption that the headers are all the same, uses the header of one of the files.  The script will ask you to confirm if the output file exists."
    exit
fi

output="$2"
outputflag="$1"
shift
shift
files="$@"

#echo "$output"
#echo "$outputflag"
#echo "$files"

if [ ${outputflag} != "-o" ] 
then
    echo "Call script with the arguments ${ARGUMENTS}"
else
    if [ -e $output ]
    then
	echo "Are you sure you want to overwrite the file ${output}? RET/ CTRL-c|n|N|q|Q"
	read response
	if [ "$response" = "n" ] || [ "$response" = "N" ] || [ "$response" = "q" ] || [ "$response" = "Q" ] 
	then
	    exit
	fi
    fi
    head -1 "$3" > ${output}
    #    echo ${files[0]}
    echo "Files included:"
    for file in ${files} 
    do
	echo "$file"
	tail -1 ${file} >> ${output}
    done
    echo "Output:"
    cat $output
fi
