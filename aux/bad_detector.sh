#!/bin/bash

PATTERNS=(assert bad xxx dummy hack todo temporarily '#include <assert.h>' pragma)

for pattern in "${PATTERNS[@]}"; do
	grep -n --recursive --ignore-case "${pattern}" ./src
done
