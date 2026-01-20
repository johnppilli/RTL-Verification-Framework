# RTL Verification Framework Makefile




#these lines are just all the different variables
VERILATOR = verilator
ZIG = /Users/johnpilli/zig/zig-x86_64-macos-0.15.0-dev.1274+147a85280/zig
ROOT_DIR = $(shell pwd)
RTL_DIR = rtl
SIM_DIR = sim
ZIG_DIR = zig_src

# Default target
all: run_counter #when you type make with no arguments in it, it only runs first target. all is dependent on run_counter, so it runs that

# Build Zig reference model
$(ZIG_DIR)/counter_model.o: $(ZIG_DIR)/counter_model.zig
	@echo "Building Zig reference model..."
	$(ZIG) build-obj -femit-bin=$(ZIG_DIR)/counter_model.o $(ZIG_DIR)/counter_model.zig

# Build and run counter testbench
run_counter: build_counter   #this depends on build_counter, then runs the executable 
	@echo "Running counter simulation..."
	@./sim/obj_dir/Vcounter

build_counter: $(ZIG_DIR)/counter_model.o #this depends on the Zig objects file, then runs verilator to convert RTL to C++ and compile everything
	@echo "Building counter testbench..."
	$(VERILATOR) --cc $(RTL_DIR)/counter.sv --exe $(SIM_DIR)/tb_counter.cpp \
		$(ROOT_DIR)/$(ZIG_DIR)/counter_model.o \
		--Mdir $(SIM_DIR)/obj_dir -CFLAGS "-I.."
	make -C $(SIM_DIR)/obj_dir -f Vcounter.mk Vcounter

# Build Zig FIFO reference model
$(ZIG_DIR)/fifo_model.o: $(ZIG_DIR)/fifo_model.zig
	@echo "Building Zig FIFO reference model..."
	$(ZIG) build-obj -femit-bin=$(ZIG_DIR)/fifo_model.o $(ZIG_DIR)/fifo_model.zig

# Build and run FIFO testbench
run_fifo: build_fifo
	@echo "Running FIFO simulation..."
	@./sim/obj_dir_fifo/Vfifo

build_fifo: $(ZIG_DIR)/fifo_model.o
	@echo "Building FIFO testbench..."
	$(VERILATOR) --cc $(RTL_DIR)/fifo.sv --exe $(SIM_DIR)/tb_fifo.cpp \
		$(ROOT_DIR)/$(ZIG_DIR)/fifo_model.o \
		--Mdir $(SIM_DIR)/obj_dir_fifo -CFLAGS "-I.."
	make -C $(SIM_DIR)/obj_dir_fifo -f Vfifo.mk Vfifo

clean:
	rm -rf $(SIM_DIR)/obj_dir
	rm -rf $(SIM_DIR)/obj_dir_fifo
	rm -f $(ZIG_DIR)/counter_model.o
	rm -f $(ZIG_DIR)/fifo_model.o

.PHONY: all run_counter build_counter run_fifo build_fifo clean
