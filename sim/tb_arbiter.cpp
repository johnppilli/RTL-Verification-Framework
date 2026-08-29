// Round-robin arbiter testbench.
//
// Matching the reference model proves the arbiter picks the same winner. It
// does not prove the property that actually matters for an arbiter, which is
// that no requester can be starved, so that is checked directly against a
// bound derived from the design: a continuously asserted request waits at most
// N-1 cycles, because the priority pointer passes every other position at most
// once before reaching it.

#include "Varbiter.h"
#include "vlib/vlib.h"

extern "C" {
void arbiter_init();
void arbiter_tick();
void arbiter_eval();
void arbiter_set_reset(bool rst_n);
void arbiter_set_req(uint32_t req);
uint32_t arbiter_get_grant();
bool arbiter_get_valid();
size_t arbiter_get_ptr();
size_t arbiter_width();
}

namespace {

constexpr int kN = 4;
constexpr uint32_t kAllReq = (1u << kN) - 1;

int index_of(uint32_t onehot) {
    for (int i = 0; i < kN; i++)
        if (onehot & (1u << i)) return i;
    return -1;
}

class ArbiterEnv {
public:
    explicit ArbiterEnv(vlib::TestContext& ctx) : ctx_(ctx) {
        arbiter_init();
        tb_.open_trace(ctx.wave_path());
        tb_.on_reset([](bool rst_n) { arbiter_set_reset(rst_n); });
        tb_.on_posedge([] { arbiter_tick(); });
        set_req(0);
        tb_.reset(3);
        ctx_.check.eq(tb_.cycle(), "N (model vs RTL parameter)", arbiter_width(), kN);
    }

    void set_req(uint32_t req) {
        req_ = req;
        tb_->req = req;
        arbiter_set_req(req);
        arbiter_eval();
        tb_.eval();
    }

    // Applies the current request vector for one cycle and returns the grant.
    uint32_t step() {
        check_combinational();
        const uint32_t grant = tb_.dut()->grant;
        track_starvation(grant);
        sample_coverage(grant);
        tb_.tick();
        if (grant) grants_[index_of(grant)]++;
        return grant;
    }

    uint32_t step(uint32_t req) {
        set_req(req);
        return step();
    }

    int grants(int i) const { return grants_[i]; }
    uint64_t cycle() const { return tb_.cycle(); }
    Varbiter* dut() { return tb_.dut(); }
    int worst_wait() const { return worst_wait_; }

private:
    // grant is combinational off req, so it is compared before the edge.
    void check_combinational() {
        const uint32_t rtl_grant = tb_.dut()->grant;
        ctx_.check.eq(tb_.cycle(), "grant", rtl_grant, arbiter_get_grant());
        ctx_.check.eq(tb_.cycle(), "valid", tb_.dut()->valid, arbiter_get_valid());

        // Properties that hold regardless of what the model says.
        ctx_.check.check((rtl_grant & ~req_) == 0, tb_.cycle(),
                         "granted an idle requester: grant=0x%x req=0x%x", rtl_grant, req_);
        ctx_.check.check((rtl_grant & (rtl_grant - 1)) == 0, tb_.cycle(),
                         "grant is not one-hot: 0x%x", rtl_grant);
        ctx_.check.check((rtl_grant != 0) == (req_ != 0), tb_.cycle(),
                         "grant/req disagree: grant=0x%x req=0x%x", rtl_grant, req_);
    }

    void track_starvation(uint32_t grant) {
        for (int i = 0; i < kN; i++) {
            const bool asking = (req_ >> i) & 1;
            const bool got = (grant >> i) & 1;
            if (!asking || got) {
                waiting_[i] = 0;
                continue;
            }
            waiting_[i]++;
            if (waiting_[i] > worst_wait_) worst_wait_ = waiting_[i];
            ctx_.check.check(waiting_[i] < kN, tb_.cycle(),
                             "requester %d starved for %d cycles (bound is %d)", i,
                             waiting_[i], kN - 1);
        }
    }

