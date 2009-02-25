#!/bin/sh
touch r.txt;
rm r.txt;
touch r.txt;
for f in ./data/i?.txt;
    do echo ~~~$f~~~ >> r.txt && ./a2 -s unix,fcfs,sjf,srtf,rr -i $f >> r.txt;
done
cat r.txt