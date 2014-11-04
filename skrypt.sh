#!/bin/bash
for ((i=2; $i <= 150; i+=1)) ; do
        echo "$i"
        ./main $i >> ranking.txt
done

