#!/bin/bash
for ((i=20; $i <= 100; i+=5)) ; do
        echo "$i"
        ./main $i similarity$i 
done

