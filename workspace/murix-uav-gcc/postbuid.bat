

arm-none-eabi-gcc -v

@echo "create flash images"
arm-none-eabi-objcopy -O binary murix-uav-gcc.elf murix-uav-gcc.bin
arm-none-eabi-objcopy -O ihex   murix-uav-gcc.elf murix-uav-gcc.hex

@echo "print size"
@echo "---------------------------------------"
@echo "text - shows the code and read-only data in your application (in decimal)"
@echo "data - shows the read-write data in your application (in decimal)"
@echo "bss - show the zero initialized ('bss' and 'common') data in your application (in decimal)"
@echo "dec - total of 'text' + 'data' + 'bss' (in decimal)"
@echo "hex - hexidecimal equivalent of 'dec'"
@echo "---------------------------------------"
@echo "the flash consumption of your application will then be text + data"
@echo "the RAM consumption of your application will then be data + bss."
@echo "---------------------------------------"
arm-none-eabi-size              murix-uav-gcc.elf

@echo "create listing"
arm-none-eabi-objdump -S        murix-uav-gcc.elf > murix-uav-gcc.lst

@echo "create sizes"
arm-none-eabi-nm -S --size-sort -s murix-uav-gcc.elf > murix-uav-gcc.sizes

