#!/bin/bash
for ((i=2; $i <= 30; i+=1)) ; do
        echo "$i"
        ./main $i >> ranking_euklides_do30.txt
done

