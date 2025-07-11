onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate /i2c_tb/i2c_dut/clk
add wave -noupdate /i2c_tb/i2c_dut/rstn
add wave -noupdate /i2c_tb/i2c_dut/write_op
add wave -noupdate /i2c_tb/i2c_dut/write_data
add wave -noupdate /i2c_tb/i2c_dut/read_op
add wave -noupdate /i2c_tb/i2c_dut/read_data
add wave -noupdate /i2c_tb/i2c_dut/addr
add wave -noupdate /i2c_tb/i2c_dut/op_done
add wave -noupdate /i2c_tb/i2c_dut/scl
add wave -noupdate /i2c_tb/i2c_dut/sda
add wave -noupdate /i2c_tb/i2c_dut/div_cnt
add wave -noupdate /i2c_tb/i2c_dut/scl_tick
add wave -noupdate /i2c_tb/i2c_dut/clr_scl
add wave -noupdate /i2c_tb/i2c_dut/d5ms_over
add wave -noupdate /i2c_tb/i2c_dut/d5ms_cnt
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {0 ps} 0}
quietly wave cursor active 1
configure wave -namecolwidth 198
configure wave -valuecolwidth 100
configure wave -justifyvalue left
configure wave -signalnamewidth 0
configure wave -snapdistance 10
configure wave -datasetprefix 0
configure wave -rowmargin 4
configure wave -childrowmargin 2
configure wave -gridoffset 0
configure wave -gridperiod 1
configure wave -griddelta 40
configure wave -timeline 0
configure wave -timelineunits ps
update
WaveRestoreZoom {0 ps} {3611249142724 ps}
