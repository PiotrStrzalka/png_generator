FROM ubuntu:20.04

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y cmake build-essential pkg-config libpng-dev

COPY . /workspace
WORKDIR /workspace

RUN mkdir _fbuild && cd _fbuild && cmake .. && make -j4 && make install

ENV PGEN_WORKER_PATH="/usr/local/bin/PGen_WORKER"

CMD [ "/bin/bash" ]