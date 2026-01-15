# RTL Verification Framework Makefile

VERILATOR = verilator
ZIG = /Users/johnpilli/zig/zig-x86_64-macos-0.15.0-dev.1274+147a85280/zig
ROOT_DIR = $(shell pwd)
RTL_DIR = rtl
SIM_DIR = sim
ZIG_DIR = zig_src

# Default target
all: run_counter

# Build Zig reference model
$(ZIG_DIR)/counter_model.o: $(ZIG_DIR)/counter_model.zig
	@echo "Building Zig reference model..."
	$(ZIG) build-obj -femit-bin=$(ZIG_DIR)/counter_model.o $(ZIG_DIR)/counter_model.zig

# Build and run counter testbench
run_counter: build_counter
	@echo "Running counter simulation..."
	@./sim/obj_dir/Vcounter

build_counter: $(ZIG_DIR)/counter_model.o
	@echo "Building counter testbench..."
	$(VERILATOR) --cc $(RTL_DIR)/counter.sv --exe $(SIM_DIR)/tb_counter.cpp \
		$(ROOT_DIR)/$(ZIG_DIR)/counter_model.o \
		--Mdir $(SIM_DIR)/obj_dir -CFLAGS "-I.."
	make -C $(SIM_DIR)/obj_dir -f Vcounter.mk Vcounter

clean:
	rm -rf $(SIM_DIR)/obj_dir
	rm -f $(ZIG_DIR)/counter_model.o

.PHONY: all run_counter build_counter clean
