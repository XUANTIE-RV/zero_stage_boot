make clean
make
cp zero_stage_boot.* ~/buildroot/thead_910_compat_5.1_glibc_br_defconfig/images/hw/
~/nixon/bin/riscv64-unknown-linux-gnu-objdump -d zero_stage_boot.elf > 123
