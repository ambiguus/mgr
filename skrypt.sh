#!/bin/bash
for ((i=1; $i <= 30; i+=1)) ; do
        echo "$i"
        ./main $i similarity$i 
done

