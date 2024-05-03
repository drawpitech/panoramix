#!/usr/bin/env bash

if [ "$#" -ne 1 ] && [ "$#" -ne 2 ]; then
  echo "Usage: $0 <limit> [program]"
  exit 1
fi

limit=$1
program=$2
if [ -z "$program" ]; then
  program="./panoramix"
fi

echo "About to run $(($limit * $limit * $limit * $limit)) times"

# ./panoramix <nb_villagers> <pot_size> <nb_fights> <nb_refills>
for ((vil = 1; vil <= limit; vil++)); do
  for ((pot = 1; pot <= limit; pot++)); do
    for ((fig = 1; fig <= limit; fig++)); do
      for ((ref = 1; ref <= limit; ref++)); do
        echo -n "Run number: $vil $pot $fig $ref         "
        echo -ne "\r"
        $program "$vil" "$pot" "$fig" "$ref" > /dev/null
      done
    done
  done
done

echo
