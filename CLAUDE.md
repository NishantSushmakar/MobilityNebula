# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

MobilityNebula is an open-source geospatial trajectory data streaming platform based on NebulaStream. It integrates MobilityDB datatypes and functions through the MEOS (MobilityDB Extension for Object Storage) library to provide advanced spatiotemporal stream processing capabilities for IoT applications.

## Build Commands

### Development Environment Setup
```bash
# Build local development container (recommended)
./scripts/install-local-docker-environment.sh

# Configure build with Docker
docker run --workdir $(pwd) -v $(pwd):$(pwd) nebulastream/nes-development:local cmake -B build-docker

# Build the project
docker run --workdir $(pwd) -v $(pwd):$(pwd) nebulastream/nes-development:local cmake --build build-docker -j
```

### Native Development
```bash
# Configure build with local vcpkg
cmake -B build -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake

# Build all components
cmake --build build -j

# Build plugins only
cmake --build build --target build_all_plugins
```

### Build Types
- **Debug**: Full logging and assertions (`cmake -DCMAKE_BUILD_TYPE=Debug`)
- **Release**: Optimized with error-level logging (`cmake -DCMAKE_BUILD_TYPE=Release`)
- **RelWithDebInfo**: Balanced performance with debugging info
- **Benchmark**: Maximum performance, no assertions or logging

## Testing Commands

```bash
# Run all tests in Docker
docker run --workdir $(pwd) -v $(pwd):$(pwd) nebulastream/nes-development:local ctest --test-dir build-docker -j

# Run specific test suites
ctest --test-dir build -R "nes-memory"
ctest --test-dir build -R "MEOS"

# Run system tests
ctest --test-dir build -R "systest"
```

## Interactive Development Workflow

### Docker Container Development
```bash
# Enter interactive Docker container for development
docker run -it --rm -v "$(pwd)":/workspace nebulastream/nes-development:local /bin/bash

# Inside container - create build directory and build
cd /workspace && mkdir -p build && cd build
cmake .. && ninja

# Run specific system tests
./nes-systests/systest/systest -t tests/var_aggregation.test

# Run MEOS-related tests
./nes-systests/systest/systest -t Tests/meos_intersects_simple.test
./nes-systests/systest/systest -t Tests/Functions/eintersects_tgeo_linestring_polygon.test
```

## Code Quality Commands

```bash
# Format code and run checks
./scripts/format.sh

# Fix formatting issues
./scripts/format.sh -i

# The format script includes:
# - clang-format (requires clang-format-19)
# - License preamble validation
# - Comment style enforcement (only /// comments allowed)
# - TODO checking
```

## Architecture Overview

### Core Platform (NebulaStream)
- **nes-query-engine**: Central query execution orchestration
- **nes-runtime**: Runtime execution environment and context management
- **nes-memory**: High-performance memory management and buffer pools
- **nes-nautilus**: JIT compilation framework using LLVM/MLIR
- **nes-query-optimizer**: Cost-based query optimization and plan selection
- **nes-query-compiler**: Query compilation to native code

### Data Processing Pipeline
- **nes-sources**: Data ingestion connectors (files, TCP, generators, MQTT)
- **nes-input-formatters**: Format parsers (CSV, JSON) with sequence shredding
- **nes-logical-operators**: High-level query operators (select, project, join, window)
- **nes-physical-operators**: Low-level execution operators with Nautilus integration
- **nes-sinks**: Output destinations (files, print, MQTT, checksum)

### MobilityDB Integration (MEOS)
- **MEOSWrapper**: C++ wrapper around MEOS library providing thread-safe operations
- **Temporal Functions**: 
  - `TEMPORAL_SEQUENCE(lon, lat, timestamp)`: Aggregates GPS points into trajectories
  - `TEMPORAL_EINTERSECTS_GEOMETRY(lon, lat, timestamp, [geometry])`: Spatial intersection testing (4/6 param)
  - `TEMPORAL_AINTERSECTS_GEOMETRY(lon, lat, timestamp, [geometry])`: Always spatial intersection testing (4/6 param)
- **Function Parameters**:
  - **4-param**: `(lon, lat, timestamp, static_geometry_wkt)` - temporal point vs static geometry (TgeoGeo)
  - **6-param**: `(lon1, lat1, timestamp1, lon2, lat2, timestamp2)` - temporal point vs temporal point (TgeoTgeo)
- **Architecture**: Isolated template system with complete parameter type separation
  - **TgeoGeo Templates**: Handle only 4-parameter temporal-static operations
  - **TgeoTgeo Templates**: Handle only 6-parameter temporal-temporal operations
  - **No Mixing**: Each template handles exactly one use case for consistency and maintainability
- **Integration**: Full integration with NebulaStream's query compilation pipeline through ANTLR grammar
- **Data Types**: Native geospatial coordinate handling with WGS84 support
- **MEOS Library Integration**: 
  - C++ wrapper methods map to MEOS C functions: `intersects()` → `eintersects_tgeo_tgeo()`, `intersectsStatic()` → `eintersects_tgeo_geo()`
  - Thread-safe operations with proper RAII lifecycle management

