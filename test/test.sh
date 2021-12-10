#!/bin/bash
make >> /dev/null 2>> /dev/null
echo "LEN,TIME" > test/data.csv
N=10
for file in $(ls input); do
    echo $file
    OUTPUT=$(./cnf input/$file | grep -E "(TOTALNPARSE|LEN)")
    for iter in $(seq $N); do
        OUTPUT=$(./cnf input/$file | grep -E "(TOTALNPARSE|LEN)")
        echo -n $OUTPUT | grep "LEN" | cut -d " " -f4- | tr -d '\n' >> test/data.csv
        echo -n "," >> test/data.csv
        echo "${OUTPUT}" | grep "TOTALNPARSE" | cut -d " " -f2 >> test/data.csv
    done
done

