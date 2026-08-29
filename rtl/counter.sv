// Free-running 8-bit counter with active-low reset and an enable.
module counter (
    input  logic       clk,
    input  logic       rst_n,
    input  logic       enable,
    output logic [7:0] count
);

    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n)
            count <= 8'b0;
        else if (enable)
            count <= count + 1'b1;   // wraps 255 -> 0
    end

`ifdef SIM_ASSERT
    // Count may only change on a cycle where it was enabled.
    assert property (@(posedge clk) disable iff (!rst_n)
        !enable |=> $stable(count)
    ) else $error("counter changed while disabled");
`endif

endmodule
