// Reference model for rtl/skid_buffer.sv.
//
// Every next-state value is computed from the current state before anything is
// written back, mirroring non-blocking assignment in the RTL. Getting this
// wrong is the classic way a "golden" model silently agrees with a broken DUT.

const State = struct {
    skid_valid: bool = false,
    skid_data: u8 = 0,
    out_valid: bool = false,
    out_data: u8 = 0,

    rst_n: bool = false,
    s_valid: bool = false,
    s_data: u8 = 0,
    m_ready: bool = false,
};

var st: State = .{};

export fn skid_init() void {
    st = .{};
}

export fn skid_set_reset(rst_n: bool) void {
    st.rst_n = rst_n;
}

export fn skid_set_s_valid(v: bool) void {
    st.s_valid = v;
}

export fn skid_set_s_data(d: u8) void {
    st.s_data = d;
}

export fn skid_set_m_ready(r: bool) void {
    st.m_ready = r;
}

export fn skid_tick() void {
    if (!st.rst_n) {
        st.skid_valid = false;
        st.out_valid = false;
        return;
    }

    const s_ready = !st.skid_valid;
    const accept = st.s_valid and s_ready;

    const next_skid_valid = if (accept and st.out_valid and !st.m_ready)
        true
    else if (st.m_ready)
        false
    else
        st.skid_valid;

    const next_skid_data = if (accept) st.s_data else st.skid_data;

    var next_out_valid = st.out_valid;
    var next_out_data = st.out_data;
    if (!st.out_valid or st.m_ready) {
        next_out_valid = st.skid_valid or accept;
        next_out_data = if (st.skid_valid) st.skid_data else st.s_data;
    }

    st.skid_valid = next_skid_valid;
    st.skid_data = next_skid_data;
    st.out_valid = next_out_valid;
    st.out_data = next_out_data;
}

export fn skid_get_s_ready() bool {
    return !st.skid_valid;
}

export fn skid_get_m_valid() bool {
    return st.out_valid;
}

export fn skid_get_m_data() u8 {
    return st.out_data;
}
