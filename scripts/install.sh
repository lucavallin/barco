#!/bin/bash

# Update apt
sudo apt update

# Install build tools
sudo ./scripts/install-build-tools.sh
sudo ./scripts/install-dependencies.sh
