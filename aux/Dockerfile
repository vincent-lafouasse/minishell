# usage:
# `docker build -t $NAME .` once
# `docker run --rm -it $NAME bash` whenever
# name it as you please

FROM alpine:latest

RUN apk update && apk upgrade \
    && apk add bash bash-doc bash-completion \
    && apk add vim \
    && apk add build-base git make cmake \
    && apk add clang lldb \
    && apk add gcc gdb \
    && apk add valgrind strace \
    && apk add readline-dev \
    && git clone https://github.com/vincent-lafouasse/minishell.git
