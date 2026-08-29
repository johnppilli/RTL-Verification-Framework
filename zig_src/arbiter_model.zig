// Reference model for rtl/arbiter.sv. N must match the RTL parameter.
//
// grant is combinational, so alu-style eval/get separation applies: call
// arbiter_eval() after changing req, and arbiter_tick() to advance the
// priority pointer at a clock edge.

const N: usize = 4;

const State = struct {
    ptr: usize = 0,

    rst_n: bool = false,
    req: u32 = 0,

    grant: u32 = 0,
    valid: bool = false,
};

var st: State = .{};

// Index of the winning requester under the current pointer, if any.
fn arbitrate() ?usize {
    var i: usize = 0;
    while (i < N) : (i += 1) {
        const idx = (i + st.ptr) % N;
        if (((st.req >> @intCast(idx)) & 1) == 1) return idx;
    }
    return null;
}

export fn arbiter_init() void {
    st = .{};
}

export fn arbiter_width() usize {
    return N;
}

export fn arbiter_set_reset(rst_n: bool) void {
    st.rst_n = rst_n;
}

export fn arbiter_set_req(req: u32) void {
    st.req = req;
}

export fn arbiter_eval() void {
    if (arbitrate()) |w| {
        st.grant = @as(u32, 1) << @intCast(w);
        st.valid = true;
    } else {
        st.grant = 0;
        st.valid = false;
    }
}

export fn arbiter_tick() void {
    if (!st.rst_n) {
        st.ptr = 0;
        arbiter_eval();
        return;
    }
    if (arbitrate()) |w| {
        st.ptr = (w + 1) % N;
    }
    arbiter_eval();
}

export fn arbiter_get_grant() u32 {
    return st.grant;
}

export fn arbiter_get_valid() bool {
    return st.valid;
}

export fn arbiter_get_ptr() usize {
    return st.ptr;
}
