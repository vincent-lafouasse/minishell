#!/bin/bash

PATTERNS=(assert bad xxx dummy hack todo temporarily '#include <assert.h>' pragma)

for pattern in "${PATTERNS[@]}"; do
	grep --recursive --ignore-case "${pattern}" ./src
done

MAKEFILE_HAS_WERROR=$(grep '^CFLAGS += -Werror' Makefile)
if [ -z "${MAKEFILE_HAS_WERROR}" ];
then
	echo "Makefile doesn't compile with -Werror"
fi

MAKEFILE_HAS_WILDCARD=$(grep find Makefile)
if [ -n "${MAKEFILE_HAS_WILDCARD}" ];
then
	echo "Makefile uses wildcard to list sources"
fi