    void sample_coverage(uint32_t grant) {
        vlib::CoverGroup& c = ctx_.cov;
        c.cover("no requests", req_ == 0);
        c.cover("single request", req_ && (req_ & (req_ - 1)) == 0);
        c.cover("all requesting", req_ == kAllReq);
        if (grant) c.cover_value("granted", index_of(grant));
        c.cover("contention", req_ && (req_ & (req_ - 1)) != 0);
        for (int i = 0; i < kN; i++)
            if (waiting_[i] == kN - 1) c.cover("worst-case wait reached");
    }

    vlib::TestContext& ctx_;
    vlib::TestBench<Varbiter> tb_;
    uint32_t req_ = 0;
    int grants_[kN] = {0, 0, 0, 0};
    int waiting_[kN] = {0, 0, 0, 0};
    int worst_wait_ = 0;
};

void declare_bins(vlib::CoverGroup& cov) {
    cov.bins({"no requests", "single request", "all requesting", "contention",
              "worst-case wait reached"});
    cov.value_bins("granted", 0, kN - 1);
}

}  // namespace

VLIB_TEST(idle_grants_nothing, "no request means no grant") {
    declare_bins(ctx.cov);
    ArbiterEnv env(ctx);

    for (int i = 0; i < 8; i++) {
        const uint32_t g = env.step(0);
        ctx.check.eq(env.cycle(), "grant while idle", g, 0);
        ctx.check.eq(env.cycle(), "valid while idle", env.dut()->valid, 0);
    }
}

VLIB_TEST(single_requester_always_wins, "an uncontended requester is granted every cycle") {
    declare_bins(ctx.cov);
    ArbiterEnv env(ctx);

    for (int i = 0; i < kN; i++) {
        for (int c = 0; c < 5; c++) {
            const uint32_t g = env.step(1u << i);
            ctx.check.eq(env.cycle(), "grant", g, 1u << i);
        }
    }
}

VLIB_TEST(rotates_under_full_load, "with everyone asking, grants cycle through in order") {
    declare_bins(ctx.cov);
    ArbiterEnv env(ctx);

    env.set_req(kAllReq);
    const int first = index_of(env.step());

    // Each subsequent grant must be the next index round the ring.
    for (int i = 1; i <= 4 * kN; i++) {
        const int got = index_of(env.step());
        const int want = (first + i) % kN;
        ctx.check.eq(env.cycle(), "rotating grant index", got, want);
    }

    // Over a whole number of laps every requester is served equally.
    for (int i = 0; i < kN; i++)
        ctx.check.check(env.grants(i) >= 4, env.cycle(),
                        "requester %d got only %d grants", i, env.grants(i));
}

VLIB_TEST(no_starvation_under_contention, "a held request is granted within N-1 cycles") {
    declare_bins(ctx.cov);
    ArbiterEnv env(ctx);

    // Requester 0 holds forever while the others come and go.
    for (uint64_t i = 0; i < ctx.cfg.cycles; i++) {
        uint32_t req = 1u;
        for (int b = 1; b < kN; b++)
            if (ctx.rng.chance(70)) req |= (1u << b);
        env.step(req);
    }

    ctx.check.check(env.grants(0) > 0, env.cycle(), "held requester was never granted");
    ctx.log("worst observed wait: %d cycles (bound %d)", env.worst_wait(), kN - 1);
}

VLIB_TEST(random_stress, "randomized request patterns against the model") {
    declare_bins(ctx.cov);
    ArbiterEnv env(ctx);

    for (uint64_t i = 0; i < ctx.cfg.cycles; i++) {
        // Weighted so idle, single-requester and full contention all appear
        // often enough to close coverage.
        uint32_t req;
        switch (ctx.rng.weighted({1, 3, 4, 2})) {
            case 0:  req = 0; break;
            case 1:  req = 1u << ctx.rng.range(0, kN - 1); break;
            case 2:  req = ctx.rng.u32() & kAllReq; break;
            default: req = kAllReq; break;
        }
        env.step(req);
    }
}

VLIB_SUITE_MAIN("arbiter")
