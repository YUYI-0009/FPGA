module ir(
input wire ld_ir,
input wire clk,
output reg [7:0] x=8'b00000000,
input wire [7:0] a
);
always @(negedge clk)
	begin
if(ld_ir==1'b1) 
x<=a;	end
endmodule
	