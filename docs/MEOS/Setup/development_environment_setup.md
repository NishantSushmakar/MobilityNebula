# NebulaStream & MobilityDB Integration – Environment Setup Guide

This document explains how to set up a local development environment for the NebulaStream and MobilityDB integration using Docker. It covers building the container, solving common setup issues, and compiling the project inside the development container.

---

## Prerequisites

Before you begin, ensure you have the following installed:

- [Docker](https://www.docker.com/)
- [Homebrew](https://brew.sh/) (for macOS users)
- A code editor like VS Code or CLion

---

## Step-by-Step Setup

### 1. Fork and Open the Repository

```bash
# Fork the MobilityNebula repository on GitHub
# Then clone and open it in your IDE
git clone https://github.com/<your-username>/MobilityNebula.git
cd MobilityNebula
```

### 2. Run the script

```bash
# Go to the scripts folder
cd scripts
# Run the .sh file for starting the setup
./install-local-docker-environment.sh -l
```

### 3. Run the command in docker container

```bash
# Go to the main directory of MobilityNebula
cd ..
# Run the docker command in the terminal
docker run -it --rm -v "$(pwd)":/workspace nebulastream/nes-development:local /bin/bash
# Now inside the bash shell go to workspace directory
cd workspace
# Create a build directory and go inside it
mkdir build
cd ..
# Compile and Link the project
cmake ..
ninja
```

## Bugs and Fixes

### Fix: sha256sum command not found

#### Error
```
docker/dependency/hash_dependencies.sh: line 37: sha256sum: command not found
```

#### Solution

Install coreutils:
```bash
brew install coreutils
```

Add to PATH:
```bash
echo 'export PATH="/opt/homebrew/opt/coreutils/libexec/gnubin:$PATH"' >> ~/.zshrc
source ~/.zshrc
```

Verify:
```bash
sha256sum --version
```



