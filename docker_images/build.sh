#!/usr/bin/env bash

cd "$(dirname "$0")" || exit 1

DIR="$(cd - || exit 1)"

for lang in containers/*
do
    cd "${lang}/" || exit 1

    LANG="$(echo "${lang}" | cut -d'/' -f2)"
    CONTAINER="tr0llex/compiler-worker-${LANG}"

    docker image rm "${CONTAINER}" 2> /dev/null
    docker build -t "${CONTAINER}" .
    cd "${DIR}" || exit 1
done
