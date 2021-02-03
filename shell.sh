#!/bin/sh
make compile
xterm -title "App 1" -hold -e ./a.out test.txt  &
xterm -title "App 2" -hold -e ./a.out test.txt
