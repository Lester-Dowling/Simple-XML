# Simple-XML
An abridged grammar to parse simple XML files such as from Microsoft Excel.

Intended as a git submodule within a larger project.

## Operational

This repo is known to work and be operational with the following versions:

| Version         | Comment                                              |
| --------------- | ---------------------------------------------------- |
| C++17           | Both g++ 8.3.0 and cl.exe 19.24.28117                |
| Boost 1.71.0    | Requires custom install.  See my Boost scripts repo. |
| CMake 3.15      | Requires custom install or custom build in Ubuntu.   |
| GraphViz 2.38.0 | GraphViz's bin directory is assumed to be in PATH.   |

GraphViz parses the DOT output from Boost.Test in order to automatically capture
the list of tests for CTest to run.  GraphViz is normally installed when
installing Doxygen.

CMake version 3.15 is used to follow modern CMake best practices.

## Unit Tests

The unit tests of this repo are intended for CTest to execute.  Unit test
execution is known to work under:

| IDE                                    | Version                        |
| -------------------------------------- | ------------------------------ |
| Microsoft Visual Studio Community 2019 | 16.4.0 Preview 1.0             |
| Microsoft Visual Studio Code           | 1.38 for Windows 10 and Ubuntu |

### Ubuntu Terminal

Run unit tests from Ubuntu terminal:

```Bash
    > cd String-Extra
    > mkdir build
    > cd build
    > cmake ..
    > cd ..
    > cmake --build build
    > cd build
    > ctest
```

### Cmd

Run unit tests from the x64 Native Tools Command Prompt for VS 2019 Preview:

```cmd
    > cd String-Extra
    > mkdir build
    > cd build
    > cmake -G Ninja -D CMAKE_BUILD_TYPE=Release .. 
    > cd ..
    > cmake --build build
    > cd build
    > ctest
```
