module ins_decode(
      //ir,en,mova,movb,movc,add,sub,jmp,jg,inl,outl,movi,halt
      input wire en,
      input wire [3:0] ir,
      output reg add,sub,mova,movb,jmp,jg,in1,movd,out1,movc,movi,halt
      );
      always @(ir or en)
      begin 
      {mova,movd,add,movb,jg,jmp,movi,in1,out1,movc,sub,halt}=0;
      
      if(en) 
      begin
      if(ir[3:2]==2'b01)
      begin
      if(ir[1:0]==2'b00) mova=1;
      else if(ir[1:0]==2'b10) movc=1;
      else if(ir[1:0]==2'b01) movb=1;
      else movd=1;
      end
      
      else if(ir[3:2]==2'b10)
      begin 
      if(ir[1:0]==2'b00) add=1;
      else if(ir[1:0]==2'b10) jmp=1;
      else if(ir[1:0]==2'b01) sub=1;
      else jg=1;
      end
      
      else 
      begin
     if(ir[1:0]==2'b00) in1=1; 
      else if(ir[1:0]==2'b10) movi=1;
      else if(ir[1:0]==2'b01) out1=1;
      else halt=1;
      end
      end
      end
      endmodule