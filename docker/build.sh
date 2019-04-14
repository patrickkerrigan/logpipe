#!/bin/bash

cd /opt/logpipe/output

export CMAKE_PREFIX_PATH="/opt/aws-sdk-cpp/sdk_build"

cmake3 ..
make