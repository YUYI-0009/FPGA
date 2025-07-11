module reg_group(
input wire [1:0] sr,dr,
input wire clk,we,
input wire [7:0] i,
output reg [7:0] s,d
);
reg [7:0] R0=8'b00000001,R1=8'b00000000,R2=8'b00000000,R3=8'b00000111;
always @(sr,dr)
begin
if(sr==2'b00) s=R0;
else if(sr==2'b01) s=R1;
else if(sr==2'b10) s=R2;
else s=R3;

if(dr==2'b00) d=R0;
else if(dr==2'b01) d=R1;
else if(dr==2'b10) d=R2;
else d=R3;
end
always @(negedge clk)
begin 
if(we)
begin
if(dr==2'b00) R0<=i;
else if(dr==2'b10) R2<=i;
else if(dr==2'b01) R1<=i;
else R3<=i;
end
end
endmodule
