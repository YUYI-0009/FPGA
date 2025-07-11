module fsm(
input clk,
input reset,
input start,
input skip3,
input wait3,
output reg[2:0] zot
);

parameter state0 = 2'h0,
					state1 = 2'h1,
					state2 = 2'h2,
					
					state3 = 2'h3;
					
reg [1:0] state, next_st;

always @ (*)
case(state)
	state0:	if(start)	next_st = state1;
					else 			next_st = state0;
			
			
	state1:						next_st = state2;

	state2: if(skip3)	next_st = state0;
					else 			next_st = state3;
					
	state3:	if(wait3)	next_st = state3;
					else 			next_st = state0;
	default:next_st = state0;

endcase

always @ (posedge clk or posedge reset)
if(reset)	state <= state0;
else			state <= next_st;


always @ (*)
case(state)
	state0:	zot = 3'b000;

	state1:	zot = 3'b101;

	state2:	zot = 3'b111;

	state3:	zot = 3'b001;

	default:zot = 3'b000;
endcase

endmodule
