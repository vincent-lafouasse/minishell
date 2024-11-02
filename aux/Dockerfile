# usage:
# move Dockerfile to root of minishell
# `docker build -t minishell_image .` once
# `docker run --rm -it minishell_image bash` whenever

FROM alpine:latest

RUN apk update && apk upgrade \
    && apk add bash bash-doc bash-completion \
    && apk add vim \
    && apk add build-base git make cmake \
    && apk add clang lldb \
    && apk add gcc gdb \
    && apk add valgrind \
    && apk add readline-dev \
    && git clone https://github.com/vincent-lafouasse/minishell.git
