// Reference model for counter - the "golden" implementation
// This is what the RTL should behave like

const CounterState = struct { //define a struct (container for data)
    count: u8 = 0,  //8 bit unsigned int that starts at 0
    rst_n: bool = false, //boolean value that starts with false
    enable: bool = false, //boolean value that starts with false 
};

var state: CounterState = .{}; //this line creates a global instance of the struct above

                            // .{}; this initializes it with the default values, the ones above
//same as the RTL counter: counts value, resets signal, enables signal

// Called on every rising clock edge - mimics the RTL behavior
export fn counter_tick() void {
    if (!state.rst_n) {  //if reset is active then count = 0
        state.count = 0;
    } else if (state.enable) { //else if, enable -> count then increment the count by 1
        state.count +%= 1; // wrapping add, meaning that if it goes past 255, it will just go back down to 0 and restart from there
    }
}

// Set the reset signal
export fn counter_set_reset(rst_n: bool) void { //all this does is take the value of counter_set_reset from the tb and stores it in the new variable   
    state.rst_n = rst_n; //this is how the tb tells the Zig model what the rst input is
} //since counter_set_reset = true in the tb, state.rst_n = true

// Set the enable signal ------ takes value of counter_set_enable from the tb and stores it in the new variable
export fn counter_set_enable(enable: bool) void {
    state.enable = enable; //since counter_set_enable = true in tb, state.enable = true
}

// Get the current count value
export fn counter_get_count() u8 { //this returns the count value so that the tb can compare it to the RTL output
    return state.count;
}

// Reset the model to initial state (for starting new tests)
export fn counter_init() void {
    state = .{}; //resets everything back to the default/intial value;s
}
