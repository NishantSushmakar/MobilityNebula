# Nebula MEOS Integration Generator

This document explains the working of the Nebula MEOS integration generator code and provides a step-by-step guide for implementing new MEOS functions.

## Overview

The generator focuses on creating code through existing created templates. The integration process involves several key components that work together to enable MEOS function calls within the Nebula system.

## Architecture Flow

The integration follows this workflow:

**Template Generation → Logical Operator → Physical Operator → SQL Parser → Build & Run**

## Implementation Steps

### 1. NES Logical Operator Setup

**Location:** `nes-logical-operator` folder

- **Include folder:** Add function definitions
- **Src folder:** Add function constructors and implementation under the MEOS section
- **Purpose:** Define the logical structure and interface of the MEOS function

### 2. NES Physical Operator Implementation  

**Location:** `nes-physical-operator` folder

- Implement the actual MEOS function calls
- This layer handles the physical execution of the logical operators defined in step 1

### 3. CMake Configuration

**Requirement:** Update `cmake_list.txt` in both:
- Logical operator directory
- Physical operator directory

**Purpose:** Ensure the new functions are included in the build process

### 4. SQL Parser Integration

The Nebula stream needs to identify which MEOS function is being called. This requires modifications in the `nes-sql-parser` folder:

#### 4.1 Grammar Definition
**File:** `AntlrSQL.g4`
- Define how to call the new function
- Specify the syntax and grammar rules for the function

#### 4.2 Query Plan Creator
**File:** `src/AntlrSQLQueryPlanCreator.cpp`
- Define how function arguments should be parsed
- Implement the logic for sending parsed arguments to the logical layer

### 5. Build and Run

1. **Build the modifications:**
   - Follow the setup instructions for building the project
   - Ensure all cmake configurations are properly updated

2. **Test the function:**
   - Run the implemented function to verify integration
   - Validate that the MEOS function is correctly identified and executed

