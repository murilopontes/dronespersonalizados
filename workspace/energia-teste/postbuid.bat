

arm-none-eabi-gcc -v

@echo "create flash images"
arm-none-eabi-objcopy -O binary energia-teste.elf energia-teste.bin
arm-none-eabi-objcopy -O ihex   energia-teste.elf energia-teste.hex

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
arm-none-eabi-size              energia-teste.elf

@echo "create listing"
arm-none-eabi-objdump -S        energia-teste.elf > energia-teste.lst


@rem arm-none-eabi-objdump energia-teste.elf --source --all-headers --demangle --line-numbers > energia-teste.lst
@rem arm-none-eabi-size ${BuildArtifactFileName}; 
@rem arm-none-eabi-objdump -S ${BuildArtifactFileName} >${BuildArtifactFileBaseName}.lss;
@rem arm-none-eabi-objcopy -O ihex ${BuildArtifactFileName} ${BuildArtifactFileBaseName}.hex;
@rem arm-none-eabi-nm -S --size-sort -s energia-teste.elf 


