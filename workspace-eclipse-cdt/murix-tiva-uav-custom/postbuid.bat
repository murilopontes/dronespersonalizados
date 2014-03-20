

arm-none-eabi-gcc -v

@echo "create flash images"
arm-none-eabi-objcopy -O binary murix-uav-gcc.elf murix-uav-gcc.bin
arm-none-eabi-objcopy -O ihex   murix-uav-gcc.elf murix-uav-gcc.hex

@echo "print size"
arm-none-eabi-size              murix-uav-gcc.elf

@echo "create listing"
arm-none-eabi-objdump -S        murix-uav-gcc.elf > murix-uav-gcc.lst

@echo "create sizes"
arm-none-eabi-nm -S --size-sort -s murix-uav-gcc.elf > murix-uav-gcc.sizes

