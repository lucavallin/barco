#!/bin/bash

# Install LLVM tools required for building the project
wget https://apt.llvm.org/llvm.sh
chmod +x llvm.sh
sudo ./llvm.sh 18

# Install Clang tools
sudo apt install -y \
    clang-format-18 \
    clang-tidy-18 \
    clang-tools \
    clangd
