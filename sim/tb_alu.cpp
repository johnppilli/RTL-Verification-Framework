// ALU testbench.
//
// Purely combinational, so there is no clock and no reset: the framework's
// TestBench detects that and only exposes eval(). Stimulus leans on corner
// values rather than uniform randomness, because ALU bugs live at the
// boundaries (sign flips, shift-by-zero, INT_MIN, carry out of the top bit).

#include "Valu.h"
#include "vlib/vlib.h"

extern "C" {
void alu_init();
void alu_eval();
void alu_set_op(unsigned char op);
void alu_set_a(uint32_t a);
void alu_set_b(uint32_t b);
uint32_t alu_get_result();
bool alu_get_zero();
bool alu_get_carry();
bool alu_get_negative();
bool alu_get_overflow();
}

namespace {

enum Op : uint8_t {
    ADD = 0, SUB = 1, AND = 2, OR = 3, XOR = 4,
    SLL = 5, SRL = 6, SRA = 7, SLT = 8, SLTU = 9,
    OP_COUNT = 10
};

const char* op_name(uint8_t op) {
    static const char* names[] = {"ADD", "SUB", "AND", "OR", "XOR",
                                  "SLL", "SRL", "SRA", "SLT", "SLTU"};
    return op < OP_COUNT ? names[op] : "UNDEF";
}

// Values that historically break ALUs.
const uint32_t kCorners[] = {
    0x00000000u, 0x00000001u, 0x00000002u, 0x0000000Fu, 0x0000001Fu, 0x00000020u,
    0x0000FFFFu, 0x7FFFFFFEu, 0x7FFFFFFFu, 0x80000000u, 0x80000001u,
    0xFFFFFFFEu, 0xFFFFFFFFu, 0xAAAAAAAAu, 0x55555555u,
};
constexpr size_t kCornerCount = sizeof(kCorners) / sizeof(kCorners[0]);

class AluEnv {
public:
    explicit AluEnv(vlib::TestContext& ctx) : ctx_(ctx) {
        alu_init();
        tb_.open_trace(ctx.wave_path());
    }

    // Applies one operation and compares result and all four flags.
    void apply(uint8_t op, uint32_t a, uint32_t b) {
        tb_->op = op;
        tb_->a = a;
        tb_->b = b;
        tb_.eval();

        alu_set_op(op);
        alu_set_a(a);
        alu_set_b(b);
        alu_eval();

        Valu* d = tb_.dut();
        const char* n = op_name(op);
        if (!ctx_.check.eq(0, "result", d->result, alu_get_result()))
            ctx_.check.report(0, "  during %s a=0x%08x b=0x%08x", n, a, b);
        if (!ctx_.check.eq(0, "zero", d->zero, alu_get_zero()))
            ctx_.check.report(0, "  during %s a=0x%08x b=0x%08x", n, a, b);
        if (!ctx_.check.eq(0, "carry", d->carry, alu_get_carry()))
            ctx_.check.report(0, "  during %s a=0x%08x b=0x%08x", n, a, b);
        if (!ctx_.check.eq(0, "negative", d->negative, alu_get_negative()))
            ctx_.check.report(0, "  during %s a=0x%08x b=0x%08x", n, a, b);
        if (!ctx_.check.eq(0, "overflow", d->overflow, alu_get_overflow()))
            ctx_.check.report(0, "  during %s a=0x%08x b=0x%08x", n, a, b);

        sample_coverage(op, d);
        ops_++;
    }

    uint32_t result() const { return tb_.dut()->result; }
    bool zero() const { return tb_.dut()->zero; }
    bool carry() const { return tb_.dut()->carry; }
    bool negative() const { return tb_.dut()->negative; }
    bool overflow() const { return tb_.dut()->overflow; }
    size_t ops() const { return ops_; }

private:
    void sample_coverage(uint8_t op, Valu* d) {
        vlib::CoverGroup& c = ctx_.cov;
        if (op < OP_COUNT) c.cover(std::string("op ") + op_name(op));
        c.cover("result zero", d->zero);
        c.cover("result negative", d->negative);
        c.cover("carry set", d->carry);
        c.cover("signed overflow", d->overflow);
        c.cover("shift by zero", (op == SLL || op == SRL || op == SRA) && (d->b & 31) == 0);
        c.cover("shift by 31", (op == SLL || op == SRL || op == SRA) && (d->b & 31) == 31);
        c.cross("op ADD", "signed overflow", op == ADD, d->overflow != 0);
        c.cross("op SUB", "signed overflow", op == SUB, d->overflow != 0);
    }

    vlib::TestContext& ctx_;
    vlib::TestBench<Valu> tb_;
    size_t ops_ = 0;
};

void declare_bins(vlib::CoverGroup& cov) {
    for (uint8_t op = 0; op < OP_COUNT; op++) cov.bin(std::string("op ") + op_name(op));
    cov.bins({"result zero", "result negative", "carry set", "signed overflow",
              "shift by zero", "shift by 31",
              "op ADD x signed overflow", "op SUB x signed overflow"});
}

}  // namespace

