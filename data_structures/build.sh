#!/usr/local/bin/bash

set -ev

rm -rf bin/ test/
mkdir bin/
mkdir test/
python compute_makefile.py
make
