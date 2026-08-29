#pragma once
// Test registry, CLI and runner.
//
// A testbench declares cases with VLIB_TEST and ends with VLIB_SUITE_MAIN.
// The runner owns the seed, the shared coverage group and the pass/fail
// summary, so every DUT gets identical reporting and identical CLI flags.

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

#include "verilated.h"

#include "vlib/check.h"
#include "vlib/coverage.h"
#include "vlib/random.h"

namespace vlib {

struct RunConfig {
    uint64_t seed = 1;
    // Length knob for randomized tests. Directed tests ignore it.
    uint64_t cycles = 2000;
    bool trace = false;
    std::string trace_dir = "build/waves";
    bool verbose = false;
    std::string filter;
    double cov_goal = 100.0;
};

struct TestContext {
    Random rng;
    Checker check;
    CoverGroup& cov;
    const RunConfig& cfg;
    std::string suite;
    std::string name;

    // Empty unless --trace was passed, which is what TestBench::open_trace
    // treats as "no dump", so tests can call it unconditionally.
    std::string wave_path() const {
        if (!cfg.trace) return "";
        return cfg.trace_dir + "/" + suite + "_" + name + ".vcd";
    }

    bool verbose() const { return cfg.verbose; }

    void log(const char* fmt, ...) const {
        if (!cfg.verbose) return;
        va_list ap;
        va_start(ap, fmt);
        std::printf("         ");
        std::vprintf(fmt, ap);
        va_end(ap);
        std::printf("\n");
    }
};

using TestFn = void (*)(TestContext&);

struct TestCase {
    const char* name;
    const char* desc;
    TestFn fn;
};

inline std::vector<TestCase>& registry() {
    static std::vector<TestCase> tests;
    return tests;
}

struct AutoRegister {
    explicit AutoRegister(const TestCase& tc) { registry().push_back(tc); }
};

inline uint64_t name_hash(const char* s) {
    uint64_t h = 1469598103934665603ULL;
    for (; *s; ++s) h = (h ^ static_cast<unsigned char>(*s)) * 1099511628211ULL;
    return h;
}

inline void usage(const char* prog) {
    std::printf(
        "usage: %s [options]\n"
        "  --seed N        stimulus seed (default 1); printed on every run\n"
        "  --test SUBSTR   only run tests whose name contains SUBSTR\n"
        "  --cycles N      length of randomized tests (default 2000)\n"
        "  --trace         dump VCD waveforms\n"
        "  --trace-dir D   where to put them (default build/waves)\n"
        "  --cov-goal P    required coverage percent for a full run (default 100)\n"
        "  --verbose       per-transaction logging\n"
        "  --list          list tests and exit\n",
        prog);
}

inline int run_all(const char* suite, int argc, char** argv) {
    Verilated::commandArgs(argc, argv);

    RunConfig cfg;
    bool list_only = false;

    for (int i = 1; i < argc; i++) {
        const std::string a = argv[i];
        auto value = [&](const char* what) -> const char* {
            if (i + 1 >= argc) {
                std::fprintf(stderr, "%s needs a value\n", what);
                std::exit(2);
            }
            return argv[++i];
        };
        if (a == "--seed") cfg.seed = std::strtoull(value("--seed"), nullptr, 0);
        else if (a == "--test") cfg.filter = value("--test");
        else if (a == "--cycles") cfg.cycles = std::strtoull(value("--cycles"), nullptr, 0);
        else if (a == "--trace") cfg.trace = true;
        else if (a == "--trace-dir") cfg.trace_dir = value("--trace-dir");
        else if (a == "--cov-goal") cfg.cov_goal = std::strtod(value("--cov-goal"), nullptr);
        else if (a == "--verbose" || a == "-v") cfg.verbose = true;
        else if (a == "--list") list_only = true;
        else if (a == "--help" || a == "-h") { usage(argv[0]); return 0; }
    }

    if (list_only) {
        for (const TestCase& tc : registry())
            std::printf("%-28s %s\n", tc.name, tc.desc ? tc.desc : "");
        return 0;
    }

    CoverGroup cov(std::string(suite) + " coverage");
    size_t passed = 0, failed = 0, skipped = 0;
    size_t total_errors = 0, total_checks = 0;

    std::printf("=== %s === seed %llu\n", suite, static_cast<unsigned long long>(cfg.seed));

    for (const TestCase& tc : registry()) {
        if (!cfg.filter.empty() && std::string(tc.name).find(cfg.filter) == std::string::npos) {
            skipped++;
            continue;
        }

        // Per-test seed derived from the run seed, so adding or filtering tests
        // does not shift the stimulus every other test sees.
        TestContext ctx{Random(cfg.seed ^ name_hash(tc.name)), Checker(tc.name), cov, cfg,
                        suite, tc.name};

        std::printf("[ RUN  ] %s\n", tc.name);
        tc.fn(ctx);

        total_errors += ctx.check.errors();
        total_checks += ctx.check.checks();

        if (ctx.check.errors() == 0) {
            passed++;
            std::printf("[  OK  ] %s (%zu checks)\n", tc.name, ctx.check.checks());
        } else {
            failed++;
            std::printf("[ FAIL ] %s (%zu of %zu checks failed)\n", tc.name,
                        ctx.check.errors(), ctx.check.checks());
        }
    }

    cov.report();

    const bool full_run = cfg.filter.empty();
    const bool cov_ok = !full_run || cov.closure() >= cfg.cov_goal - 1e-9;

    std::printf("\n=== %s: %zu passed, %zu failed", suite, passed, failed);
    if (skipped) std::printf(", %zu filtered out", skipped);
    std::printf(" | %zu checks, %zu errors | coverage %.1f%%",
                total_checks, total_errors, cov.closure());
    std::printf(" ===\n");

    if (!cov_ok)
        std::printf("COVERAGE SHORTFALL: %.1f%% < goal %.1f%%\n", cov.closure(), cfg.cov_goal);

    // Machine-readable line consumed by scripts/run_regression.sh.
    std::printf("VLIB_RESULT suite=%s seed=%llu passed=%zu failed=%zu errors=%zu coverage=%.1f\n",
                suite, static_cast<unsigned long long>(cfg.seed), passed, failed,
                total_errors, cov.closure());

    return (failed || !cov_ok) ? 1 : 0;
}

}  // namespace vlib

#define VLIB_TEST(NAME, DESC)                                                  \
    static void vlib_test_##NAME(::vlib::TestContext&);                        \
    static ::vlib::AutoRegister vlib_reg_##NAME{                               \
        ::vlib::TestCase{#NAME, DESC, &vlib_test_##NAME}};                     \
    static void vlib_test_##NAME(::vlib::TestContext& ctx)

#define VLIB_SUITE_MAIN(SUITE)                                                 \
    int main(int argc, char** argv) { return ::vlib::run_all(SUITE, argc, argv); }
