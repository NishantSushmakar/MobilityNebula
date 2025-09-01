# Parse MEOS Functions

This utility parses the MEOS function headers and generates a JSON file containing detailed information about all functions.

## Setup

1. In the file `scripts/parse_meos_functions.cpp`, hardcode the location of the MEOS header to be parsed:

```cpp
std::string headerPath = "../MobilityDB/meos/include/meos_geo.h";
```
   
Update the path if your header is located elsewhere.

2. Build

From the scripts directory, compile the parser:

```bash
 g++ -std=c++17 -o parse-meos-functions parse-meos-functions.cpp

```

This will create an executable named parse-meos-functions.

3. Usage

Run the executable to generate the JSON file:
```bash
./parse-meos-functions
```
