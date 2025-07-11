onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate /fsm_tb/dut1/clk
add wave -noupdate /fsm_tb/dut1/reset
add wave -noupdate /fsm_tb/dut1/start
add wave -noupdate /fsm_tb/dut1/skip3
add wave -noupdate /fsm_tb/dut1/wait3
add wave -noupdate /fsm_tb/dut1/zot
add wave -noupdate /fsm_tb/dut1/next_st
add wave -noupdate /fsm_tb/dut1/state
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {536700 ps} 0}
quietly wave cursor active 1
configure wave -namecolwidth 154
configure wave -valuecolwidth 76
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
WaveRestoreZoom {0 ps} {1791600 ps}
