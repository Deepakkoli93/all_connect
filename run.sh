#!/bin/bash
chmod 777 compile.sh
./compile.sh
./client $1 $2