### Plugin System
- **nes-plugins/MEOS**: MobilityDB integration plugin
- **nes-plugins/Sources**: Additional source connectors (Audio, Generator, MQTT, TCP)
- **nes-plugins/Sinks**: Additional sink connectors (Checksum, MQTT)
- **Dynamic Loading**: Plugin registration through generated CMake registrars

## Development Guidelines

### C++ Standards
- **Language**: C++23 (requires Clang 18+)
- **Standard Library**: Supports both libc++ (default in Docker) and libstdc++
- **Compilation**: Clang-only (GCC not supported)

### Key Dependencies
- MEOS library (with GEOS, PROJ, GSL, JSON-C dependencies)
- gRPC and Protocol Buffers for distributed communication
- LLVM/MLIR for JIT compilation
- vcpkg for package management
- Google Test for unit testing

### Code Organization
- Each `nes-*` directory is a modular component with clear dependencies
- Plugin-based architecture allows optional functionality
- Registry pattern used for extensible operator and function sets
- Header-only public interfaces with private implementation details

### Memory Management
- Arena-based allocation for high-throughput streaming
- RAII patterns for MEOS object lifecycle management
- Buffer recycling for reduced garbage collection pressure

### Testing Strategy
- Unit tests in each component's `tests/` directory
- System tests in `nes-systests/` for end-to-end validation
- Specialized test data in `testdata/` directories
- Docker-based test environments for reproducibility

## Common Development Tasks

### Adding New MEOS Functions (Automated Process)

#### **Fully Automated Generation**
Adding a new MEOS spatial function is now **completely automated** with a single command:

```bash
cd scripts/build && make
./generate-meos-function-smart EIntersects
```

This single command automatically:
1. **Generates 4 C++ files** - logical and physical headers and implementations for both TgeoGeo (4-param) and TgeoTgeo (6-param) variants
2. **Updates 2 CMakeLists.txt files** - adds plugin registrations for logical and physical operators
3. **Updates SQL parser** - adds case handlers, includes, and ANTLR grammar tokens
4. **Uses isolated templates** - TgeoGeo templates for 4-param only, TgeoTgeo templates for 6-param only
5. **Validates integration** - ensures all 7 files are consistently integrated

#### **Function Architecture**
All MEOS functions follow the **isolated template architecture**:
- **TgeoGeo Functions**: `Temporal{Name}TgeoGeoLogicalFunction` - handles only 4-parameter temporal-static operations
- **TgeoTgeo Functions**: `Temporal{Name}TgeoTgeoLogicalFunction` - handles only 6-parameter temporal-temporal operations
- **SQL Tokens**: `TEMPORAL_{NAME}_TGEO_GEO` and `TEMPORAL_{NAME}_TGEO_TGEO` for precise parameter type matching
- **No Parameter Mixing**: Each generated function handles exactly one parameter signature

#### **Integration Points (7 Files Total)**
The generator automatically handles all integration points:

1. **C++ Implementation (4 files)**:
   - `nes-logical-operators/include/Functions/Meos/Temporal{Name}TgeoGeoLogicalFunction.hpp`
   - `nes-logical-operators/src/Functions/Meos/Temporal{Name}TgeoGeoLogicalFunction.cpp`
   - `nes-physical-operators/include/Functions/Meos/Temporal{Name}TgeoGeoPhysicalFunction.hpp`
   - `nes-physical-operators/src/Functions/Meos/Temporal{Name}TgeoGeoPhysicalFunction.cpp`
   - (Plus TgeoTgeo variants for 6-parameter functions)

2. **Build System Integration (2 files)**:
   - Auto-updates `nes-logical-operators/src/Functions/Meos/CMakeLists.txt`
   - Auto-updates `nes-physical-operators/src/Functions/Meos/CMakeLists.txt`

3. **SQL Parser Integration (1 file)**:
   - Auto-updates `nes-sql-parser/AntlrSQL.g4` grammar
   - Auto-inserts case handlers in `AntlrSQLQueryPlanCreator.cpp`
   - Auto-adds header includes

#### **Configuration-Driven**
Function behavior is defined in `scripts/meos-functions.json`:
```json
{
  "EIntersects": {
    "4param_method": "eintersects_tgeo_geo",
    "6param_method": "eintersects_tgeo_tgeo", 
    "wrapper_4param": "intersectsStatic",
    "wrapper_6param": "intersects",
    "return_type": "INT32",
    "sql_token": "TEMPORAL_EINTERSECTS"
  }
}
```

#### **Testing**
- Generated functions include debug output for validation
- Test both isolated variants: 4-param TgeoGeo and 6-param TgeoTgeo
- Use existing test patterns in `Tests/Functions/` directory

### Working with Spatial Data
- Default SRID is 4326 (WGS84)
- Coordinates are FLOAT64 (longitude, latitude order)
- Timestamps can be seconds or milliseconds (UINT64)
- Use MEOSWrapper classes for thread-safe operations

### Plugin Development
- Follow existing plugin patterns in `nes-plugins/`
- Use CMake registrar system for automatic registration
- Ensure proper dependency linking in CMakeLists.txt
- Add to `build_all_plugins` target

