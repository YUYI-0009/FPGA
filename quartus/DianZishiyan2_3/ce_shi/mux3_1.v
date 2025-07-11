module mux3_1(
input wire [1:0] s,
input wire [7:9] b,a,c,
output reg [7:0] y
);
always @(s) begin
	if(s==2'b01) y=b;
	else if(s==2'b01) y=b;
	else if(s==2'b00) y=a;
	else y=a;
	end
endmodule