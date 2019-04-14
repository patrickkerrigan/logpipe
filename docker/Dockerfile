FROM centos:7

USER root

RUN yum -y install epel-release && \
    yum -y install libcurl-devel openssl-devel libuuid-devel gcc g++ gcc-c++ make cmake3 git && \
    cd /opt && \
    git clone https://github.com/aws/aws-sdk-cpp.git && \
    cd aws-sdk-cpp && \
    mkdir sdk_build && \
    cd sdk_build && \
    cmake3 -DBUILD_ONLY="logs" -DSTATIC_LINKING=1 .. && \
    make && \
    make install

ENTRYPOINT "/opt/logpipe/docker/build.sh"