onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate /fsm_tb/clk
add wave -noupdate /fsm_tb/reset
add wave -noupdate /fsm_tb/start
add wave -noupdate /fsm_tb/skip3
add wave -noupdate /fsm_tb/wait3
add wave -noupdate /fsm_tb/zot
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {0 ps} 0}
quietly wave cursor active 0
configure wave -namecolwidth 150
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
WaveRestoreZoom {305700 ps} {4165800 ps}
