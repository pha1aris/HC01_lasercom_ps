# 
# Usage: To re-create this platform project launch xsct with below options.
# xsct E:\FPGA_Code\10G_1km\lasercom_ps_total\lasercom_ps_total_v12\lasercom_pl_total_v0_wrapper\platform.tcl
# 
# OR launch xsct and run below command.
# source E:\FPGA_Code\10G_1km\lasercom_ps_total\lasercom_ps_total_v12\lasercom_pl_total_v0_wrapper\platform.tcl
# 
# To create the platform in a different location, modify the -out option of "platform create" command.
# -out option specifies the output directory of the platform project.

platform create -name {lasercom_pl_total_v0_wrapper}\
-hw {E:\FPGA_Code\10G_1km\lasercom_ps_total\lasercom_ps_total_v12\lasercom_pl_total_v0_wrapper.xsa}\
-arch {64-bit} -fsbl-target {psu_cortexa53_0} -out {E:/FPGA_Code/10G_1km/lasercom_ps_total/lasercom_ps_total_v12}

platform write
domain create -name {standalone_psu_cortexa53_0} -display-name {standalone_psu_cortexa53_0} -os {standalone} -proc {psu_cortexa53_0} -runtime {cpp} -arch {64-bit} -support-app {empty_application}
platform generate -domains 
platform active {lasercom_pl_total_v0_wrapper}
domain active {zynqmp_fsbl}
domain active {zynqmp_pmufw}
domain active {standalone_psu_cortexa53_0}
platform generate -quick
platform generate
platform generate
