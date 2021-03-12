#!/bin/bash

if ! command -v autoreconf &> /dev/null
then
    echo "autoreconf command not found. Please install autoconf pacakge"
    exit 1
fi

if ! command -v make &> /dev/null
then
    echo "make command not found. Please install build-essentials package"
    exit 1
fi

echo -n "Generating configure script... "
if autoreconf -i &> build.log; then
    echo "Done!"
else
    echo "Error running autoreconf. See build.log"
    exit 1
fi

echo -n "Generating Makefile... "
if ./configure &>> build.log; then
    echo "Done!"
else
    echo "Error running ./configure. See build.log"
    exit 1
fi

echo -n "Running make... "
if make &>> build.log; then
    echo "Done!"
else
    echo "Error running make. See build.log"
    exit 1
fi

echo "Build complete. Executable './ksh' generated"