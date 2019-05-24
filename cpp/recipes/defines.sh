#/bin/bash

g++ -posix -E -dM - < /dev/null >> defines.h
