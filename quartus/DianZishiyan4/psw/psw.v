module psw(
input wire g,clk,
output reg gf=1'b0,
input wire g_en
);
always @(negedge clk)
begin 
if(g_en==1'b1)
gf<=g;
end
endmodule
