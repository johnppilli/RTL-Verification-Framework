// Round-robin arbiter.
//
// Grant is combinational so a requester sees its grant in the same cycle it
// asks. The rotating priority pointer is registered and moves just past the
// winner, which is what bounds the worst-case wait to N cycles.
/* verilator lint_off WIDTHEXPAND */
module arbiter #(
    parameter int N = 4
)(
    input  logic         clk,
    input  logic         rst_n,
    input  logic [N-1:0] req,
    output logic [N-1:0] grant,
    output logic         valid
);

    localparam int PTR_W = (N > 1) ? $clog2(N) : 1;

    logic [PTR_W-1:0] ptr;      // index that currently holds highest priority
    logic [PTR_W-1:0] winner;

    always_comb begin
        grant  = '0;
        winner = ptr;
        // Walk outward from ptr so the requester after the last winner is
        // considered first.
        for (int i = 0; i < N; i++) begin
            automatic logic [PTR_W-1:0] idx = PTR_W'((i + int'(ptr)) % N);
            if (grant == '0 && req[idx]) begin
                grant[idx] = 1'b1;
                winner     = idx;
            end
        end
    end

    assign valid = |grant;

    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n)
            ptr <= '0;
        else if (valid)
            ptr <= (int'(winner) == N - 1) ? '0 : winner + 1'b1;
    end

`ifdef SIM_ASSERT
    // Never grant more than one requester.
    assert property (@(posedge clk) disable iff (!rst_n)
        $onehot0(grant)
    ) else $error("arbiter granted more than one requester");

    // Never grant someone who did not ask.
    assert property (@(posedge clk) disable iff (!rst_n)
        (grant & ~req) == '0
    ) else $error("arbiter granted an idle requester");

    // Grant exactly when someone is asking.
    assert property (@(posedge clk) disable iff (!rst_n)
        (|req) == valid
    ) else $error("arbiter valid does not track req");
`endif

endmodule
/* verilator lint_on WIDTHEXPAND */
