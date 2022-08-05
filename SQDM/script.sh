#!/bin/bash
for i in {1..10..1}
do
./sqdm.x 16 10000
echo $i
done
