
.PHONY: build-server
build-server:
	gcc -o ./bin/server ./src/server.c ./md5_src/md5c.c

.PHONY: build-client
build-client:
	gcc -o ./bin/client ./src/client.c

build:
	make build-server
	make build-client

run-server: build-server
	./bin/server

run-client: build-client
	./bin/server

test:
	echo "test"

docker-build:
	docker build -t architecture .

docker-run:
	docker run --rm -it architecture /bin/sh
