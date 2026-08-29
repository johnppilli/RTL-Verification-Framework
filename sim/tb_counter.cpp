// Counter testbench.
//
// The counter is small enough that the interesting question is not "does it
// count" but "does it hold under the conditions that are easy to get wrong":
// reset asserted mid-count, enable deasserted for arbitrary stretches, and the
// 255 -> 0 wrap.

#include "Vcounter.h"
#include "vlib/vlib.h"

extern "C" {
void counter_init();
void counter_tick();
void counter_set_reset(bool rst_n);
void counter_set_enable(bool enable);
unsigned char counter_get_count();
}

namespace {

// Wires a DUT and the reference model together so that every input goes to both
// and every output is compared on every cycle. Tests drive `enable` and tick.
class CounterEnv {
public:
    explicit CounterEnv(vlib::TestContext& ctx) : ctx_(ctx) {
        counter_init();
        tb_.open_trace(ctx.wave_path());
        tb_.on_reset([](bool rst_n) { counter_set_reset(rst_n); });
        tb_.on_posedge([] { counter_tick(); });
        tb_.on_cycle_end([this] { compare(); });
    }

    void set_enable(bool en) {
        tb_->enable = en;
        counter_set_enable(en);
        tb_.eval();
    }

    void reset(int cycles = 3) { tb_.reset(cycles); }
    void tick(int n = 1) { tb_.tick(n); }
    uint8_t count() const { return tb_.dut()->count; }
    uint64_t cycle() const { return tb_.cycle(); }

private:
    void compare() {
        const uint8_t rtl = tb_.dut()->count;
        const uint8_t ref = counter_get_count();
        ctx_.check.eq(tb_.cycle(), "count", rtl, ref);

        ctx_.cov.cover("count==0", rtl == 0);
        ctx_.cov.cover("count==255", rtl == 255);
        ctx_.cov.cover("enable low", !tb_.dut()->enable);
        ctx_.cov.cover("enable high", tb_.dut()->enable);
        if (has_prev_ && prev_ == 255 && rtl == 0) ctx_.cov.cover("wrap 255->0");
        prev_ = rtl;
        has_prev_ = true;
    }

    vlib::TestContext& ctx_;
    vlib::TestBench<Vcounter> tb_;
    uint8_t prev_ = 0;
    bool has_prev_ = false;
};

void declare_bins(vlib::CoverGroup& cov) {
    cov.bins({"count==0", "count==255", "enable low", "enable high",
              "wrap 255->0", "reset during count"});
}

}  // namespace

VLIB_TEST(reset_clears_count, "reset forces count to zero and holds it there") {
    declare_bins(ctx.cov);
    CounterEnv env(ctx);

    env.set_enable(true);
    env.reset(4);
    ctx.check.eq(env.cycle(), "count after reset", env.count(), 0);

    // Count up, then reset from a non-zero value.
    env.tick(7);
    ctx.check.check(env.count() != 0, env.cycle(), "counter never left zero");
    env.reset(2);
    ctx.cov.cover("reset during count");
    ctx.check.eq(env.cycle(), "count after mid-run reset", env.count(), 0);
}

VLIB_TEST(counts_when_enabled, "count increments once per enabled cycle") {
    declare_bins(ctx.cov);
    CounterEnv env(ctx);

    env.set_enable(true);
    env.reset(2);

    for (int i = 1; i <= 32; i++) {
        env.tick();
        ctx.check.eq(env.cycle(), "count", env.count(), i & 0xFF);
    }
}

VLIB_TEST(enable_gates_counting, "count is frozen while enable is low") {
    declare_bins(ctx.cov);
    CounterEnv env(ctx);

    env.set_enable(false);
    env.reset(2);
    env.tick(10);
    ctx.check.eq(env.cycle(), "count with enable low", env.count(), 0);

    env.set_enable(true);
    env.tick(5);
    const uint8_t held = env.count();
    ctx.check.eq(env.cycle(), "count after 5 enabled cycles", held, 5);

    env.set_enable(false);
    env.tick(20);
    ctx.check.eq(env.cycle(), "count still held", env.count(), held);
}

VLIB_TEST(wraps_at_max, "count rolls 255 -> 0 rather than saturating") {
    declare_bins(ctx.cov);
    CounterEnv env(ctx);

    env.set_enable(true);
    env.reset(2);
    env.tick(255);
    ctx.check.eq(env.cycle(), "count at max", env.count(), 255);

    env.tick();
    ctx.check.eq(env.cycle(), "count after wrap", env.count(), 0);

    env.tick();
    ctx.check.eq(env.cycle(), "count after wrap+1", env.count(), 1);
}

VLIB_TEST(random_enable, "randomized enable, compared against the model every cycle") {
    declare_bins(ctx.cov);
    CounterEnv env(ctx);

    env.set_enable(false);
    env.reset(2);

    const uint64_t n = ctx.cfg.cycles;
    for (uint64_t i = 0; i < n; i++) {
        env.set_enable(ctx.rng.chance(70));
        env.tick();
    }
    ctx.log("ran %llu randomized cycles", (unsigned long long)n);
}

VLIB_SUITE_MAIN("counter")
