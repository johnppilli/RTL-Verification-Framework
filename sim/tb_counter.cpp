#include <stdio.h>
#include <stdlib.h>
#include "Vcounter.h"
#include "verilated.h"

// Zig reference model functions (compiled from counter_model.zig)
extern "C" {
    void counter_init();
    void counter_tick();
    void counter_set_reset(bool rst_n);
    void counter_set_enable(bool enable);
    unsigned char counter_get_count();
}

int main(int argc, char** argv) {
    // Initialize Verilator
    Verilated::commandArgs(argc, argv);

    // Create instance of our RTL counter
    Vcounter* dut = new Vcounter;

    // Initialize Zig reference model
    counter_init(); //resets the Zig reference model to its starting state with count = 0, reset active, enable off

    // Initialize RTL signals
    dut->clk = 0;   //this sets clock to 0
    dut->rst_n = 0; //this sets reset to 0, turning it on/activating it becasue reset is active low
    dut->enable = 0; //this sets enable to 0, disabling counting (from the main loop)
    dut->eval(); //this tells Verilator to propagate these values throughout the circuit

    // Sync reference model
    counter_set_reset(false); //this sets the Zig model's signals to match the same as the RTL, with both now having reset active
    counter_set_enable(false); //and enable off

    printf("Starting counter verification with Zig reference model...\n");
    printf("Comparing RTL output against Zig golden model\n\n");

    // Reset sequence
    for (int i = 0; i < 4; i++) { //for loop with 4 iterations
        dut->clk = 1; //rising edge of the clock 
        dut->eval(); //the rtl will react to the rising edge
        counter_tick();  // Zig model also sees rising edge

        dut->clk = 0; //falling edge of the clock
        dut->eval(); //RTL will see the falling edge as well but nothing happens because the counter only triggers on rising 
    }

    // Release reset on both RTL and reference model
    dut->rst_n = 1; //here is where we change the inputs, reset = 1, in other words -- release reset (1)
    dut->enable = 1; //here is where we change the inputs, enable = 1, in other words -- start counting
    dut->eval();

    counter_set_reset(true); //done for this zig model too, changing the inputs so that they stay in sync
    counter_set_enable(true); //this is like, "I am gonna tell the software model what exactly my inputs are"

    // Run test
    int errors = 0; //this initializes the testing variables, errors is a counter that starts at 0
    int cycles = 20; //also intializies the testing variables, says cycles can only go for 20 iterations

    for (int cycle = 0; cycle < cycles; cycle++) { //cycle for loop, 20 iterations
        // Rising edge - RTL
        dut->clk = 1; //same idea/for loop as above
        dut->eval();

        // Rising edge - Zig reference model
        counter_tick(); //same idea/for loop as above

        // Falling edge - RTL
        dut->clk = 0; //same idea/for loop as above
        dut->eval();

        // Compare RTL vs reference model -- Reads the outputs
        unsigned char rtl_count = dut->count; //gets count value from RTL
        unsigned char ref_count = counter_get_count(); //gets count value from Zig

        if (rtl_count != ref_count) { //if both RTL and Zig are different values, print Error and increment the error count
            printf("ERROR at cycle %d: RTL=%d, Reference=%d (MISMATCH)\n",
                   cycle, rtl_count, ref_count);
            errors++;
        } else { //if they are the same value, print that they match
            printf("Cycle %2d: RTL=%2d, Reference=%2d (match)\n",
                   cycle, rtl_count, ref_count);
        }
    }

    // Report results
    printf("\n========== Test Complete ==========\n"); //prints whether test passed or failed 
    printf("Cycles tested: %d\n", cycles);
    if (errors == 0) { //if there aren't any errors print passed
        printf("Result: PASSED - RTL matches reference model\n");
    } else { //if there are any errors print failed
        printf("Result: FAILED - %d mismatches found\n", errors);
    }


    //cleanup area/exit
    delete dut; //free memory
    return errors > 0 ? 1 : 0; //return 1 if there were errors, 0 if no errors 
}
