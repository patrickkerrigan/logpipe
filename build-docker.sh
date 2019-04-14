#!/bin/bash

cd docker
docker build -t logpipe-builder .
cd ..
mkdir -p output
docker run --rm -v $(pwd):"/opt/logpipe" logpipe-builder