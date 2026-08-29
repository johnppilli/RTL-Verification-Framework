// Combinational ALU with flag outputs.
//
// carry means carry-out for ADD and "no borrow" for SUB; it is zero for the
// logical and shift ops. overflow is signed two's complement overflow and is
// only meaningful for ADD and SUB.
/* verilator lint_off WIDTHEXPAND */
module alu #(
    parameter int WIDTH = 32
)(
    input  logic [3:0]       op,
    input  logic [WIDTH-1:0] a,
    input  logic [WIDTH-1:0] b,
    output logic [WIDTH-1:0] result,
    output logic             zero,
    output logic             carry,
    output logic             negative,
    output logic             overflow
);

    localparam logic [3:0] OP_ADD  = 4'd0;
    localparam logic [3:0] OP_SUB  = 4'd1;
    localparam logic [3:0] OP_AND  = 4'd2;
    localparam logic [3:0] OP_OR   = 4'd3;
    localparam logic [3:0] OP_XOR  = 4'd4;
    localparam logic [3:0] OP_SLL  = 4'd5;
    localparam logic [3:0] OP_SRL  = 4'd6;
    localparam logic [3:0] OP_SRA  = 4'd7;
    localparam logic [3:0] OP_SLT  = 4'd8;
    localparam logic [3:0] OP_SLTU = 4'd9;

    localparam int SHAMT_W = $clog2(WIDTH);

    logic [WIDTH:0]   sum;
    logic [WIDTH:0]   diff;
    logic [SHAMT_W-1:0] shamt;

    assign shamt = b[SHAMT_W-1:0];
    assign sum   = {1'b0, a} + {1'b0, b};
    assign diff  = {1'b0, a} - {1'b0, b};

    always_comb begin
        result = '0;
        carry  = 1'b0;
        unique case (op)
            OP_ADD:  begin result = sum[WIDTH-1:0];  carry = sum[WIDTH];   end
            OP_SUB:  begin result = diff[WIDTH-1:0]; carry = ~diff[WIDTH]; end
            OP_AND:  result = a & b;
            OP_OR:   result = a | b;
            OP_XOR:  result = a ^ b;
            OP_SLL:  result = a << shamt;
            OP_SRL:  result = a >> shamt;
            OP_SRA:  result = $signed(a) >>> shamt;
            OP_SLT:  result = ($signed(a) < $signed(b)) ? {{WIDTH-1{1'b0}}, 1'b1} : '0;
            OP_SLTU: result = (a < b) ? {{WIDTH-1{1'b0}}, 1'b1} : '0;
            default: begin result = '0; carry = 1'b0; end
        endcase
    end

    always_comb begin
        unique case (op)
            OP_ADD:  overflow = (a[WIDTH-1] == b[WIDTH-1]) && (result[WIDTH-1] != a[WIDTH-1]);
            OP_SUB:  overflow = (a[WIDTH-1] != b[WIDTH-1]) && (result[WIDTH-1] != a[WIDTH-1]);
            default: overflow = 1'b0;
        endcase
    end

    assign zero     = (result == '0);
    assign negative = result[WIDTH-1];

endmodule
/* verilator lint_on WIDTHEXPAND */
