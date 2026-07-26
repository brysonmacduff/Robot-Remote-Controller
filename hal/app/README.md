# App

This directory functions as an easily accessible shortcut to the application logic layer source files.

When the app project is built, it copies its header and source files into this directory so they can be
discovered by the hal project.

## A Note About Cross-Platform Dependencies

This style of application layer source code integration is less formal than building a dedication pico-compatible
application layer library that can be linked to the hardware abstraction layer. The reason for this is to simplify the
cmake code and also because it is probably not worth it to build standalone pico libraries that are only usuable in this project.

### Instructions

To ensure the application code files are symbolically linked to this directory, run the following command:

```
cmake -S . -B build
```