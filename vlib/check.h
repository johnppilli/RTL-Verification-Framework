#pragma once
// Error accounting for a single test.
//
// Checks never abort the run: a testbench keeps driving after a mismatch so one
// failure surfaces every downstream symptom in a single pass. Reporting is
// capped so a broken DUT cannot bury the summary in scrollback.

#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <string>
#include <type_traits>

namespace vlib {

class Checker {
public:
    explicit Checker(std::string name = "", size_t max_reports = 20)
        : name_(std::move(name)), max_reports_(max_reports) {}

    bool check(bool ok, uint64_t cycle, const char* fmt, ...) {
        checks_++;
        if (ok) return true;
        va_list ap;
        va_start(ap, fmt);
        fail(cycle, fmt, ap);
        va_end(ap);
        return false;
    }

    // Compares an RTL output against the reference model. Both sides are
    // unsigned in every DUT here, so widening to unsigned long long is safe.
    template <typename A, typename B>
    bool eq(uint64_t cycle, const char* what, A rtl, B ref) {
        using C = typename std::common_type<A, B>::type;
        const bool ok = static_cast<C>(rtl) == static_cast<C>(ref);
        checks_++;
        if (!ok) {
            report(cycle, "%s mismatch: RTL=%llu REF=%llu", what,
                   static_cast<unsigned long long>(rtl),
                   static_cast<unsigned long long>(ref));
        }
        return ok;
    }

    void report(uint64_t cycle, const char* fmt, ...) {
        va_list ap;
        va_start(ap, fmt);
        fail(cycle, fmt, ap);
        va_end(ap);
    }

    size_t errors() const { return errors_; }
    size_t checks() const { return checks_; }

private:
    void fail(uint64_t cycle, const char* fmt, va_list ap) {
        errors_++;
        if (errors_ > max_reports_) {
            if (errors_ == max_reports_ + 1)
                std::printf("         ... further mismatches suppressed\n");
            return;
        }
        char buf[512];
        std::vsnprintf(buf, sizeof buf, fmt, ap);
        std::printf("         @%-6llu %s\n", static_cast<unsigned long long>(cycle), buf);
    }

    std::string name_;
    size_t max_reports_;
    size_t errors_ = 0;
    size_t checks_ = 0;
};

}  // namespace vlib
