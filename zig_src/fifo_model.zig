// Reference model for rtl/fifo.sv. DEPTH and DATA_WIDTH must match the RTL
// parameters; the testbench asserts this against the DUT at startup.

const DEPTH: usize = 8;

const State = struct {
    memory: [DEPTH]u8 = [_]u8{0} ** DEPTH,
    wr_ptr: usize = 0,
    rd_ptr: usize = 0,
    count: usize = 0,

    rst_n: bool = false,
    wr_en: bool = false,
    rd_en: bool = false,
    data_in: u8 = 0,
};

var st: State = .{};

export fn fifo_init() void {
    st = .{};
}

export fn fifo_depth() usize {
    return DEPTH;
}

export fn fifo_set_reset(rst_n: bool) void {
    st.rst_n = rst_n;
}

export fn fifo_set_wr_en(wr_en: bool) void {
    st.wr_en = wr_en;
}

export fn fifo_set_rd_en(rd_en: bool) void {
    st.rd_en = rd_en;
}

export fn fifo_set_data_in(data: u8) void {
    st.data_in = data;
}

export fn fifo_tick() void {
    if (!st.rst_n) {
        st.wr_ptr = 0;
        st.rd_ptr = 0;
        st.count = 0;
        return;
    }

    const do_write = st.wr_en and st.count < DEPTH;
    const do_read = st.rd_en and st.count > 0;

    if (do_write) {
        st.memory[st.wr_ptr] = st.data_in;
        st.wr_ptr = (st.wr_ptr + 1) % DEPTH;
    }
    if (do_read) {
        st.rd_ptr = (st.rd_ptr + 1) % DEPTH;
    }

    if (do_write and !do_read) {
        st.count += 1;
    } else if (do_read and !do_write) {
        st.count -= 1;
    }
}

export fn fifo_get_data_out() u8 {
    return st.memory[st.rd_ptr];
}

export fn fifo_get_full() bool {
    return st.count == DEPTH;
}

export fn fifo_get_empty() bool {
    return st.count == 0;
}

export fn fifo_get_count() usize {
    return st.count;
}