### MEOS Function Generation Troubleshooting

#### **Critical Build Error Patterns and Solutions**

**1. MEOS Namespace Conflicts**
```cpp
// ❌ WRONG - Causes "use of undeclared identifier" errors
extern "C" {
    #include <meos.h>
    #include <meos_geo.h>
}
// Also including MEOSWrapper.hpp

// ✅ CORRECT - MEOSWrapper already includes headers in MEOS namespace
#include <MEOSWrapper.hpp>
// Use MEOS:: prefix for C functions
MEOS::Temporal* temp = MEOS::tgeometry_in(wkt.c_str());
auto result = MEOS::eintersects_tgeo_tgeo(temp1, temp2);
```

**2. ANTLR Grammar Token Issues**
```antlr
// ❌ WRONG - Parser rule references undefined tokens
functionName: IDENTIFIER | NEW_UNDEFINED_TOKEN;

// ✅ CORRECT - Always add lexer definitions first
NEW_DEFINED_TOKEN: 'NEW_DEFINED_TOKEN' | 'new_defined_token';
functionName: IDENTIFIER | NEW_DEFINED_TOKEN;
```

**3. SQL Parser Parameter Extraction**
```cpp
// ❌ WRONG - Wrong context and parameter types
auto param0_varsized = parameterValues[0].cast<VariableSizedData>();
const auto function = MyFunction(temporal0Function, temporal1Function);

// ✅ CORRECT - Extract LogicalFunctions from functionBuilder stack
const auto param1Function = helpers.top().functionBuilder.back();
helpers.top().functionBuilder.pop_back();
const auto param0Function = helpers.top().functionBuilder.back();
helpers.top().functionBuilder.pop_back();
const auto function = MyFunction(param0Function, param1Function);
```

**4. CMake Plugin Registration**
```cmake
# ❌ WRONG - Using full class name as plugin name
add_plugin(MyFunctionLogicalFunction LogicalFunction nes-logical-operators MyFunction.cpp)

# ✅ CORRECT - Use concise plugin name (affects registry key)
add_plugin(MyFunction LogicalFunction nes-logical-operators MyFunctionLogicalFunction.cpp)
```

**5. Missing Header Includes**
```cpp
// ❌ WRONG - Using functions without proper includes
const auto function = TemporalIntersectsGeometryLogicalFunction(...);

// ✅ CORRECT - Always include required headers
#include <Functions/Meos/TemporalIntersectsGeometryLogicalFunction.hpp>
```

#### **Complete Integration Checklist**
✅ **Before Generation:**
- [ ] MEOS C function exists in meos_geo.h
- [ ] Function signature documented in meos-functions.json
- [ ] Template files use correct placeholder format `{{PLACEHOLDER}}`

✅ **After Code Generation:**
- [ ] All 4 files generated (2 logical + 2 physical)
- [ ] Headers include all required interface methods
- [ ] CMakeLists.txt updated with correct plugin names
- [ ] Registry functions use correct parameter counts

✅ **ANTLR Integration:**
- [ ] Lexer tokens defined before parser rules reference them
- [ ] Token names match exactly between lexer and parser
- [ ] AntlrSQLQueryPlanCreator.cpp includes required headers
- [ ] Case handlers extract parameters from functionBuilder stack correctly

✅ **Build Verification:**
- [ ] ANTLR grammar compiles without warnings
- [ ] Logical and physical functions compile without errors
- [ ] SQL parser compiles and links successfully
- [ ] Functions accessible via registry system

#### **Common Error Solutions**

| Error Pattern | Root Cause | Solution |
|--------------|------------|----------|
| `use of undeclared identifier 'tgeometry_in'` | MEOS namespace conflict | Use `MEOS::tgeometry_in()` and remove duplicate includes |
| `implicit definition of token X in parser` | Missing lexer definition | Add `X: 'X' \| 'x';` to lexer section |
| `Unknown aggregation function` | Missing SQL parser case | Add case handler in AntlrSQLQueryPlanCreator.cpp |
| `use of undeclared identifier 'temporal0Function'` | Wrong parameter extraction | Extract from `functionBuilder` stack, not undefined variables |
| `no matching function for call` | Wrong parameter types | Use `LogicalFunction` not `VariableSizedData` in logical layer |

#### **Generator Debugging Steps**
1. **Template Validation**: Verify all `{{PLACEHOLDER}}` have replacements
2. **File Permissions**: Check target directories are writable
3. **Syntax Validation**: Ensure generated code compiles independently
4. **Integration Testing**: Build full system after generation
5. **Registry Verification**: Check functions appear in generated registrar files

## Important Notes

- Docker development environment is strongly recommended
- All CI runs in containerized environment with libc++
- MEOS library requires system-level dependencies (GEOS, PROJ)
- Thread safety is critical for streaming applications
- Memory leaks are tracked with optional instrumentation (`CMAKE_NES_DEBUG_TUPLE_BUFFER_LEAKS`)
- MEOS internal Implementation is `/Users/nishantsushmakar/Documents/mobility_internship/MobilityDB/meos`

## Memories
- compact
- to memorize