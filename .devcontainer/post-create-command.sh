#!/bin/bash

# Update apt
sudo apt update

# Install Clang tools
sudo apt install -y \
    clang \
    clang-tidy \
    clang-format \
    lldb \
    clangd \
    valgrind
