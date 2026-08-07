# RTL Verification Framework

Checks SystemVerilog hardware designs against independent Zig reference models,
running both side by side and comparing their behavior cycle-by-cycle. Verilator
compiles the RTL to C++, and a C++ testbench drives both the RTL and the Zig
"golden" model with identical inputs, catching any mismatch.

Two modules are verified: an 8-bit counter and a parameterized synchronous FIFO
(the FIFO testbench also tracks write-to-read latency, not just data correctness).

## Prerequisites

- [Verilator](https://www.veripool.org/verilator/) (`brew install verilator` on macOS)
- [Zig](https://ziglang.org/download/) — built against `0.15.0-dev`, but any
  reasonably recent version should work

**Before building:** open the `Makefile` and update the `ZIG` variable to point
at your own Zig binary:

```makefile
ZIG = /path/to/your/zig
```

## Build & run

```bash
make            # builds and runs the counter verification (default)
make run_fifo   # builds and runs the FIFO verification
```

A passing run means every cycle-by-cycle comparison between the RTL and the
reference model matched, for the whole simulation.

## Clean

```bash
make clean
```

## Structure

- `rtl/` — SystemVerilog designs being verified
- `zig_src/` — Zig reference models (the "golden" behavior)
- `sim/` — C++ testbenches that drive both and compare outputs
