# Hardware Abstraction Layer Directory

## Remote GDB Debugging Instructions

### Debugging Requirements

- GDB Multiarch
```
sudo apt install gdb-multiarch
```
- Open OCD
```
git clone https://github.com/raspberrypi/openocd
cd openocd
./bootstrap
./configure --enable-ftdi --enable-sysfsgpio --enable-bcm2835gpio --enable-cmsis-dap
make -j$(nproc)
sudo make install
```

### Debugging CLI Option

- Run openocd
```
openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c "adapter speed 4000" -c "transport select swd"
```
- Run GDB multiarch
```
arm-none-eabi-gdb <firmware name>.elf
```
- Connect GDB to Target
```
target remote localhost:3333
```
- Load ELF
```
load
```
- Prepare for debugging
```
monitor reset init
```
- Continue
```
continue
```

### Debugging VS Code GUI Option

- Select Run > Start Debugging
    - You can also set the "runToEntryPoint" field in launch.json to break firmward execution at a specified point by default.