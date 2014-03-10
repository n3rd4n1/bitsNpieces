#!/bin/bash

set -e

DEVTOOLS=( "gcc:gcc"
           "g++:g++"
           "curl:curl"
           "ncurses:libncurses5-dev"
           "makeinfo:texinfo"
           "unzip:unzip"
           "tar:tar"
           "gzip:gzip"
           "bzip2:bzip2"
           "make:make"
           "perl:perl"
           "m4:m4"
           "flex:flex"
           "bison:bison" )

sudo cat /dev/null

for devtool in "${DEVTOOLS[@]}"
do
    bin="${devtool%%:*}"
    pkg="${devtool##*:}"

    if [ "`whereis $bin`" == "$bin:" ]
    then
        echo ">>> $bin: apt-get install $pkg"
        sudo apt-get -y install $pkg > /dev/null
    fi
done

