#!/bin/bash

# Install build tools
sudo $CODESPACE_VSCODE_FOLDER/scripts/install.sh

# Install development tools
sudo apt install -y \
    lldb \
    clangd \
    valgrind \
    binutils
