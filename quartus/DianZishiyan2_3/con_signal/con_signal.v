module con_signal(
	input wire [7:0] ir,
	input wire mova,sub,g,movi,movc,movb,in1,jmp,out1,halt,movd,add,jg,sm,
	output reg [3:0] au_ac,
	output reg [1:0] reg_sr,reg_dr,s,
	output reg ram_wr,reg_we,out_en,sm_en,au_en,pc_in,in_en,ir_ld,pc_ld,ram_re,gf_en,mux_s
);
always @(*) begin
	gf_en=sub;
	pc_in=movi|(~sm);
	ir_ld=~sm;
	reg_we=mova|movc|movd|movi|add|sub|in1;
	pc_ld=jmp|(g&jg);
	au_ac=ir[7:4];
	mux_s=mova|movc|movi|add|sub|in1;
	ram_re=(~sm)|movc|movi;
	out_en=out1;
	ram_wr=movb;
	in_en=in1;
	sm_en=~halt;
	au_en=mova|movb|out1|sub|add;
	
	if(movb) s=2'b10;
	else if(movc) s=2'b01;
	else s=2'b00;
	reg_dr=ir[3:2];
	reg_sr=ir[1:0];
	end
endmodule