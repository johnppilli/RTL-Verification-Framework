# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build Commands

```bash
# Build and run the counter simulation (default target)
make

# Build only (without running)
make build_counter

# Clean build artifacts
make clean
```

Note: The Makefile uses a hardcoded Zig path (`/Users/johnpilli/zig/zig-x86_64-macos-0.15.0-dev.1274+147a85280/zig`). Update the `ZIG` variable if using a different Zig installation.

## Architecture

This is an RTL verification framework that compares hardware designs against software reference models.

### Verification Flow

1. **RTL Design** (`rtl/*.sv`) - SystemVerilog modules being verified
2. **Reference Model** (`zig_src/*.zig`) - Zig implementations serving as "golden" behavioral models
3. **Testbench** (`sim/*.cpp`) - C++ testbenches that drive both RTL and reference model, comparing outputs

### Build Pipeline

1. Zig reference model compiles to object file (`zig build-obj`)
2. Verilator converts SystemVerilog RTL to C++ (`verilator --cc`)
3. C++ testbench links against both Verilator-generated code and Zig object file
4. Resulting executable runs cycle-accurate comparison tests

### Key Interfaces

Reference models export C-compatible functions that testbenches call via `extern "C"`:
- `*_init()` - Reset model state
- `*_tick()` - Advance one clock cycle
- `*_set_*()` - Set input signals
- `*_get_*()` - Read output values

### Directory Structure

- `rtl/` - SystemVerilog RTL designs
- `zig_src/` - Zig reference models (golden implementations)
- `sim/` - C++ testbenches and Verilator build output (`obj_dir/`)
- `tb/` - (Reserved for future testbench components)
