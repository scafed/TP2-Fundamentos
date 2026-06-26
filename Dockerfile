FROM ubuntu:24.04

RUN apt-get update && \
    apt-get install -y \
        gcc \
        make \
        valgrind && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /tp2

CMD ["/bin/bash"]