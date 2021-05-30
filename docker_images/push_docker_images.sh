#!/usr/bin/env bash

cd $(dirname "$0")

DIR=$(cd -)

docker login

for lang in $(ls "$DIR/containers")
do
    cd ${DIR}/containers/${lang}
    docker push tr0llex/compiler-worker-${lang}
    cd ${DIR}
done