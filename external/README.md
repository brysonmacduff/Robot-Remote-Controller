# External Dependencies

This directory contains external dependencies of the robot remote controller project.

## Git Submodule Management

You must ensure that the git submodules in this directory are updated before attemping to build them.

```
git submodule update --init --recursive
```

## Build RF24 for Raspberry Pi Pico W

```
cd RF24
mkdir build
cd build
cmake ../examples_pico -DCMAKE_BUILD_TYPE=Release -DPICO_BOARD=pico_w -DPICO_SDK_PATH=../../pico-sdk
cmake --build . --config Release
```

## Build MTP32 for Raspberry Pi Pico W

Follow the build instructions in the README file under:

```
MTP32/lib/platforms/pico-armv6m
```

## Build Robot Middleware for Raspberry Pi Pico W

Follow the build instructions in the README file under:

```
robot-controller/robot_middleware/platforms/pico-armv6m
```
