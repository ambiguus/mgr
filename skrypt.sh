#!/bin/bash
for ((i=25; $i <= 200; i+=10)) ; do
        echo "$i"
        ./main $i >> ranking.txt
done

