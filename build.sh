#!/bin/sh

gcc -o ./bin/server ./src/server.c ./md5_src/md5c.c
gcc -o ./bin/client ./src/client.c

kill -9 $(ps a | grep ./bin/server | awk '{print $1}' | head -1)

./bin/server &
sleep 3 
./bin/client
