#!/bin/bash

# Update apt
sudo apt update && sudo apt upgrade -y

# Install build tools
sudo ./scripts/install-build.sh
sudo ./scripts/install-deps.sh
