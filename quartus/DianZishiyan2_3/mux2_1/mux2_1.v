module mux2_1(
	output reg [7:0] y,
	input wire [7:0] a,b,
	input wire s
);
always @(s) begin
if(s==1'b0) y=a;
else y=b;
end
endmodule