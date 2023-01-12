#!/bin/bash

if [[ $1 == '' ]]; then
    make -f mkkeylog.mk
    echo "If you are prompted for sudo password, enter it. This should ideally be only asked once when the compilation is happening"
    sudo chown root:root keylog
    sudo chmod u+s keylog
elif [[ $1 == 'clean' ]]; then
    make -f mkkeylog.mk clean
fi
