#!/bin/bash
for ((i=5; $i <= 100; i+=5)) ; do
        echo "$i"
        ./main $i >> ranking_euklides.txt
done

