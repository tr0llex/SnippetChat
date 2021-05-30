#!/usr/bin/env bash

RED="\033[0;31m"
RESET="\033[0m"
cd "$(dirname "$0")" || exit

DIR=$(pwd)

#RANDOM_NUMBER="$(( ( RANDOM % 9000 )  + 1000 ))"
RUN_BOX="${DIR}"

#echo "${RUN_BOX}"

mkdir -p "${RUN_BOX}"

#cp -f "${DIR}/source.cpp" "${RUN_BOX}/source.cpp"
#cp -f "${DIR}/run.stdin" "${RUN_BOX}/run.stdin"

docker run \
    --cpus="0.5" \
    --memory="100m" \
    --ulimit nofile=64:64 \
    --rm \
    --read-only \
    -v "$RUN_BOX":/var/box \
    -w /var/box \
    tr0llex/compiler-worker-cpp20 \
    bash -c "/bin/compile.sh && /bin/run.sh"

#ls -lh "${RUN_BOX}"

stdin="$(cat "${RUN_BOX}"/run.stdin)"
stdout="$(cat "${RUN_BOX}"/run.stdout)"

#time_taken="$(< "${RUN_BOX}"/time.log tail -n 1)"
#IFS='\n' read -r -a resources <<< "$(cat "${RUN_BOX}"/time.log)"
readarray -t resources <<< "$(cat "${RUN_BOX}"/time.log)"


#rm -rf "${RUN_BOX}"

echo -e "\n${RED}Execution ID: ${RESET}${RANDOM_NUMBER}\n${RED}Execution time: ${RESET}${resources[0]}sec.\n${RED}Memory used: ${RESET}${resources[1]}"

echo -e "\n${RED}Input:${RESET}\n$stdin\n\n\n${RED}Output:${RESET}\n$stdout"