VLIB_TEST(directed_arithmetic, "hand-checked add and subtract results and flags") {
    declare_bins(ctx.cov);
    AluEnv env(ctx);

    env.apply(ADD, 2, 3);
    ctx.check.eq(0, "2+3", env.result(), 5u);
    ctx.check.check(!env.carry() && !env.overflow(), 0, "2+3 set a flag it should not");

    env.apply(ADD, 0xFFFFFFFFu, 1);
    ctx.check.eq(0, "-1+1", env.result(), 0u);
    ctx.check.check(env.zero(), 0, "-1+1 should be zero");
    ctx.check.check(env.carry(), 0, "-1+1 should carry out");

    // Signed overflow: two positives summing to a negative.
    env.apply(ADD, 0x7FFFFFFFu, 1);
    ctx.check.eq(0, "INT_MAX+1", env.result(), 0x80000000u);
    ctx.check.check(env.overflow(), 0, "INT_MAX+1 should overflow");
    ctx.check.check(env.negative(), 0, "INT_MAX+1 should be negative");

    env.apply(SUB, 5, 3);
    ctx.check.eq(0, "5-3", env.result(), 2u);
    ctx.check.check(env.carry(), 0, "5-3 should not borrow");

    env.apply(SUB, 3, 5);
    ctx.check.eq(0, "3-5", env.result(), 0xFFFFFFFEu);
    ctx.check.check(!env.carry(), 0, "3-5 should borrow");
    ctx.check.check(env.negative(), 0, "3-5 should be negative");

    // Signed overflow: negative minus positive going positive.
    env.apply(SUB, 0x80000000u, 1);
    ctx.check.eq(0, "INT_MIN-1", env.result(), 0x7FFFFFFFu);
    ctx.check.check(env.overflow(), 0, "INT_MIN-1 should overflow");
}

VLIB_TEST(directed_logic_and_shift, "hand-checked logical and shift results") {
    declare_bins(ctx.cov);
    AluEnv env(ctx);

    env.apply(AND, 0xF0F0F0F0u, 0xFF00FF00u);
    ctx.check.eq(0, "and", env.result(), 0xF000F000u);
    env.apply(OR, 0xF0F0F0F0u, 0x0F0F0F0Fu);
    ctx.check.eq(0, "or", env.result(), 0xFFFFFFFFu);
    env.apply(XOR, 0xFFFFFFFFu, 0xAAAAAAAAu);
    ctx.check.eq(0, "xor", env.result(), 0x55555555u);

    env.apply(SLL, 1, 31);
    ctx.check.eq(0, "1 << 31", env.result(), 0x80000000u);
    env.apply(SLL, 0xFFFFFFFFu, 4);
    ctx.check.eq(0, "shifted-out bits are dropped", env.result(), 0xFFFFFFF0u);

    env.apply(SRL, 0x80000000u, 31);
    ctx.check.eq(0, "logical shift right fills zero", env.result(), 1u);
    env.apply(SRA, 0x80000000u, 31);
    ctx.check.eq(0, "arithmetic shift right fills sign", env.result(), 0xFFFFFFFFu);
    env.apply(SRA, 0x40000000u, 30);
    ctx.check.eq(0, "arithmetic shift of a positive", env.result(), 1u);

    // Only the low 5 bits of b are a shift amount.
    env.apply(SLL, 1, 32);
    ctx.check.eq(0, "shift amount wraps at 32", env.result(), 1u);
}

VLIB_TEST(directed_comparison, "signed and unsigned compares disagree where they should") {
    declare_bins(ctx.cov);
    AluEnv env(ctx);

    env.apply(SLT, 0xFFFFFFFFu, 1);
    ctx.check.eq(0, "signed -1 < 1", env.result(), 1u);
    env.apply(SLTU, 0xFFFFFFFFu, 1);
    ctx.check.eq(0, "unsigned 0xFFFFFFFF < 1", env.result(), 0u);

    env.apply(SLT, 0x80000000u, 0x7FFFFFFFu);
    ctx.check.eq(0, "signed INT_MIN < INT_MAX", env.result(), 1u);
    env.apply(SLTU, 0x80000000u, 0x7FFFFFFFu);
    ctx.check.eq(0, "unsigned INT_MIN > INT_MAX", env.result(), 0u);

    env.apply(SLT, 7, 7);
    ctx.check.eq(0, "x < x is false", env.result(), 0u);
    ctx.check.check(env.zero(), 0, "false compare should set zero");
}

VLIB_TEST(corner_cross_product, "every op against every pair of corner values") {
    declare_bins(ctx.cov);
    AluEnv env(ctx);

    for (uint8_t op = 0; op < OP_COUNT; op++)
        for (size_t i = 0; i < kCornerCount; i++)
            for (size_t j = 0; j < kCornerCount; j++)
                env.apply(op, kCorners[i], kCorners[j]);

    ctx.log("%zu operand combinations checked", env.ops());
}

VLIB_TEST(undefined_opcodes, "unimplemented opcodes read as zero with clean flags") {
    declare_bins(ctx.cov);
    AluEnv env(ctx);

    for (uint8_t op = OP_COUNT; op < 16; op++) {
        env.apply(op, 0xDEADBEEFu, 0x12345678u);
        ctx.check.eq(0, "undefined opcode result", env.result(), 0u);
        ctx.check.check(!env.carry() && !env.overflow(), 0,
                        "undefined opcode 0x%x set a flag", op);
    }
}

VLIB_TEST(random_operands, "randomized operands across every opcode") {
    declare_bins(ctx.cov);
    AluEnv env(ctx);

    for (uint64_t i = 0; i < ctx.cfg.cycles; i++) {
        const uint8_t op = static_cast<uint8_t>(ctx.rng.range(0, OP_COUNT - 1));
        // Mix uniform noise with corner values so both wide arithmetic and
        // boundary behaviour get exercised.
        const uint32_t a = ctx.rng.chance(30) ? kCorners[ctx.rng.range(0, kCornerCount - 1)]
                                              : ctx.rng.u32();
        const uint32_t b = ctx.rng.chance(30) ? kCorners[ctx.rng.range(0, kCornerCount - 1)]
                                              : ctx.rng.u32();
        env.apply(op, a, b);
    }
    ctx.log("%zu random operations checked", env.ops());
}

VLIB_SUITE_MAIN("alu")
