// Reference model for rtl/alu.sv (WIDTH = 32).
//
// Written against the ISA definition of each operation rather than by
// transcribing the RTL, which is the only way the comparison means anything.

const Op = enum(u4) {
    add = 0,
    sub = 1,
    op_and = 2,
    op_or = 3,
    op_xor = 4,
    sll = 5,
    srl = 6,
    sra = 7,
    slt = 8,
    sltu = 9,
    _,
};

const State = struct {
    op: u4 = 0,
    a: u32 = 0,
    b: u32 = 0,

    result: u32 = 0,
    zero: bool = true,
    carry: bool = false,
    negative: bool = false,
    overflow: bool = false,
};

var st: State = .{};

export fn alu_init() void {
    st = .{};
}

export fn alu_set_op(op: u8) void {
    st.op = @truncate(op);
}

export fn alu_set_a(a: u32) void {
    st.a = a;
}

export fn alu_set_b(b: u32) void {
    st.b = b;
}

export fn alu_eval() void {
    const a = st.a;
    const b = st.b;
    const sa: i32 = @bitCast(a);
    const sb: i32 = @bitCast(b);
    const shamt: u5 = @truncate(b);

    var result: u32 = 0;
    var carry = false;
    var overflow = false;

    switch (@as(Op, @enumFromInt(st.op))) {
        .add => {
            const wide = @as(u64, a) + @as(u64, b);
            result = @truncate(wide);
            carry = (wide >> 32) != 0;
            overflow = (a >> 31 == b >> 31) and (result >> 31 != a >> 31);
        },
        .sub => {
            result = a -% b;
            carry = a >= b; // carry is "no borrow"
            overflow = (a >> 31 != b >> 31) and (result >> 31 != a >> 31);
        },
        .op_and => result = a & b,
        .op_or => result = a | b,
        .op_xor => result = a ^ b,
        .sll => result = a << shamt,
        .srl => result = a >> shamt,
        .sra => result = @bitCast(sa >> shamt),
        .slt => result = if (sa < sb) 1 else 0,
        .sltu => result = if (a < b) 1 else 0,
        else => result = 0,
    }

    st.result = result;
    st.carry = carry;
    st.overflow = overflow;
    st.zero = result == 0;
    st.negative = (result >> 31) != 0;
}

export fn alu_get_result() u32 {
    return st.result;
}

export fn alu_get_zero() bool {
    return st.zero;
}

export fn alu_get_carry() bool {
    return st.carry;
}

export fn alu_get_negative() bool {
    return st.negative;
}

export fn alu_get_overflow() bool {
    return st.overflow;
}
