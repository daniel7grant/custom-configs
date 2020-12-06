#!/bin/sh

function alpine() {
    if command -v podman &> /dev/null ; then
        podman run -it --rm -v $(pwd):/app --workdir /app alpine /bin/sh
    elif command -v docker &> /dev/null ; then
        docker run -it --rm -v $(pwd):/app --workdir /app alpine /bin/sh
    fi
}
