#!/bin/sh
touch output.txt;
rm output.txt;
touch output.txt;
for f in data?.txt;
    do echo '\n'~~~$f~~~ >> output.txt && ./a2 -s unix,fcfs,sjf,srtf,rr -i $f >> output.txt;
done
cat output.txt