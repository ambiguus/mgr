#!/bin/bash
for ((i=2; $i <= 4; i+=1)) ; do
        echo "$i" >> promien
        ./main aaa $i >> promien
done

