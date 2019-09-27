#!/bin/bash
LEN=${#1}

for (( c=1; c <= 50; c++ )) do
    COUNT=0
    while :
    do
        COUNT=$[COUNT+1]
        IN=$(head -c 40 /dev/urandom | base64)
        #OUT=$(echo "$IN" | ./wrapper.sh $LEN)
        # inlining took this from 28 hashes/sec to 170.5 hashes/sec
        OUT=$(echo "$IN" | base64 -d | sha1sum -b | cut -f 1 -d' ' | head -c $LEN)
        if [ "$1" == "$OUT" ]; then
            echo "$c": "$COUNT" "$IN" "$OUT"
            break
        fi
    done
done
