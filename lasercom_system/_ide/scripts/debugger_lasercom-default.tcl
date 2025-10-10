# Usage with Vitis IDE:
# In Vitis IDE create a Single Application Debug launch configuration,
# change the debug type to 'Attach to running target' and provide this 
# tcl script in 'Execute Script' option.
# Path of this script: E:\FPGA_Code\10G_1km\lasercom_ps_total\lasercom_ps_total_v12\lasercom_system\_ide\scripts\debugger_lasercom-default.tcl
# 
# 
# Usage with xsct:
# To debug using xsct, launch xsct and run below command
# source E:\FPGA_Code\10G_1km\lasercom_ps_total\lasercom_ps_total_v12\lasercom_system\_ide\scripts\debugger_lasercom-default.tcl
# 
connect -url tcp:127.0.0.1:3121
source E:/Xilinx/Vitis/2023.2/scripts/vitis/util/zynqmp_utils.tcl
targets -set -nocase -filter {name =~"APU*"}
rst -system
after 3000
targets -set -filter {jtag_cable_name =~ "Digilent JTAG-HS1 210512180081" && level==0 && jtag_device_ctx=="jsn-JTAG-HS1-210512180081-14710093-0"}
fpga -file E:/FPGA_Code/10G_1km/lasercom_ps_total/lasercom_ps_total_v12/lasercom/_ide/bitstream/lasercom_pl_total_v0_wrapper.bit
targets -set -nocase -filter {name =~"APU*"}
loadhw -hw E:/FPGA_Code/10G_1km/lasercom_ps_total/lasercom_ps_total_v12/lasercom_pl_total_v0_wrapper/export/lasercom_pl_total_v0_wrapper/hw/lasercom_pl_total_v0_wrapper.xsa -mem-ranges [list {0x80000000 0xbfffffff} {0x400000000 0x5ffffffff} {0x1000000000 0x7fffffffff}] -regs
configparams force-mem-access 1
targets -set -nocase -filter {name =~"APU*"}
set mode [expr [mrd -value 0xFF5E0200] & 0xf]
targets -set -nocase -filter {name =~ "*A53*#0"}
rst -processor
dow E:/FPGA_Code/10G_1km/lasercom_ps_total/lasercom_ps_total_v12/lasercom_pl_total_v0_wrapper/export/lasercom_pl_total_v0_wrapper/sw/lasercom_pl_total_v0_wrapper/boot/fsbl.elf
set bp_23_11_fsbl_bp [bpadd -addr &XFsbl_Exit]
con -block -timeout 60
bpremove $bp_23_11_fsbl_bp
targets -set -nocase -filter {name =~ "*A53*#0"}
rst -processor
dow E:/FPGA_Code/10G_1km/lasercom_ps_total/lasercom_ps_total_v12/lasercom/Debug/lasercom.elf
configparams force-mem-access 0
targets -set -nocase -filter {name =~ "*A53*#0"}
con
