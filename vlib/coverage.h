#pragma once
// Functional coverage.
//
// Bins are declared before they are sampled so that a scenario the stimulus
// never reached reports as MISS rather than silently not existing -- an
// undeclared bin is the one failure mode that makes a coverage number a lie.

#include <cstdint>
#include <cstdio>
#include <initializer_list>
#include <map>
#include <string>
#include <vector>

namespace vlib {

class CoverGroup {
public:
    explicit CoverGroup(std::string name = "functional") : name_(std::move(name)) {}

    CoverGroup& bin(const std::string& name) { slot(name); return *this; }

    CoverGroup& bins(std::initializer_list<const char*> names) {
        for (const char* n : names) slot(n);
        return *this;
    }

    // One bin per value in [lo, hi], named "<prefix>=<v>". Used for count
    // distributions where every occupancy level should be exercised.
    CoverGroup& value_bins(const std::string& prefix, long lo, long hi) {
        for (long v = lo; v <= hi; v++) slot(value_name(prefix, v));
        return *this;
    }

    void cover(const std::string& name, bool cond = true) {
        if (cond) slot(name).hits++;
    }

    void cover_value(const std::string& prefix, long v) {
        slot(value_name(prefix, v)).hits++;
    }

    // Cross of two conditions; the bin is only credited when both hold.
    void cross(const std::string& a, const std::string& b, bool ca, bool cb) {
        cover(a + " x " + b, ca && cb);
    }

    size_t bins_total() const { return bins_.size(); }

    size_t bins_hit() const {
        size_t n = 0;
        for (const Bin& b : bins_) if (b.hits) n++;
        return n;
    }

    double closure() const {
        return bins_.empty() ? 100.0 : 100.0 * bins_hit() / bins_.size();
    }

    void report(std::FILE* out = stdout) const {
        std::fprintf(out, "\n-- %s: %zu/%zu bins, %.1f%% --\n",
                     name_.c_str(), bins_hit(), bins_total(), closure());
        size_t width = 0;
        for (const Bin& b : bins_) width = b.name.size() > width ? b.name.size() : width;
        for (const Bin& b : bins_) {
            std::fprintf(out, "   %-*s  %s", static_cast<int>(width), b.name.c_str(),
                         b.hits ? "HIT " : "MISS");
            if (b.hits) std::fprintf(out, " (%llu)", static_cast<unsigned long long>(b.hits));
            std::fprintf(out, "\n");
        }
    }

private:
    struct Bin { std::string name; uint64_t hits = 0; };

    static std::string value_name(const std::string& prefix, long v) {
        return prefix + "=" + std::to_string(v);
    }

    Bin& slot(const std::string& name) {
        auto it = index_.find(name);
        if (it != index_.end()) return bins_[it->second];
        index_.emplace(name, bins_.size());
        bins_.push_back(Bin{name, 0});
        return bins_.back();
    }

    std::string name_;
    std::vector<Bin> bins_;
    std::map<std::string, size_t> index_;
};

}  // namespace vlib
