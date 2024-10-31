#!/bin/bash

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <minimum temperature> <maximum temperature>"
    exit 1
fi

rm -v results/*
root -l -q 'newHistData.C("'$1'", "'$2'")'