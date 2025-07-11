module ir(
	input wire clk,ld_ir,
	input wire [7:0] a,
	output reg [7:0] x=8'b00000000
);
always @(negedge clk)
	begin
	if(ld_ir==1'b1) x<=a;
	end
endmodule
	
	