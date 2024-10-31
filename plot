#!/bin/bash

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <minimum temperature> <maximum temperature>"
    exit 1
fi

if [ "$(ls -A results)" ]; then
    rm -v results/*
fi

root -l -q 'newHistData.C("'$1'", "'$2'")'