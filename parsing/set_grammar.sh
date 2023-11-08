#!/bin/bash
GRAMMAR=${1}

filename="$(basename -- "$GRAMMAR")"
name=${filename%.*}
extension="${filename##*.}"

if [ "$extension" != "ll1" ]; then
    echo "Expecting filename with ll1 extension"
    exit 1
fi

if [ ! -f ${GRAMMAR} ]; then
   echo Grammar file not found: ${GRAMMAR}
   exit 1
fi 

if [ ! -f lexer_${name}.l ]; then
   echo Lex file not found: lexer_${name}.l
   exit 1
fi 

rm -f lexer.l

ln -s lexer_${name}.l lexer.l
export GRAMMAR=${name}

make clean
make all
