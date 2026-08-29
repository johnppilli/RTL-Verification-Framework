// FIFO testbench.
//
// Two independent checks run on every test. The cycle-by-cycle comparison
// against the Zig model catches wrong flags and wrong counts; the scoreboard
// catches the thing a per-cycle comparison cannot see, which is data coming out
// in the wrong order or not coming out at all.

#include "Vfifo.h"
#include "vlib/vlib.h"

extern "C" {
void fifo_init();
void fifo_tick();
void fifo_set_reset(bool rst_n);
void fifo_set_wr_en(bool wr_en);
void fifo_set_rd_en(bool rd_en);
void fifo_set_data_in(unsigned char data);
unsigned char fifo_get_data_out();
bool fifo_get_full();
bool fifo_get_empty();
size_t fifo_get_count();
size_t fifo_depth();
}

namespace {

constexpr int kDepth = 8;

class FifoEnv {
public:
    explicit FifoEnv(vlib::TestContext& ctx, uint64_t max_latency = 0)
        : ctx_(ctx), sb_("fifo data", max_latency) {
        fifo_init();
        tb_.open_trace(ctx.wave_path());
        tb_.on_reset([](bool rst_n) { fifo_set_reset(rst_n); });
        tb_.on_posedge([] { fifo_tick(); });
        tb_.on_cycle_end([this] { compare(); });
        drive(false, 0, false);
        tb_.reset(3);
        ctx_.check.eq(tb_.cycle(), "depth (model vs RTL parameter)", fifo_depth(), kDepth);
    }

    // One cycle with the given stimulus. Returns the value read, if any.
    //
    // data_out is combinational off the read pointer, so the beat a read
    // consumes is the one visible before the edge, not after it.
    void step(bool wr_en, uint8_t data, bool rd_en) {
        drive(wr_en, data, rd_en);

        const bool full = tb_.dut()->full;
        const bool empty = tb_.dut()->empty;
        const bool will_write = wr_en && !full;
        const bool will_read = rd_en && !empty;
        const uint8_t head = tb_.dut()->data_out;

        sample_coverage(wr_en, rd_en, full, empty);
        tb_.tick();

        // Both are stamped after the edge, so latency reads as the number of
        // cycles the beat actually sat in the FIFO.
        if (will_write) sb_.expect(data, tb_.cycle());
        if (will_read) {
            sb_.observe(head, tb_.cycle());
            last_read_ = head;
        }
        if (will_write) writes_++;
        if (will_read) reads_++;
    }

    void write(uint8_t d) { step(true, d, false); }
    void read() { step(false, 0, true); }
    void idle(int n = 1) { for (int i = 0; i < n; i++) step(false, 0, false); }

    void finish() {
        if (!sb_.drained()) ctx_.check.report(tb_.cycle(), "scoreboard did not drain");
        ctx_.check.check(sb_.errors() == 0, tb_.cycle(), "scoreboard reported %zu error(s)",
                         sb_.errors());
        if (ctx_.verbose()) sb_.report();
    }

    Vfifo* dut() { return tb_.dut(); }
    uint64_t cycle() const { return tb_.cycle(); }
    uint8_t last_read() const { return last_read_; }
    size_t writes() const { return writes_; }
    size_t reads() const { return reads_; }
    vlib::Scoreboard<uint8_t>& sb() { return sb_; }
    // Reset discards whatever was in flight, so the scoreboard is dropped too.
    void reset(int n = 3) {
        tb_.reset(n);
        sb_.clear();
    }

private:
    void drive(bool wr_en, uint8_t data, bool rd_en) {
        tb_->wr_en = wr_en;
        tb_->rd_en = rd_en;
        tb_->data_in = data;
        fifo_set_wr_en(wr_en);
        fifo_set_rd_en(rd_en);
        fifo_set_data_in(data);
        tb_.eval();
    }

