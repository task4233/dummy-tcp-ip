FROM alpine:3.13
RUN apk add --no-cache gcc libc-dev make cpputest

WORKDIR /app
COPY ./src /app/src
COPY ./test /app/test
COPY ./md5_src/ /app/md5_src
COPY ./Makefile /app/Makefile

RUN mkdir -p /app/bin
RUN make test
RUN make build

