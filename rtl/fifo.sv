// Synchronous FIFO.
//
// data_out is combinational off the read pointer, so the head of the queue is
// visible in the same cycle rd_en is asserted for it. Writes to a full FIFO and
// reads from an empty one are ignored rather than corrupting state.
/* verilator lint_off WIDTHEXPAND */
/* verilator lint_off WIDTHTRUNC */
module fifo #(
    parameter int DATA_WIDTH = 8,
    parameter int DEPTH      = 8
)(
    input  logic                    clk,
    input  logic                    rst_n,

    input  logic                    wr_en,
    input  logic [DATA_WIDTH-1:0]   data_in,

    input  logic                    rd_en,
    output logic [DATA_WIDTH-1:0]   data_out,

    output logic                    full,
    output logic                    empty,
    output logic [$clog2(DEPTH):0]  count
);

    localparam int PTR_WIDTH = $clog2(DEPTH);
    localparam logic [PTR_WIDTH-1:0] PTR_MAX = DEPTH - 1;

    logic [DATA_WIDTH-1:0] memory [0:DEPTH-1];
    logic [PTR_WIDTH-1:0]  wr_ptr;
    logic [PTR_WIDTH-1:0]  rd_ptr;
    logic [$clog2(DEPTH):0] count_reg;

    assign full     = (count_reg == DEPTH);
    assign empty    = (count_reg == 0);
    assign count    = count_reg;
    assign data_out = memory[rd_ptr];

    wire do_write = wr_en && !full;
    wire do_read  = rd_en && !empty;

    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            wr_ptr <= '0;
        end else if (do_write) begin
            memory[wr_ptr] <= data_in;
            wr_ptr <= (wr_ptr == PTR_MAX) ? '0 : wr_ptr + 1'b1;
        end
    end

    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n)
            rd_ptr <= '0;
        else if (do_read)
            rd_ptr <= (rd_ptr == PTR_MAX) ? '0 : rd_ptr + 1'b1;
    end

    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            count_reg <= '0;
        end else begin
            case ({do_write, do_read})
                2'b10:   count_reg <= count_reg + 1'b1;
                2'b01:   count_reg <= count_reg - 1'b1;
                default: count_reg <= count_reg;
            endcase
        end
    end

`ifdef SIM_ASSERT
    assert property (@(posedge clk) disable iff (!rst_n) count_reg <= DEPTH)
        else $error("fifo count exceeded depth");

    assert property (@(posedge clk) disable iff (!rst_n) !(full && empty))
        else $error("fifo reported full and empty at once");

    assert property (@(posedge clk) disable iff (!rst_n) full |-> (count_reg == DEPTH))
        else $error("fifo full flag inconsistent with count");

    assert property (@(posedge clk) disable iff (!rst_n) empty |-> (count_reg == 0))
        else $error("fifo empty flag inconsistent with count");

    // A write that is not paired with a read must move the count up by one.
    assert property (@(posedge clk) disable iff (!rst_n)
        (do_write && !do_read) |=> (count_reg == $past(count_reg) + 1))
        else $error("fifo count did not track a write");
`endif

endmodule
/* verilator lint_on WIDTHTRUNC */
/* verilator lint_on WIDTHEXPAND */
