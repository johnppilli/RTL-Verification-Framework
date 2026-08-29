#pragma once
// In-order scoreboard with latency tracking.
//
// Cycle-by-cycle output comparison proves the DUT matches the model right now;
// it does not prove data ordering survives a queue. The scoreboard closes that
// gap: items pushed with expect() must reappear from observe() in the same
// order, and the cycle gap between the two is accumulated as latency.

#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <deque>
#include <string>
#include <type_traits>

namespace vlib {

template <typename T>
inline typename std::enable_if<std::is_integral<T>::value, std::string>::type
to_str(T v) {
    char b[48];
    std::snprintf(b, sizeof b, "%llu (0x%llx)",
                  static_cast<unsigned long long>(v),
                  static_cast<unsigned long long>(v));
    return b;
}

struct LatencyStats {
    uint64_t samples = 0;
    uint64_t total = 0;
    uint64_t min = UINT64_MAX;
    uint64_t max = 0;

    void add(uint64_t l) {
        samples++;
        total += l;
        if (l < min) min = l;
        if (l > max) max = l;
    }

    double mean() const { return samples ? static_cast<double>(total) / samples : 0.0; }
};

template <typename T>
class Scoreboard {
public:
    // max_latency of 0 means unbounded; anything else is checked per item.
    Scoreboard(std::string name, uint64_t max_latency = 0)
        : name_(std::move(name)), max_latency_(max_latency) {}

    void expect(const T& item, uint64_t cycle) { q_.push_back(Entry{item, cycle}); }

    bool observe(const T& item, uint64_t cycle) {
        if (q_.empty()) {
            fail("unexpected output %s with nothing outstanding", to_str(item).c_str());
            return false;
        }
        const Entry e = q_.front();
        q_.pop_front();

        if (!(e.item == item)) {
            fail("out-of-order or corrupt data: expected %s got %s",
                 to_str(e.item).c_str(), to_str(item).c_str());
            return false;
        }

        const uint64_t latency = cycle - e.cycle;
        latency_.add(latency);
        matched_++;

        if (max_latency_ && latency > max_latency_) {
            fail("%s took %llu cycles, limit is %llu", to_str(item).c_str(),
                 static_cast<unsigned long long>(latency),
                 static_cast<unsigned long long>(max_latency_));
            return false;
        }
        return true;
    }

    // Call at end of test: anything still queued never came out.
    bool drained() {
        if (q_.empty()) return true;
        fail("%zu item(s) never observed", q_.size());
        return false;
    }

    size_t outstanding() const { return q_.size(); }
    size_t matched() const { return matched_; }
    size_t errors() const { return errors_; }
    const LatencyStats& latency() const { return latency_; }

    void report(std::FILE* out = stdout) const {
        std::fprintf(out, "   %s: %zu matched, %zu outstanding, %zu errors",
                     name_.c_str(), matched_, q_.size(), errors_);
        if (latency_.samples) {
            std::fprintf(out, " | latency min/avg/max = %llu/%.2f/%llu",
                         static_cast<unsigned long long>(latency_.min),
                         latency_.mean(),
                         static_cast<unsigned long long>(latency_.max));
        }
        std::fprintf(out, "\n");
    }

private:
    struct Entry { T item; uint64_t cycle; };

    void fail(const char* fmt, ...) {
        errors_++;
        if (errors_ > 20) return;
        char buf[512];
        va_list ap;
        va_start(ap, fmt);
        std::vsnprintf(buf, sizeof buf, fmt, ap);
        va_end(ap);
        std::printf("         [%s] %s\n", name_.c_str(), buf);
    }

    std::string name_;
    uint64_t max_latency_;
    std::deque<Entry> q_;
    LatencyStats latency_;
    size_t matched_ = 0;
    size_t errors_ = 0;
};

}  // namespace vlib
