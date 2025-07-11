module psw(
	input wire g_en,g,clk,
	output reg gf=1'b0
);
always @(negedge clk)
	begin
	if(g_en==1'b1) gf<=g;
	end
endmodule
	