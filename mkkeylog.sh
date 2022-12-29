#!/bin/bash

if [[ $1 == '' ]]; then
    make -f mkkeylog.mk
elif [[ $1 == 'clean' ]]; then
    make -f mkkeylog.mk clean
fi
