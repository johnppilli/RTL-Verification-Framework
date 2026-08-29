#pragma once
// Verilated model wrapper: clocking, reset and waveform dumping.
//
// Ports are found by name, so a DUT is expected to call its clock `clk` and its
// active-low reset `rst_n`. A DUT with neither (a combinational block) still
// works; tick() and reset() are simply unavailable on it, enforced at compile
// time rather than by convention.

#include <cstdint>
#include <functional>
#include <string>
#include <type_traits>

#include "verilated.h"
#include "verilated_vcd_c.h"

namespace vlib {

template <typename T, typename = void>
struct has_clk : std::false_type {};
template <typename T>
struct has_clk<T, decltype(void(std::declval<T&>().clk))> : std::true_type {};

template <typename T, typename = void>
struct has_rst_n : std::false_type {};
template <typename T>
struct has_rst_n<T, decltype(void(std::declval<T&>().rst_n))> : std::true_type {};

template <typename DUT>
class TestBench {
public:
    TestBench() {
        dut_ = new DUT;
        if constexpr (has_clk<DUT>::value) dut_->clk = 0;
        if constexpr (has_rst_n<DUT>::value) dut_->rst_n = 0;
        dut_->eval();
    }

    ~TestBench() {
        close_trace();
        dut_->final();
        delete dut_;
    }

    TestBench(const TestBench&) = delete;
    TestBench& operator=(const TestBench&) = delete;

    DUT* operator->() { return dut_; }
    DUT* dut() { return dut_; }
    uint64_t cycle() const { return cycle_; }

    // Runs after the rising edge has propagated through the RTL. This is where
    // the reference model is advanced, so both see the same inputs.
    void on_posedge(std::function<void()> f) { posedge_ = std::move(f); }

    // Runs once the cycle has settled. This is where outputs are compared.
    void on_cycle_end(std::function<void()> f) { cycle_end_ = std::move(f); }

    // Drives reset on the DUT and, through this hook, on the model too.
    void on_reset(std::function<void(bool)> f) { set_reset_ = std::move(f); }

    void eval() {
        dut_->eval();
        dump();
    }

    void tick(int n = 1) {
        static_assert(has_clk<DUT>::value, "tick() requires a DUT with a clk port");
        for (int i = 0; i < n; i++) {
            dut_->clk = 1;
            dut_->eval();
            if (posedge_) posedge_();
            dump();

            dut_->clk = 0;
            dut_->eval();
            cycle_++;
            dump();

            if (cycle_end_) cycle_end_();
        }
    }

    // Holds reset for `cycles`, then releases it. Comparison hooks stay live
    // throughout so a model that resets differently is caught here.
    void reset(int cycles = 5) {
        static_assert(has_rst_n<DUT>::value, "reset() requires a DUT with a rst_n port");
        dut_->rst_n = 0;
        if (set_reset_) set_reset_(false);
        eval();
        tick(cycles);
        dut_->rst_n = 1;
        if (set_reset_) set_reset_(true);
        eval();
    }

    void open_trace(const std::string& path) {
        if (path.empty() || trace_) return;
        Verilated::traceEverOn(true);
        trace_ = new VerilatedVcdC;
        dut_->trace(trace_, 99);
        trace_->open(path.c_str());
        trace_open_ = true;
        dump();
    }

    void close_trace() {
        if (!trace_) return;
        if (trace_open_) trace_->close();
        delete trace_;
        trace_ = nullptr;
        trace_open_ = false;
    }

private:
    void dump() {
        if (trace_open_) trace_->dump(time_ns_);
        time_ns_ += 5;
    }

    DUT* dut_ = nullptr;
    VerilatedVcdC* trace_ = nullptr;
    bool trace_open_ = false;
    uint64_t time_ns_ = 0;
    uint64_t cycle_ = 0;
    std::function<void()> posedge_;
    std::function<void()> cycle_end_;
    std::function<void(bool)> set_reset_;
};

}  // namespace vlib
