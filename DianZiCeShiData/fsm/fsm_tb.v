`timescale 1 ns / 100 ps		//time unit = 1ns
module fsm_tb;
reg clk, reset, start, skip3, wait3;
wire [2:0] zot;

fsm dut1(clk, reset, start, skip3, wait3, zot);

always #50 clk = ~clk;		//100 MHz

initial begin
		clk = 0; reset = 0;
		#1 reset = 1;
		#200 reset = 0;
end

initial begin
		start = 0; skip3 = 0; wait3 = 0;
		#1               
		#300 start = 1;                                    
		wait(zot == 3'b001);        //change from dut1.state3 to 3'b001
		wait(zot == 3'b001);   			//change from dut1.state3 to 3'b001
		#400 wait3 = 1;                                    
		#200 wait3 = 0;                                    
		wait(zot == 3'b001);   			//change from dut1.state3 to 3'b001
		skip3 = 1;                                         
		wait(zot == 3'b111);        //change from dut1.state2 to 3'b111
		repeat (1024) begin
    @(posedge clk);    // 等待上升沿
    skip3 = $random;   // 每个周期赋值
		end                      
		$stop;                                             
end
endmodule