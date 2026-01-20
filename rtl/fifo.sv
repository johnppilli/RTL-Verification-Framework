// Synchronous FIFO with parameterized depth and width
/* verilator lint_off WIDTHEXPAND */
/* verilator lint_off WIDTHTRUNC */
module fifo #(
    parameter DATA_WIDTH = 8,
    parameter DEPTH = 8
)(
    input  logic                    clk,
    input  logic                    rst_n,

    // Write interface
    input  logic                    wr_en,
    input  logic [DATA_WIDTH-1:0]   data_in,

    // Read interface
    input  logic                    rd_en,
    output logic [DATA_WIDTH-1:0]   data_out,

    // Status signals
    output logic                    full,
    output logic                    empty,
    output logic [$clog2(DEPTH):0]  count
);

    // Local parameters
    localparam PTR_WIDTH = $clog2(DEPTH);
    localparam [PTR_WIDTH-1:0] PTR_MAX = DEPTH - 1;

    // Internal storage
    logic [DATA_WIDTH-1:0] memory [0:DEPTH-1];

    // Pointers
    logic [PTR_WIDTH-1:0] wr_ptr;
    logic [PTR_WIDTH-1:0] rd_ptr;

    // Internal count register
    logic [$clog2(DEPTH):0] count_reg;

    // Status flags
    assign full  = (count_reg == DEPTH);
    assign empty = (count_reg == 0);
    assign count = count_reg;

    // Data output - always shows head of queue
    assign data_out = memory[rd_ptr];

    // Write logic
    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            wr_ptr <= '0;
        end else if (wr_en && !full) begin
            memory[wr_ptr] <= data_in;
            wr_ptr <= (wr_ptr == PTR_MAX) ? '0 : wr_ptr + 1'b1;
        end
    end

    // Read logic
    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            rd_ptr <= '0;
        end else if (rd_en && !empty) begin
            rd_ptr <= (rd_ptr == PTR_MAX) ? '0 : rd_ptr + 1'b1;
        end
    end

    // Count logic
    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            count_reg <= '0;
        end else begin
            case ({wr_en && !full, rd_en && !empty})
                2'b10:   count_reg <= count_reg + 1;  // Write only
                2'b01:   count_reg <= count_reg - 1;  // Read only
                default: count_reg <= count_reg;      // Both or neither
            endcase
        end
    end

`ifdef FORMAL
    // =========================================================================
    // SVA Assertions - Protocol and correctness checking
    // =========================================================================

    // Count should never exceed depth
    assert property (@(posedge clk) disable iff (!rst_n)
        count_reg <= DEPTH
    ) else $error("ASSERT FAIL: Count exceeded FIFO depth");

    // Full and empty should be mutually exclusive
    assert property (@(posedge clk) disable iff (!rst_n)
        !(full && empty)
    ) else $error("ASSERT FAIL: FIFO both full and empty");

    // When full, count must equal DEPTH
    assert property (@(posedge clk) disable iff (!rst_n)
        full |-> (count_reg == DEPTH)
    ) else $error("ASSERT FAIL: Full flag inconsistent with count");

    // When empty, count must equal 0
    assert property (@(posedge clk) disable iff (!rst_n)
        empty |-> (count_reg == 0)
    ) else $error("ASSERT FAIL: Empty flag inconsistent with count");
`endif

endmodule
