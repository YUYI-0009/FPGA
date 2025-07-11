module pc(
input wire in_pc,clk,ld_pc,
input wire [7:0] a,
output reg [7:0] c=8'b00000000
);
always @(negedge clk)
begin 
if(ld_pc==1'b1&&in_pc==1'b0 ) 
c<=a;
else if(ld_pc==1'b0&&in_pc==1'b1 )
c<=c+8'b00000001;
end
endmodule
