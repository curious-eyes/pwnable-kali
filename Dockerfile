FROM kalilinux/kali-rolling

RUN apt-get update
# clean up
RUN apt-get upgrade -y && apt-get clean && rm -rf /var/cache/apt/archives/* /var/lib/apt/lists/*
RUN apt-get update

# https://tools.kali.org/kali-metapackages
RUN apt-get -y install kali-tools-reverse-engineering
# RUN apt-get -y install kali-tools-default
# RUN apt-get -y install kali-tools-headless
# RUN apt-get -y install kali-tools-top10

RUN apt-get -y install gdb ncat

# ENTRYPOINT bin/bash
