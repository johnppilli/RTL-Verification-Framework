#pragma once
// Deterministic stimulus generation.
//
// Every random decision in a testbench comes from here so that a failing run
// can be replayed exactly: the seed is printed on every run and accepted back
// via --seed. xoshiro256** is used rather than rand() because libc's generator
// differs between platforms, which would make a seed non-portable.

#include <cstdint>
#include <cstddef>
#include <vector>

namespace vlib {

class Random {
public:
    explicit Random(uint64_t seed = 1) { reseed(seed); }

    void reseed(uint64_t seed) {
        sm_ = seed + 0x243F6A8885A308D3ULL;
        for (int i = 0; i < 4; i++) s_[i] = splitmix();
        for (int i = 0; i < 16; i++) next();  // discard startup correlation
    }

    uint64_t next() {
        const uint64_t result = rotl(s_[1] * 5, 7) * 9;
        const uint64_t t = s_[1] << 17;
        s_[2] ^= s_[0];
        s_[3] ^= s_[1];
        s_[1] ^= s_[2];
        s_[0] ^= s_[3];
        s_[2] ^= t;
        s_[3] = rotl(s_[3], 45);
        return result;
    }

    uint32_t u32() { return static_cast<uint32_t>(next() >> 32); }
    uint16_t u16() { return static_cast<uint16_t>(next() >> 48); }
    uint8_t  u8()  { return static_cast<uint8_t>(next() >> 56); }

    // Inclusive on both ends.
    uint64_t range(uint64_t lo, uint64_t hi) {
        if (hi <= lo) return lo;
        const uint64_t span = hi - lo + 1;
        return span == 0 ? next() : lo + next() % span;
    }

    bool flip() { return (next() >> 63) != 0; }

    // True `percent` of the time. The main knob for shaping stimulus density
    // (e.g. how often a downstream consumer asserts ready).
    bool chance(int percent) {
        if (percent <= 0) return false;
        if (percent >= 100) return true;
        return static_cast<int>(next() % 100) < percent;
    }

    // Index into `weights` chosen proportionally. Returns 0 if all weights are
    // zero so callers never have to guard the degenerate case.
    size_t weighted(const std::vector<int>& weights) {
        int total = 0;
        for (int w : weights) total += w > 0 ? w : 0;
        if (total <= 0) return 0;
        int roll = static_cast<int>(next() % static_cast<uint64_t>(total));
        for (size_t i = 0; i < weights.size(); i++) {
            if (weights[i] <= 0) continue;
            roll -= weights[i];
            if (roll < 0) return i;
        }
        return weights.size() - 1;
    }

    template <typename T>
    const T& pick(const std::vector<T>& v) { return v[range(0, v.size() - 1)]; }

private:
    static uint64_t rotl(uint64_t x, int k) { return (x << k) | (x >> (64 - k)); }

    uint64_t splitmix() {
        uint64_t z = (sm_ += 0x9E3779B97F4A7C15ULL);
        z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
        z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
        return z ^ (z >> 31);
    }

    uint64_t s_[4] = {0, 0, 0, 0};
    uint64_t sm_ = 0;
};

}  // namespace vlib
