# RTL Verification Framework
#
#   make              build and run every testbench
#   make <dut>        build and run one (counter fifo arbiter skid_buffer alu)
#   make build        build everything, run nothing
#   make regress      multi-seed regression across all DUTs
#   make lint         Verilator lint pass over the RTL
#   make waves DUT=fifo   re-run one DUT dumping VCD into build/waves
#   make clean
#
# Zig is picked up from PATH, or from ~/zig*/zig. Override if it lives
# elsewhere:  make ZIG=/path/to/zig

VERILATOR ?= verilator
ZIG       ?= $(or $(shell command -v zig 2>/dev/null),$(firstword $(wildcard $(HOME)/zig*/zig)),zig)

DUTS := counter fifo arbiter skid_buffer alu

ROOT  := $(abspath .)
BUILD := build
OBJ   := $(BUILD)/obj
WAVES := $(BUILD)/waves

# Passed through to every simulation. SEED is printed on each run and replays
# a failure exactly.
SEED     ?= 1
SIMFLAGS ?=
DUT      ?= fifo

VLIB_HDRS := $(wildcard vlib/*.h)
BINS      := $(foreach d,$(DUTS),$(BUILD)/$(d)/V$(d))

# SYNCASYNCNET is suppressed because `disable iff (!rst_n)` in the assertions
# counts as a synchronous use of the async reset. It is an artifact of the SVA,
# not of the design.
VFLAGS := --cc --exe --build -j 0 \
          --assert +define+SIM_ASSERT \
          --trace \
          -Wall -Wno-SYNCASYNCNET \
          -CFLAGS "-std=c++17 -O2 -I$(ROOT)"

# On macOS, Zig defaults to the running OS version while clang links against
# the SDK's (older) deployment target, which makes ld warn on every link.
# Pinning Zig to the platform default minimum keeps the two compatible.
ifeq ($(shell uname -s),Darwin)
  ZIGFLAGS ?= -target $(shell uname -m | sed 's/arm64/aarch64/')-macos
else
  ZIGFLAGS ?=
endif

.PHONY: all build test regress lint waves clean $(DUTS)

all: test

build: $(BINS)

$(OBJ)/%_model.o: zig_src/%_model.zig
	@mkdir -p $(OBJ)
	@echo "  ZIG       $<"
	@$(ZIG) build-obj $(ZIGFLAGS) --cache-dir $(BUILD)/zig-cache -femit-bin=$@ $<

define DUT_RULE
$$(BUILD)/$(1)/V$(1): rtl/$(1).sv sim/tb_$(1).cpp $$(OBJ)/$(1)_model.o $$(VLIB_HDRS)
	@echo "  VERILATE  $(1)"
	@$$(VERILATOR) $$(VFLAGS) --Mdir $$(BUILD)/$(1) -o V$(1) --top-module $(1) \
		rtl/$(1).sv $$(ROOT)/sim/tb_$(1).cpp $$(ROOT)/$$(OBJ)/$(1)_model.o

$(1): $$(BUILD)/$(1)/V$(1)
	@./$$(BUILD)/$(1)/V$(1) --seed $$(SEED) $$(SIMFLAGS)
endef
$(foreach d,$(DUTS),$(eval $(call DUT_RULE,$(d))))

test: build
	@fail=0; \
	for d in $(DUTS); do \
	  ./$(BUILD)/$$d/V$$d --seed $(SEED) $(SIMFLAGS) || fail=1; \
	  echo; \
	done; \
	if [ $$fail -ne 0 ]; then echo "REGRESSION FAILED"; fi; \
	exit $$fail

regress: build
	@bash scripts/run_regression.sh

lint:
	@for d in $(DUTS); do \
	  echo "  LINT      $$d"; \
	  $(VERILATOR) --lint-only -Wall -Wno-SYNCASYNCNET --assert +define+SIM_ASSERT \
	    --top-module $$d rtl/$$d.sv || exit 1; \
	done

waves:
	@mkdir -p $(WAVES)
	@$(MAKE) --no-print-directory $(BUILD)/$(DUT)/V$(DUT)
	@./$(BUILD)/$(DUT)/V$(DUT) --seed $(SEED) --trace --trace-dir $(WAVES) $(SIMFLAGS)
	@echo "VCDs written to $(WAVES)/"

clean:
	rm -rf $(BUILD) zig_src/.zig-cache
