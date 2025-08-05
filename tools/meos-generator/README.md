# MEOS Function Generator

Automated code generation tool for integrating MEOS (Mobility Engine Open Source) functions into MobilityNebula.

## Overview

This tool automatically discovers MEOS functions from header files and generates the complete integration code needed for MobilityNebula, including:

- MEOSWrapper method declarations and implementations
- Logical function classes following existing patterns
- Physical function classes with coordinate-to-WKT conversion
- SQL grammar tokens and parser cases
- Comprehensive test files

## Features

- **Header Parsing**: Automatically discovers MEOS functions from C header files
- **Template-Based Generation**: Uses templates based on existing aintersects/eintersects patterns
- **MobilityNebula Structure**: Maintains exact current directory structure
- **Filtering Support**: Generate specific functions by pattern or category
- **Dry Run Mode**: Preview what would be generated without writing files
- **Skip Existing**: Optionally skip generation if files already exist

## Installation Options

### Option 1: CMake Integration (Recommended)

Add to your MobilityNebula CMakeLists.txt:

```cmake
# Add the generator as a subdirectory
add_subdirectory(tools/meos-generator)

# Custom target to run code generation
add_custom_target(generate_meos_functions
    COMMAND meos-generator 
        --meos-include ${meos_INCLUDE_DIR}
        --mobility-root ${CMAKE_SOURCE_DIR}
        --pattern "*intersects*"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating MEOS function integration code"
)
```

### Option 2: Standalone Build

```bash
cd tools/meos-generator
mkdir build && cd build
cmake .. -DMEOS_GENERATOR_STANDALONE=ON
make -j
```

### Option 3: Docker

```dockerfile
FROM nebulastream/nes-development:local
WORKDIR /workspace
COPY tools/meos-generator /workspace/meos-generator
RUN cd meos-generator && cmake -B build && cmake --build build
```

## Usage

### Basic Usage

```bash
# Generate all temporal geometry functions
./meos-generator --meos-include /usr/local/include/meos --output ./generated

# Generate specific function patterns
./meos-generator --pattern "*intersects*" --pattern "*contains*"

# Generate by category
./meos-generator --category spatial_intersection --category spatial_containment

# Dry run to see what would be generated
./meos-generator --dry-run --pattern "*intersects*"
```

### Advanced Usage

```bash
# Use with MobilityNebula project structure
./meos-generator \
    --meos-include /usr/local/include/meos \
    --mobility-root /path/to/MobilityNebula \
    --pattern "*intersects*" \
    --skip-existing \
    --no-tests

# Generate for specific functions only
./meos-generator \
    --pattern "eintersects*" \
    --pattern "aintersects*" \
    --pattern "contains*"
```

### Command Line Options

- `--meos-include PATH`: Path to MEOS include directory (required)
- `--output PATH`: Output directory for generated code (default: ./generated)
- `--template PATH`: Template directory path (default: ./templates)
- `--mobility-root PATH`: MobilityNebula root directory for proper structure
- `--pattern PATTERN`: Function name pattern to match (can be repeated)
- `--category CATEGORY`: Function category to generate (can be repeated)
- `--skip-existing`: Skip generation if file already exists
- `--no-tests`: Don't generate test files
- `--dry-run`: Don't write files, just show what would be generated
- `--help`: Show help message

## Function Categories

The generator automatically categorizes discovered functions:

- `spatial_intersection`: Functions containing "intersects"
- `spatial_containment`: Functions containing "contains"
- `spatial_distance`: Functions containing "distance"
- `temporal_geometry`: Functions starting with "tgeo_" or "tpoint_"
- `temporal`: Functions starting with "temporal_"
- `geometry`: Other geometry-related functions
- `general`: All other functions

## Generated Code Structure

When using `--mobility-root`, the generator maintains MobilityNebula's structure:

```
MobilityNebula/
├── nes-logical-operators/
│   ├── include/Functions/Meos/
│   │   └── [Function]LogicalFunction.hpp
│   └── src/Functions/Meos/
│       └── [Function]LogicalFunction.cpp
├── nes-physical-operators/
│   ├── include/Functions/Meos/
│   │   └── [Function]PhysicalFunction.hpp
│   └── src/Functions/Meos/
│       └── [Function]PhysicalFunction.cpp
├── nes-plugins/MEOS/include/
│   └── generated_methods.hpp
├── nes-sql-parser/
│   ├── generated_tokens.g4
│   └── generated_parser_cases.cpp
└── Tests/Functions/
    └── [function]_geometry.test
```

## Template System

The generator uses a simple template engine supporting:

- Variable substitution: `{{variableName}}`
- Array iteration: `{{#arrayName}}...{{/arrayName}}`
- Special variables in arrays: `{{item}}`, `{{index}}`, `{{notLast}}`

### Available Templates

- `logical_function_header.hpp.template`: Logical function header
- `logical_function_impl.cpp.template`: Logical function implementation
- `meos_wrapper_method.hpp.template`: MEOSWrapper method declaration
- `meos_wrapper_method.cpp.template`: MEOSWrapper method implementation
- `sql_grammar_token.template`: SQL grammar token
- `sql_parser_case.template`: SQL parser case
- `test_file.test.template`: Test file template

## Examples

### Generate All Intersection Functions

```bash
./meos-generator \
    --meos-include /usr/local/include/meos \
    --mobility-root /path/to/MobilityNebula \
    --pattern "*intersects*"
```

### Generate Containment Functions Only

```bash
./meos-generator \
    --pattern "*contains*" \
    --category spatial_containment \
    --output ./generated/containment
```

### Preview Generation

```bash
./meos-generator \
    --dry-run \
    --pattern "*" \
    --meos-include /usr/local/include/meos
```

## Integration with MobilityNebula

After generating code:

1. **Include Headers**: Add generated logical function headers to appropriate includes
2. **Register Functions**: Add function registration calls to plugin registrars
3. **Update SQL Grammar**: Merge generated tokens into AntlrSQL.g4
4. **Update Parser**: Merge generated parser cases into AntlrSQLQueryPlanCreator.cpp
5. **Build**: Run the build process to compile generated code
6. **Test**: Run generated tests to verify functionality

## Architecture

The generator follows these design principles:

- **Template-Based**: All code generation uses templates based on existing patterns
- **Type-Safe**: Generates proper C++ type mappings and validations
- **Structure-Preserving**: Maintains MobilityNebula's exact directory structure
- **Extensible**: Easy to add new templates and generation patterns
- **Robust**: Handles errors gracefully and provides detailed feedback

## Contributing

To add support for new function types:

1. Create new templates in the `templates/` directory
2. Add template rendering logic to `CodeGenerator.cpp`
3. Update the function categorization in `MeosFunctionDiscovery.cpp`
4. Add corresponding command-line options if needed

## Dependencies

- C++23 compiler (Clang 18+)
- MEOS library headers
- CMake 3.20+
- Standard library with filesystem support