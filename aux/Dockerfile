# usage:
# `docker build -t $NAME .` once
# `docker run --rm -it $NAME bash` whenever
# name it as you please

FROM ubuntu:latest

RUN apt-get update &&\
    apt-get -y install vim &&\
    apt-get -y install build-essential &&\
    apt-get -y install cmake &&\
    apt-get -y install clang gcc &&\
    apt-get -y install lldb gdb &&\
    apt-get -y install bash bash-doc bash-completion &&\
    apt-get -y install valgrind &&\
    apt-get -y install libreadline8 libreadline-dev &&\
    apt-get -y install libx11-dev &&\
    apt-get -y install strace

RUN git clone https://github.com/froz42/funcheck.git &&\
    make -C /funcheck/library &&\
    make -C /funcheck/host

ENV PATH="$PATH:/funcheck/host"

RUN git clone https://github.com/vincent-lafouasse/minimal-vimrc.git &&\
    cp /minimal-vimrc/.vimrc /etc/vim/vimrc.local
