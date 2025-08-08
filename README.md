![macos-arm status](https://github.com/rfmineguy/rflang_2025/actions/workflows/macos-arm.yml/badge.svg?branch=main&label=MacOS%20ARM%20Build)
![linux-x64 status](https://github.com/rfmineguy/rflang_2025/actions/workflows/linux-x64.yml/badge.svg?branch=main&label=MacOS%20ARM%20Build)
![windows status](https://github.com/rfmineguy/rflang_2025/actions/workflows/windows.yml/badge.svg?branch=main&label=MacOS%20ARM%20Build)

# rflang_2025
I'm beginning again an endeavor to write a more complicated language compiler.


# Building Compiler
When in the root folder...
```sh
make clean build PROFILING={ON/OFF, default OFF}
```


# Building/Running Tests
When in the tests/ folder...
```sh
make clean build_tests run_tests
```


# Profiling
Courtesy of [spall-web](https://github.com/colrdavidson/spall-web/tree/master) this repo can automatically generate performance traces. This is controlled via the `PROFILING` flag demoed in [Building Compiler](#building-compiler)
