FROM debian:buster-slim
RUN apt-get update && apt-get install -y gcc libc-dev make libcunit1-dev

WORKDIR /app
COPY ./src /app/src
COPY ./test /app/test
COPY ./md5_src/ /app/md5_src
COPY ./Makefile /app/Makefile

RUN mkdir -p /app/bin
# RUN make test
RUN make build

