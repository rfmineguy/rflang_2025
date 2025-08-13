# Build
The program is split in to multiple parts, the library and the exe
It additionally has a `normal` build and `debug` build

## Normal Build
```sh
make clean build PROFILING={ON/OFF, default OFF} DEBUG={ON/OFF, default ON}
```

1. Compile source files into objects
2. Link the objects into a static lib
3. Builds a driver executable

## Debug Build
```sh
make clean_preprocess build_preprocess PROFILING={ON/OFF, default OFF}
```

This build is slightly different, but allows for easier debugging of the macros within the application
1. Preprocess the source files so that the macros are expanded
2. Compile the preprocessed files into objects
3. Link the objects into a static lib
4. Builds a driver executable
