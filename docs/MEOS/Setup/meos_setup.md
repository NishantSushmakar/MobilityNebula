# MobilityDB Build Instructions



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
