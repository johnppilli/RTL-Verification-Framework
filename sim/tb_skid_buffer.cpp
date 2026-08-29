// Skid buffer testbench.
//
// A skid buffer is easy to get almost right, so the checks target the ways it
// fails in practice: dropping a beat that was accepted just as the consumer
// stalled, duplicating one, reordering, or quietly losing throughput. Every
// beat is scoreboarded, and handshake stability is asserted directly.

#include "Vskid_buffer.h"
#include "vlib/vlib.h"

extern "C" {
void skid_init();
void skid_tick();
void skid_set_reset(bool rst_n);
void skid_set_s_valid(bool v);
void skid_set_s_data(unsigned char d);
void skid_set_m_ready(bool r);
bool skid_get_s_ready();
bool skid_get_m_valid();
unsigned char skid_get_m_data();
}

namespace {

class SkidEnv {
public:
    explicit SkidEnv(vlib::TestContext& ctx, uint64_t max_latency = 0)
        : ctx_(ctx), sb_("skid beats", max_latency) {
        skid_init();
        tb_.open_trace(ctx.wave_path());
        tb_.on_reset([](bool rst_n) { skid_set_reset(rst_n); });
        tb_.on_posedge([] { skid_tick(); });
        tb_.on_cycle_end([this] { compare(); });
        drive(false, 0, false);
        tb_.reset(3);
    }

    // One cycle of the handshake. Returns true if the upstream beat was taken.
    bool step(bool s_valid, uint8_t s_data, bool m_ready) {
        drive(s_valid, s_data, m_ready);

        const bool accepted = s_valid && tb_.dut()->s_ready;
        const bool delivered = tb_.dut()->m_valid && m_ready;
        const uint8_t out = tb_.dut()->m_data;

        check_stability(s_valid, s_data, m_ready);
        sample_coverage(s_valid, m_ready, accepted, delivered);
        tb_.tick();

        if (accepted) {
            sb_.expect(s_data, tb_.cycle());
            accepted_++;
        }
        if (delivered) {
            sb_.observe(out, tb_.cycle());
            delivered_++;
        }
        return accepted;
    }

    void idle(int n) { for (int i = 0; i < n; i++) step(false, 0, false); }

    // Drains whatever is still inside with ready held high. Deliberately stops
    // offering upstream, so the producer-protocol self-check is stood down.
    void flush(int limit = 16) {
        draining_ = true;
        offered_valid_ = false;
        for (int i = 0; i < limit && tb_.dut()->m_valid; i++) step(false, 0, true);
    }

    void finish() {
        flush();
        if (!sb_.drained()) ctx_.check.report(tb_.cycle(), "beats went missing");
        ctx_.check.check(sb_.errors() == 0, tb_.cycle(), "scoreboard reported %zu error(s)",
                         sb_.errors());
        if (ctx_.verbose()) sb_.report();
    }

    Vskid_buffer* dut() { return tb_.dut(); }
    uint64_t cycle() const { return tb_.cycle(); }
    size_t accepted() const { return accepted_; }
    size_t delivered() const { return delivered_; }
    const vlib::LatencyStats& latency() const { return sb_.latency(); }

private:
    void drive(bool s_valid, uint8_t s_data, bool m_ready) {
        tb_->s_valid = s_valid;
        tb_->s_data = s_data;
        tb_->m_ready = m_ready;
        skid_set_s_valid(s_valid);
        skid_set_s_data(s_data);
        skid_set_m_ready(m_ready);
        skid_eval_model();
        tb_.eval();
    }

    // s_ready and m_valid come straight off registers, so the model needs no
    // combinational eval; this exists to keep the drive path symmetrical.
    static void skid_eval_model() {}

    // A valid output beat that was not accepted must still be there next cycle,
    // unchanged. This is the handshake rule most skid buffers break.
    void check_stability(bool s_valid, uint8_t s_data, bool m_ready) {
        if (held_valid_ && !held_ready_) {
            ctx_.check.check(tb_.dut()->m_valid, tb_.cycle(),
                             "m_valid dropped while the beat was unaccepted");
            ctx_.check.eq(tb_.cycle(), "stalled m_data", tb_.dut()->m_data, held_data_);
        }
        // An upstream beat offered but not accepted must be re-offered by the
        // producer; the testbench honours that, so record what was offered.
        if (offered_valid_ && !offered_ready_ && !draining_) {
            ctx_.check.check(s_valid && s_data == offered_data_, tb_.cycle(),
                             "testbench dropped an unaccepted upstream beat");
        }
        held_valid_ = tb_.dut()->m_valid;
        held_ready_ = m_ready;
        held_data_ = tb_.dut()->m_data;
        offered_valid_ = s_valid;
        offered_ready_ = tb_.dut()->s_ready;
        offered_data_ = s_data;
    }

    void sample_coverage(bool s_valid, bool m_ready, bool accepted, bool delivered) {
        vlib::CoverGroup& c = ctx_.cov;
        const bool s_ready = tb_.dut()->s_ready;
        const bool m_valid = tb_.dut()->m_valid;
        c.cover("upstream stalled (valid, !ready)", s_valid && !s_ready);
        c.cover("downstream stalled (valid, !ready)", m_valid && !m_ready);
        c.cover("skid slot occupied", !s_ready);
        c.cover("empty", !m_valid);
        c.cover("back-to-back transfer", accepted && delivered);
        c.cover("input only", accepted && !delivered);
        c.cover("output only", delivered && !accepted);
        c.cover("fully idle", !s_valid && !m_valid);
        c.cross("s_valid", "m_ready", s_valid, m_ready);
    }

