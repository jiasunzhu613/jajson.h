FROM --platform=linux/amd64 ubuntu:jammy AS builder

# Update and upgrade packages
RUN apt update && apt upgrade -y && apt -y install sudo
RUN apt install make valgrind gcc vim curl -y

WORKDIR /jajson.h/recursive_descent

# Run benchmarking 
# RUN make benchmarking

# CMD ["./benchmarking.out"]

ENTRYPOINT [ "/bin/bash" ]