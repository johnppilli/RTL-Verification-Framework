// Simple 8-bit counter - our first RTL module to verify
module counter (
    input  logic       clk, //intializing clock as input
    input  logic       rst_n,    // Active-low reset
    input  logic       enable, //intializing enable as input
    output logic [7:0] count //intializing count as 8 bit counter
);

    always_ff @(posedge clk or negedge rst_n) begin //always(flipflop), @() will execute whatever is in the (), on a positive edge or falling edge, begin 
        if (!rst_n) // if reset = 0 then count = 0, if reset = 1, then go to else if
            count <= 8'b0; //then set the count to 0, 8 bit 0
        else if (enable) // otherwise, if enable = 1 counter increments, but if enable = 0, counter doesn't do anything 
            count <= count + 1; //increment the count by 1
    end

endmodule
