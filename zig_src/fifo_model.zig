// Reference model for FIFO - the "golden" implementation
// This is what the RTL should behave like

const DEPTH: usize = 8;
const DATA_WIDTH: usize = 8;

const FifoState = struct {
    memory: [DEPTH]u8 = [_]u8{0} ** DEPTH,
    wr_ptr: usize = 0,
    rd_ptr: usize = 0,
    count: usize = 0,

    // Input signals (set by testbench before tick)
    wr_en: bool = false,
    rd_en: bool = false,
    data_in: u8 = 0,
    rst_n: bool = false,
};

var state: FifoState = .{};

// Called on every rising clock edge - mimics the RTL behavior
export fn fifo_tick() void {
    if (!state.rst_n) {
        // Reset: clear pointers and count
        state.wr_ptr = 0;
        state.rd_ptr = 0;
        state.count = 0;
    } else {
        const can_write = state.wr_en and (state.count < DEPTH);
        const can_read = state.rd_en and (state.count > 0);

        // Write operation
        if (can_write) {
            state.memory[state.wr_ptr] = state.data_in;
            state.wr_ptr = (state.wr_ptr + 1) % DEPTH;
        }

        // Read operation (advances pointer)
        if (can_read) {
            state.rd_ptr = (state.rd_ptr + 1) % DEPTH;
        }

        // Update count
        if (can_write and !can_read) {
            state.count += 1;
        } else if (can_read and !can_write) {
            state.count -= 1;
        }
    }
}

// Set the reset signal
export fn fifo_set_reset(rst_n: bool) void {
    state.rst_n = rst_n;
}

// Set the write enable signal
export fn fifo_set_wr_en(wr_en: bool) void {
    state.wr_en = wr_en;
}

// Set the read enable signal
export fn fifo_set_rd_en(rd_en: bool) void {
    state.rd_en = rd_en;
}

// Set the data input
export fn fifo_set_data_in(data: u8) void {
    state.data_in = data;
}

// Get the data output (always shows head of queue)
export fn fifo_get_data_out() u8 {
    return state.memory[state.rd_ptr];
}

// Get the full flag
export fn fifo_get_full() bool {
    return state.count == DEPTH;
}

// Get the empty flag
export fn fifo_get_empty() bool {
    return state.count == 0;
}

// Get the current count
export fn fifo_get_count() usize {
    return state.count;
}

// Reset the model to initial state (for starting new tests)
export fn fifo_init() void {
    state = .{};
}