    void sample_coverage(bool wr_en, bool rd_en, bool full, bool empty) {
        vlib::CoverGroup& c = ctx_.cov;
        c.cover("empty", empty);
        c.cover("full", full);
        c.cover("write while full", wr_en && full);
        c.cover("read while empty", rd_en && empty);
        c.cover("simultaneous r/w", wr_en && rd_en);
        c.cover("idle", !wr_en && !rd_en);
        c.cover_value("count", tb_.dut()->count);
        if (wr_ptr_wrapped()) c.cover("write pointer wrap");
        if (rd_ptr_wrapped()) c.cover("read pointer wrap");
    }

    // The pointers are internal, so wrap is inferred from the observable
    // sequence: DEPTH accepted writes (or reads) since the last wrap.
    bool wr_ptr_wrapped() {
        if (!(tb_->wr_en && !tb_.dut()->full)) return false;
        return (++wr_beats_ % kDepth) == 0;
    }
    bool rd_ptr_wrapped() {
        if (!(tb_->rd_en && !tb_.dut()->empty)) return false;
        return (++rd_beats_ % kDepth) == 0;
    }

    void compare() {
        Vfifo* d = tb_.dut();
        ctx_.check.eq(tb_.cycle(), "full", d->full, fifo_get_full());
        ctx_.check.eq(tb_.cycle(), "empty", d->empty, fifo_get_empty());
        ctx_.check.eq(tb_.cycle(), "count", d->count, fifo_get_count());
        // data_out is undefined while empty; the RTL exposes stale memory.
        if (!d->empty)
            ctx_.check.eq(tb_.cycle(), "data_out", d->data_out, fifo_get_data_out());
    }

    vlib::TestContext& ctx_;
    vlib::TestBench<Vfifo> tb_;
    vlib::Scoreboard<uint8_t> sb_;
    uint8_t last_read_ = 0;
    size_t writes_ = 0, reads_ = 0;
    size_t wr_beats_ = 0, rd_beats_ = 0;
};

void declare_bins(vlib::CoverGroup& cov) {
    cov.bins({"empty", "full", "write while full", "read while empty",
              "simultaneous r/w", "idle", "write pointer wrap", "read pointer wrap"});
    cov.value_bins("count", 0, kDepth);
}

}  // namespace

VLIB_TEST(reset_empties_fifo, "reset clears the count and both flags settle") {
    declare_bins(ctx.cov);
    FifoEnv env(ctx);

    ctx.check.eq(env.cycle(), "empty after reset", env.dut()->empty, 1);
    ctx.check.eq(env.cycle(), "full after reset", env.dut()->full, 0);
    ctx.check.eq(env.cycle(), "count after reset", env.dut()->count, 0);

    // Reset from a partially filled FIFO, not just from power-on.
    for (int i = 0; i < 5; i++) env.write(0xA0 + i);
    ctx.check.eq(env.cycle(), "count before reset", env.dut()->count, 5);
    env.reset(2);
    ctx.check.eq(env.cycle(), "count after mid-run reset", env.dut()->count, 0);
    ctx.check.eq(env.cycle(), "empty after mid-run reset", env.dut()->empty, 1);
    env.finish();
}

VLIB_TEST(fill_then_drain, "fill to full, drain to empty, data comes back in order") {
    declare_bins(ctx.cov);
    FifoEnv env(ctx);

    for (int i = 0; i < kDepth; i++) {
        env.write(0x10 + i);
        ctx.check.eq(env.cycle(), "count while filling", env.dut()->count, i + 1);
    }
    ctx.check.eq(env.cycle(), "full at depth", env.dut()->full, 1);

    for (int i = 0; i < kDepth; i++) {
        env.read();
        ctx.check.eq(env.cycle(), "read value", env.last_read(), 0x10 + i);
        ctx.check.eq(env.cycle(), "count while draining", env.dut()->count, kDepth - i - 1);
    }
    ctx.check.eq(env.cycle(), "empty after drain", env.dut()->empty, 1);
    env.finish();
}

