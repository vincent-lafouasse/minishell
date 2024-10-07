#!/bin/bash

PATTERNS=(bad dummy hack todo temporarily)

for pattern in "${PATTERNS[@]}"; do
	grep --recursive --ignore-case "${pattern}" ./src/
done

MAKEFILE_HAS_WERROR=$(grep '^CFLAGS += -Werror' Makefile)
if [ -z "${MAKEFILE_HAS_WERROR}" ];
then
	echo "Makefile doesn't compile with -Werror"
fi
