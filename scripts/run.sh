#!/bin/bash

set -e

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd)"
PROJECT_ROOT="${SCRIPT_DIR}/.."

function run_code() {
    for file in "${PROJECT_ROOT}/bin/"*; do
        if [ -f "$file" ] && [ -x "$file" ]; then
            "${file}"
        fi
    done
}


dir="${PROJECT_ROOT}/bin"

if [[ -d "${dir}" ]]; then
    echo "find bin dir"
    run_code
else
    echo "can't find bin dir, we build..."
    sh ${PROJECT_ROOT}/scripts/build.sh
    run_code
fi

