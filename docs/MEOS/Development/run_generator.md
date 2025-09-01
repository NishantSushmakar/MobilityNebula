# MEOS Function Generator

This project contains a generator that creates functions based on parsed data from the `parse-meos-functions.cpp` file.

## Overview

The generator code creates all the functions that have been parsed using the `parse-meos-functions.cpp` file. This allows for automated generation of MEOS functions from JSON data.

## Compilation

To compile the generator, use the following command:

```bash
g++ -std=c++17 -o generate-meos-function generate-meos-function-from-json.cpp
```

This will create an executable named `generate-meos-function` from the source file `scripts/generate-meos-function-from-json.cpp`.

## Usage

The generator supports two modes of operation:

### 1. Batch Generation

Generate all functions at once:

```bash
./generate-meos-function-from-json all
```

This command will generate all available functions in a single batch operation.

### 2. Single Function Generation

Generate a specific function:

```bash
./generate-meos-function-from-json <function_name>
```

**Example:**
```bash
./generate-meos-function-from-json eintersects_tgeo_geo
```

This will generate only the specified function (`eintersects_tgeo_geo` in this example), provided it exists in the JSON file.
