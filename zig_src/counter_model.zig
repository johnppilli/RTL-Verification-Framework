// Reference model for rtl/counter.sv.

const State = struct {
    count: u8 = 0,
    rst_n: bool = false,
    enable: bool = false,
};

var st: State = .{};

export fn counter_init() void {
    st = .{};
}

export fn counter_set_reset(rst_n: bool) void {
    st.rst_n = rst_n;
}

export fn counter_set_enable(enable: bool) void {
    st.enable = enable;
}

export fn counter_tick() void {
    if (!st.rst_n) {
        st.count = 0;
    } else if (st.enable) {
        st.count +%= 1;
    }
}

export fn counter_get_count() u8 {
    return st.count;
}
