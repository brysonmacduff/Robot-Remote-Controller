# Application Directory

This directory contains source code for the application-level lopic of the robot remote controller.

## Platform Architecture Warning

- The cmake targets in this directory cannot be linked to the cmake targets in the hal directory.
  - The hal directory builds for a different architecture than the app directory.
- Therefore, the hal directory must pull in app directory source code files directly if it wants to use code from this directory.
  - Likewise, if source code from the hal directory is needed in the app directory, such as interface classes, then the same procedure must be followed.

## Build Instructions

```
cmake -S . -B build
cmake --build build
```
