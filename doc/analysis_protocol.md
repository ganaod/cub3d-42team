# cub3D Program Analysis Protocol

## Overview

This directory contains systematic analysis protocols for the `cub3d` raycasting engine. The analysis workflow progresses from basic functionality verification to comprehensive memory safety and undefined behavior detection.

**Core Principle**: Every analysis must be reproducible and evidence-based. Document findings, not assumptions.

## Build Configuration Architecture

The Makefile implements a `BUILD_TYPE` variable system enabling different analysis configurations:

```bash
make                    # Production build (default)
make BUILD_TYPE=debug   # Debug symbols + no optimization
make BUILD_TYPE=asan    # Address Sanitizer
make BUILD_TYPE=ubsan   # Undefined Behavior Sanitizer
make BUILD_TYPE=combined # Both sanitizers enabled
```

### Build Type Specifications

| Build Type | Flags | Purpose | Performance Impact |
|------------|-------|---------|-------------------|
| `release` | `-O2` | Production executable | Baseline |
| `debug` | `-g3 -O0` | Debugging, Valgrind analysis | ~2-5x slower |
| `asan` | `-fsanitize=address -g3` | Memory error detection | ~2-3x slower |
| `ubsan` | `-fsanitize=undefined -g3` | Undefined behavior detection | ~1.5-2x slower |
| `combined` | Both sanitizers | Comprehensive error detection | ~3-4x slower |

## Analysis Methodology

### Phase 1: Basic Functionality Verification

**Objective**: Confirm program compiles and executes core functionality without crashes.

```bash
# Quick sanity check
make test
```

**Expected Outcome**: Program launches, displays 3D maze view, responds to input controls, exits cleanly.

**Failure Indicators**:
- Compilation errors
- Segmentation faults on startup
- Black/corrupted display
- Unresponsive controls

### Phase 2: Memory Safety Analysis

#### Valgrind Deep Analysis

**Objective**: Detect memory leaks, invalid memory access, and uninitialized value usage.

```bash
# Automated Valgrind test
make vtest

# Manual Valgrind execution for custom maps
make BUILD_TYPE=debug
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
         --suppressions=.valgrind-suppressions ./cub3d maps/valid/maze.cub
```

**Performance Impact**: 20-100x slowdown is normal for graphical applications.

**Critical Metrics**:
- **Definitely lost**: Direct memory leaks requiring immediate fixes
- **Indirectly lost**: Memory reachable only through lost blocks
- **Invalid read/write**: Buffer overruns, use-after-free errors
- **Uninitialized values**: Variables used before initialization

#### Address Sanitizer Analysis

**Objective**: Fast memory error detection during runtime execution.

```bash
# Automated ASan test
make asan-test

# Manual execution with custom environment
make BUILD_TYPE=asan
ASAN_OPTIONS="verbosity=1:abort_on_error=1:detect_leaks=1" ./cub3d maps/valid/gallery.cub
```

**Detects**:
- Heap buffer overflow/underflow
- Stack buffer overflow
- Use after free/return
- Double free
- Memory leaks (if `detect_leaks=1`)

### Phase 3: Undefined Behavior Detection

**Objective**: Identify subtle logic errors that may not cause immediate crashes.

```bash
make BUILD_TYPE=ubsan
./cub3d maps/valid/complex.cub
```

**Critical UB Categories**:
- **Signed integer overflow**: Math operations exceeding type limits
- **Array bounds violations**: Index calculations outside valid ranges
- **Null pointer dereference**: Accessing memory through null pointers
- **Division by zero**: Mathematical errors in calculations
- **Misaligned memory access**: Improper pointer arithmetic

### Phase 4: Combined Analysis

**Objective**: Maximum error detection coverage.

```bash
make BUILD_TYPE=combined
./cub3d maps/valid/stress_test.cub
```

**Use Case**: Final validation before submission or when debugging complex interactions between memory management and program logic.

## Systematic Edge Case Testing

### Map Parsing Edge Cases

Test these scenarios across all build configurations:

**Boundary Conditions**:
```bash
# Minimal valid map
./cub3d maps/valid/minimal_3x3.cub

# Large maps (performance/memory stress)
./cub3d maps/valid/monstermap.cub

# Irregular shapes
./cub3d maps/valid/irregular_shape.cub
```

**Error Handling**:
```bash
# Invalid file extensions
./cub3d maps/invalid/test.txt

# Missing textures
./cub3d maps/invalid/missing_texture.cub

# Malformed RGB values
./cub3d maps/invalid/invalid_colors.cub

# Open/unclosed maps
./cub3d maps/invalid/open_map.cub

# Invalid characters
./cub3d maps/invalid/bad_characters.cub
```

### Runtime Stress Testing

**Movement Boundary Testing**:
- Walk into all walls from different angles
- Rapid direction changes while moving
- Extended gameplay sessions (memory leak detection)

**Rendering Edge Cases**:
- Very close wall proximity (distance calculations)
- Looking directly at corners (texture coordinate edge cases)
- Rapid camera rotation (floating point precision)

## Tool-Specific Troubleshooting

### Valgrind Issues

**Problem**: "Invalid read of size X"
**Action**: Examine exact line number, check array bounds and pointer arithmetic

**Problem**: "Conditional jump depends on uninitialized value"
**Action**: Initialize all variables, especially in parsing functions

### ASan/UBSan Issues

**Problem**: Program terminates immediately with sanitizer error
**Action**: Run with `abort_on_error=0` to see multiple errors, fix systematically

**Problem**: Sanitizer reports in system libraries
**Action**: Focus on errors in your source code first, system library reports often false positives

## Documentation Standards

### Error Report Template

For each detected issue, document:

1. **Tool Used**: Valgrind/ASan/UBSan/Combined
2. **Reproduction Steps**: Exact command and input that triggers error
3. **Error Output**: Complete sanitizer/valgrind output
4. **Root Cause**: Specific code location and logic error
5. **Fix Applied**: Changes made to resolve issue
6. **Verification**: Confirmation that fix resolves issue without introducing new errors

### Results Log Structure

```
analysis_results/
├── valgrind_reports/
│   ├── basic_functionality.log
│   ├── edge_cases.log
│   └── stress_testing.log
├── sanitizer_reports/
│   ├── asan_results.log
│   ├── ubsan_results.log
│   └── combined_results.log
└── performance_benchmarks/
    ├── frame_rate_analysis.log
    └── memory_usage_profiles.log
```

## Success Criteria

**Memory Safety**: Zero Valgrind errors, zero ASan reports
**Logic Correctness**: Zero UBSan undefined behavior reports
**Error Handling**: Graceful failure for all invalid inputs
**Performance**: Stable framerate >30 FPS on target hardware
**Resource Management**: No memory leaks during extended execution

## Advanced Analysis (Optional)

### Performance Profiling

```bash
# Only after stability is confirmed
make BUILD_TYPE=release
perf record -g ./cub3d maps/valid/complex.cub
perf report
```

### Static Analysis

```bash
# Additional code quality checks
cppcheck --enable=all src/
scan-build make BUILD_TYPE=debug
```

---

**Remember**: The goal is not just to pass tests, but to understand exactly how your program behaves under all conditions. Every tool output is data - analyze it systematically.