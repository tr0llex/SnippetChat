FROM alpine:3.7 as builder

RUN apk add --no-cache gcc musl-dev bash wget tar make

RUN wget https://github.com/tr0llex/GNU-time/archive/refs/tags/2.0.0.tar.gz

RUN tar xzf 2.0.0.tar.gz

WORKDIR GNU-time-2.0.0/

RUN ./configure && make && make install

FROM alpine:latest

RUN apk --update --no-cache upgrade

RUN apk add --no-cache g\+\+ musl-dev bash

COPY compile.sh /bin/compile.sh

COPY run.sh /bin/run.sh

COPY --from=builder /usr/local/bin/time /usr/local/bin/time

RUN chmod 755 /bin/compile.sh

RUN chmod 755 /bin/run.sh

RUN rm -rf /etc/apk/repositories

RUN printf 'https://dl-cdn.alpinelinux.org/alpine/edge/main\nhttps://dl-cdn.alpinelinux.org/alpine/edge/community\nhttps://dl-cdn.alpinelinux.org/alpine/edge/testing' >> /etc/apk/repositories

RUN apk update && apk upgrade
