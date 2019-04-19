# logpipe

A utility for piping the output of commands to AWS CloudWatch Logs.

## Usage

```shell
$ my-command | logpipe eu-west-2 my-command-logs
```

## Building

The easiest way to build this is using the supplied Docker based build script. If you have Docker installed, simply run

```shell
$ ./build-docker.sh
```

and the binary will be placed in the ```output``` directory.

If you want to build manually, you'll require an installation of the AWS C++ SDK, CMake 3, and a C++11 compiler.

## Disclaimer

All the usual warnings apply. While this works for my needs, it's up to you to make sure it works for yours. See LICENSE for full details.
