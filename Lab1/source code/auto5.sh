#!/bin/bash

# compiles the sourcecode to executable
gcc -lpthread -o inlinetest main.c -std=c99 -finline-functions

destdir="result.txt"

echo -n "Enter the number of thread "
read t
echo "You entered: $t"

for i in 1 2 3 4 5
    do
        echo "Welcome $i times"
        # captures the time only, passes stdout through
        exec 3>&1 4>&2
        time=$(TIMEFORMAT="%R"; { time ./inlinetest 5  $t cities5.txt 1>&3 2>&4; } 2>&1)
        exec 3>&- 4>&-
        echo $time
        if [ -f "$destdir" ]
            then
            echo "$time" >> "$destdir"
        fi
    done

SUM=0
for i in `cat result.txt`
do SUM=$(echo "$SUM + $i"|bc)
done
echo $SUM