# Robot Remote Controller

## Build Instructions

This project is structured into an application logic layer and harward abstraction layer, found in the app and hal directories, respectively.

* Expose app project header and source files to hal

```
cd app
cmake -S . -B build
```

* Build hal project

```
cd hal
cmake -S . -B build
cmake --build build
```

### Build Architecture Notes

* The app project pulls in interface files from out of the app cmake tree in the hal project's interfaces directory.
* The hal project must be built after the app project because hal expects app's header and source files to be exposed
via symbolic links in the hal's app directory.

## Run Instructions

The pico firmware is built in the hal project. Consult the hal's README for further instructions.

## Project Work Board

Open work items are tracked on Trello here: https://trello.com/invite/b/6a669386e64ffbf5a96052a2/ATTI7b9f91e58f2b5d30f21e4a6238c1df3bD1FA0A13/rc-car-project