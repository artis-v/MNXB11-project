#!/bin/bash

if [ "$#" -lt 2 ] || [ "$#" -gt 4 ]; then
    echo "Usage: $0 <minimum temperature> <maximum temperature> [<city> <time>]"
    exit 1
fi

if [ "$(ls -A results)" ]; then
    rm -v results/*
fi

CITY="datasets/Boras_Data_Cleaned.csv"
CITYNAME="Boras"
TIME="18:00:00"

if [ "$#" -ge 3 ]; then
    CITY="datasets/"$3"_Data_Cleaned.csv"
    CITYNAME=$3
fi

if ! [ -e $CITY ]; then
    echo "Error: $CITY not found."
    exit 1
fi

if [ "$#" -ge 4 ]; then
    TIME=$4
fi

root -l -q 'newHistData.C("'$1'", "'$2'", "'$CITY'", "'$TIME'", "'$CITYNAME'")'