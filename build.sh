#!/bin/sh

make build

kill -9 $(ps a | grep ./bin/server | awk '{print $1}' | head -1)

./bin/server &
sleep 3 
./bin/client