VLIB_TEST(overflow_is_ignored, "writing to a full FIFO changes nothing") {
    declare_bins(ctx.cov);
    FifoEnv env(ctx);

    for (int i = 0; i < kDepth; i++) env.write(i + 1);
    ctx.check.eq(env.cycle(), "full", env.dut()->full, 1);

    const uint8_t head = env.dut()->data_out;
    for (int i = 0; i < 4; i++) env.write(0xFF);
    ctx.check.eq(env.cycle(), "count after overflow attempts", env.dut()->count, kDepth);
    ctx.check.eq(env.cycle(), "head unchanged by overflow", env.dut()->data_out, head);

    // The rejected writes must not have entered the queue.
    for (int i = 0; i < kDepth; i++) {
        env.read();
        ctx.check.eq(env.cycle(), "value survived overflow attempts", env.last_read(), i + 1);
    }
    env.finish();
}

VLIB_TEST(underflow_is_ignored, "reading an empty FIFO changes nothing") {
    declare_bins(ctx.cov);
    FifoEnv env(ctx);

    for (int i = 0; i < 5; i++) env.read();
    ctx.check.eq(env.cycle(), "still empty", env.dut()->empty, 1);
    ctx.check.eq(env.cycle(), "count still zero", env.dut()->count, 0);

    env.write(0x42);
    env.read();
    ctx.check.eq(env.cycle(), "value after underflow attempts", env.last_read(), 0x42);
    env.finish();
}

VLIB_TEST(simultaneous_read_write, "count holds steady when a read and write pair up") {
    declare_bins(ctx.cov);
    FifoEnv env(ctx);

    for (int i = 0; i < 4; i++) env.write(0x50 + i);
    const int held = env.dut()->count;

    for (int i = 0; i < 8; i++) {
        env.step(true, 0x60 + i, true);
        ctx.check.eq(env.cycle(), "count during paired r/w", env.dut()->count, held);
    }

    while (!env.dut()->empty) env.read();
    env.finish();
}

VLIB_TEST(single_entry_latency, "a lone beat is readable the cycle after it lands") {
    declare_bins(ctx.cov);
    FifoEnv env(ctx, /*max_latency=*/1);

    for (int i = 0; i < 6; i++) {
        env.write(0x70 + i);
        ctx.check.eq(env.cycle(), "not empty after write", env.dut()->empty, 0);
        env.read();
        ctx.check.eq(env.cycle(), "value", env.last_read(), 0x70 + i);
        ctx.check.eq(env.cycle(), "empty again", env.dut()->empty, 1);
    }
    env.finish();
}

VLIB_TEST(pointer_rollover, "pointers wrap cleanly past the end of memory") {
    declare_bins(ctx.cov);
    FifoEnv env(ctx);

    // Three laps around an 8-entry memory at varying occupancy.
    for (int lap = 0; lap < 3; lap++) {
        for (int i = 0; i < kDepth; i++) env.write(lap * 16 + i);
        for (int i = 0; i < kDepth; i++) {
            env.read();
            ctx.check.eq(env.cycle(), "value after wrap", env.last_read(), lap * 16 + i);
        }
    }
    ctx.check.check(env.writes() == 3 * kDepth, env.cycle(), "expected %d writes, got %zu",
                    3 * kDepth, env.writes());
    env.finish();
}

VLIB_TEST(random_stress, "randomized traffic against the model and the scoreboard") {
    declare_bins(ctx.cov);
    FifoEnv env(ctx);

    const uint64_t n = ctx.cfg.cycles;
    for (uint64_t i = 0; i < n; i++) {
        // Deliberately unconditional: wr_en while full and rd_en while empty are
        // exactly the cases the DUT has to reject on its own.
        const bool wr = ctx.rng.chance(55);
        const bool rd = ctx.rng.chance(45);
        env.step(wr, ctx.rng.u8(), rd);
    }

    while (!env.dut()->empty) env.read();
    env.finish();
    ctx.log("%zu writes, %zu reads over %llu cycles", env.writes(), env.reads(),
            (unsigned long long)n);
}

VLIB_SUITE_MAIN("fifo")
