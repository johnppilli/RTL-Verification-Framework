// Valid/ready skid buffer.
//
// Registers both the data path and s_ready so neither crosses the module
// boundary combinationally, while still accepting one beat per cycle when the
// consumer keeps up. The extra "skid" slot holds the beat that was already
// accepted when the consumer stalled, which is the whole point of the design:
// backpressure takes a cycle to travel upstream, so one beat is always in
// flight and needs somewhere to land.
module skid_buffer #(
    parameter int DATA_WIDTH = 8
)(
    input  logic                  clk,
    input  logic                  rst_n,

    // Upstream
    input  logic                  s_valid,
    output logic                  s_ready,
    input  logic [DATA_WIDTH-1:0] s_data,

    // Downstream
    output logic                  m_valid,
    input  logic                  m_ready,
    output logic [DATA_WIDTH-1:0] m_data
);

    logic                  skid_valid;
    logic [DATA_WIDTH-1:0] skid_data;
    logic                  out_valid;
    logic [DATA_WIDTH-1:0] out_data;

    assign s_ready = !skid_valid;
    assign m_valid = out_valid;
    assign m_data  = out_data;

    wire accept = s_valid && s_ready;

    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            skid_valid <= 1'b0;
        end else if (accept && out_valid && !m_ready) begin
            skid_valid <= 1'b1;     // output slot busy, park the beat
        end else if (m_ready) begin
            skid_valid <= 1'b0;     // slot drains this cycle
        end
    end

    always_ff @(posedge clk) begin
        if (accept)
            skid_data <= s_data;
    end

    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            out_valid <= 1'b0;
        end else if (!out_valid || m_ready) begin
            out_valid <= skid_valid || accept;
        end
    end

    always_ff @(posedge clk) begin
        if (!out_valid || m_ready)
            out_data <= skid_valid ? skid_data : s_data;
    end

`ifdef SIM_ASSERT
    // A held-but-unaccepted output beat must not change.
    assert property (@(posedge clk) disable iff (!rst_n)
        (m_valid && !m_ready) |=> (m_valid && $stable(m_data))
    ) else $error("skid_buffer dropped or mutated a stalled beat");

    // The skid slot only ever fills behind an occupied output slot.
    assert property (@(posedge clk) disable iff (!rst_n)
        skid_valid |-> out_valid
    ) else $error("skid_buffer skid occupied with empty output");
`endif

endmodule
