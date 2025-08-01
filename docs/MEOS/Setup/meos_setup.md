# MEOS Build Instructions
> **Note:** MEOS setup is required to validate test cases.

## Clone Repository
```bash
# Fork the MobilityDB repository on GitHub
# Then clone and open it in your IDE
git clone https://github.com/<your-username>/MobilityDB.git
cd MobilityDB
```

## Setup
Create build directory:
```bash
mkdir build
cd build
```

## Install Dependencies
```bash
brew install cmake
brew install gsl
brew install geos
brew install proj
brew install json-c
```

## Check Dependencies
Verify GEOS installation:
```bash
find /usr/local -name <DEPENDENCY> 2>/dev/null
find /opt/homebrew -name <DEPENDENCY> 2>/dev/null
```

## Build
```bash
cmake -DMEOS=on ..
make && sudo make install
```