    void compare() {
        ctx_.check.eq(tb_.cycle(), "s_ready", tb_.dut()->s_ready, skid_get_s_ready());
        ctx_.check.eq(tb_.cycle(), "m_valid", tb_.dut()->m_valid, skid_get_m_valid());
        if (tb_.dut()->m_valid)
            ctx_.check.eq(tb_.cycle(), "m_data", tb_.dut()->m_data, skid_get_m_data());
    }

    vlib::TestContext& ctx_;
    vlib::TestBench<Vskid_buffer> tb_;
    vlib::Scoreboard<uint8_t> sb_;
    size_t accepted_ = 0, delivered_ = 0;
    bool held_valid_ = false, held_ready_ = false;
    uint8_t held_data_ = 0;
    bool offered_valid_ = false, offered_ready_ = false;
    uint8_t offered_data_ = 0;
    bool draining_ = false;
};

void declare_bins(vlib::CoverGroup& cov) {
    cov.bins({"upstream stalled (valid, !ready)", "downstream stalled (valid, !ready)",
              "skid slot occupied", "empty", "back-to-back transfer", "input only",
              "output only", "fully idle", "s_valid x m_ready"});
}

}  // namespace

VLIB_TEST(reset_clears_handshake, "after reset the buffer is empty and accepting") {
    declare_bins(ctx.cov);
    SkidEnv env(ctx);

    ctx.check.eq(env.cycle(), "m_valid after reset", env.dut()->m_valid, 0);
    ctx.check.eq(env.cycle(), "s_ready after reset", env.dut()->s_ready, 1);
    env.idle(4);
    ctx.check.eq(env.cycle(), "m_valid while idle", env.dut()->m_valid, 0);
}

VLIB_TEST(single_beat_latency, "one beat in, one beat out, one cycle later") {
    declare_bins(ctx.cov);
    SkidEnv env(ctx, /*max_latency=*/1);

    for (int i = 0; i < 6; i++) {
        const uint8_t d = 0x30 + i;
        ctx.check.check(env.step(true, d, true), env.cycle(), "beat was not accepted");
        ctx.check.eq(env.cycle(), "m_valid one cycle later", env.dut()->m_valid, 1);
        ctx.check.eq(env.cycle(), "m_data", env.dut()->m_data, d);
        env.step(false, 0, true);
    }
    env.finish();
}

VLIB_TEST(full_throughput, "no backpressure means one beat per cycle, indefinitely") {
    declare_bins(ctx.cov);
    SkidEnv env(ctx);

    const int beats = 64;
    for (int i = 0; i < beats; i++)
        ctx.check.check(env.step(true, i & 0xFF, true), env.cycle(),
                        "stalled at beat %d with ready held high", i);

    env.finish();
    ctx.check.eq(env.cycle(), "beats accepted", env.accepted(), beats);
    ctx.check.eq(env.cycle(), "beats delivered", env.delivered(), beats);
}

VLIB_TEST(backpressure_preserves_beats, "a stalled consumer loses nothing") {
    declare_bins(ctx.cov);
    SkidEnv env(ctx);

    // Fill both slots, then hold ready low for a while.
    env.step(true, 0xA1, false);
    env.step(true, 0xA2, false);
    ctx.check.eq(env.cycle(), "s_ready once both slots are full", env.dut()->s_ready, 0);

    for (int i = 0; i < 10; i++) {
        env.step(false, 0, false);
        ctx.check.eq(env.cycle(), "m_valid held during stall", env.dut()->m_valid, 1);
        ctx.check.eq(env.cycle(), "m_data held during stall", env.dut()->m_data, 0xA1);
    }

    env.step(false, 0, true);
    ctx.check.eq(env.cycle(), "second beat follows", env.dut()->m_data, 0xA2);
    env.finish();
    ctx.check.eq(env.cycle(), "both beats delivered", env.delivered(), 2);
}

VLIB_TEST(intermittent_ready, "throughput survives a consumer that stalls every other cycle") {
    declare_bins(ctx.cov);
    SkidEnv env(ctx);

    int sent = 0;
    uint8_t next = 0;
    for (int i = 0; i < 200; i++) {
        const bool ready = (i % 2) == 0;
        if (env.step(true, next, ready)) {
            next++;
            sent++;
        }
    }
    env.finish();
    // A consumer accepting every other cycle should sustain about half rate.
    ctx.check.check(sent >= 90, env.cycle(), "only %d beats accepted in 200 cycles", sent);
    ctx.check.eq(env.cycle(), "all accepted beats delivered", env.delivered(), env.accepted());
}

VLIB_TEST(random_handshake, "randomized valid/ready with the producer honouring backpressure") {
    declare_bins(ctx.cov);
    SkidEnv env(ctx);

    uint8_t next = 0;
    bool offering = false;
    uint8_t offered = 0;

    for (uint64_t i = 0; i < ctx.cfg.cycles; i++) {
        // A real producer holds s_valid and s_data steady until accepted.
        if (!offering && ctx.rng.chance(70)) {
            offering = true;
            offered = next++;
        }
        const bool ready = ctx.rng.chance(60);
        if (env.step(offering, offered, ready)) offering = false;
    }

    env.finish();
    ctx.log("%zu accepted, %zu delivered, latency min/avg/max %llu/%.2f/%llu",
            env.accepted(), env.delivered(), (unsigned long long)env.latency().min,
            env.latency().mean(), (unsigned long long)env.latency().max);
}

VLIB_SUITE_MAIN("skid_buffer")
