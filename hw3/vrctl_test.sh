#!/bin/bash

mps=$(ps -aef | grep ./test)
IFS='
'
mps=($mps)

grepres=${mps[0]}
IFS='      '
grepres=($grepres)

pid=${grepres[1]}

echo $pid


while :
do
	./vrctl/vrctl $pid 1
	./vrctl/vrctl $pid
	sleep 5
	./vrctl/vrctl $pid 2
	./vrctl/vrctl $pid
	sleep 5
	./vrctl/vrctl
done
