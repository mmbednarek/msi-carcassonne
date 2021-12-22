FROM ubuntu:21.10

ENV home=/root
ENV TZ=Europe/Kiev
ENV DEBIAN_FRONTEND=noninteractive

RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN apt -y clean
RUN apt update && apt -y --no-install-recommends install \
        git \
        build-essential \
        clang \
        cmake \
        gdb \
        wget \
        libfmt-dev \
        libgtest-dev \
        libprotobuf-dev \
        libleveldb-dev \
        libsnappy-dev \
        libopencv-dev \
        libhdf5-serial-dev \
        libatlas-base-dev \
        libgflags-dev \
        libgoogle-glog-dev \
        liblmdb-dev \
        protobuf-compiler

# install cuda

RUN apt -y install gnupg2 software-properties-common

RUN wget --no-check-certificate -O /etc/apt/preferences.d/cuda-repository-pin-600 https://developer.download.nvidia.com/compute/cuda/repos/ubuntu2004/x86_64/cuda-ubuntu2004.pin
RUN apt-key adv --fetch-keys https://developer.download.nvidia.com/compute/cuda/repos/ubuntu2004/x86_64/7fa2af80.pub
RUN add-apt-repository "deb http://developer.download.nvidia.com/compute/cuda/repos/ubuntu2004/x86_64/ /"

RUN apt update && apt -y --no-install-recommends install cuda
RUN echo 'export PATH=/usr/local/cuda/bin${PATH:+:${PATH}}' >> ~/.bashrc

# building caffe

RUN apt -y install libboost-all-dev libhdf5-dev

WORKDIR /root
RUN git config --global http.sslverify false
RUN git clone https://github.com/BVLC/caffe.git
WORKDIR /root/caffe
COPY Makefile.config /root/caffe/Makefile.config
RUN make clean
RUN make all -j$(nproc)

# bulding carcassonne

COPY . /root/carcassonne
RUN mkdir -p /root/carcassonne/build-docker

WORKDIR /root/carcassonne/build-docker
RUN cmake ..