# Building PS4Delta

To build PS4Delta, you need the following dependencies:

* A Windows machine with Visual Studio 2019 (Community Edition is fine too)
* Clang-9 for windows (available [here](http://releases.llvm.org/download.html))
* LLVM Utils for Visual Studio (available [here](https://github.com/zufuliu/llvm-utils/releases))
* Qt >= 5.12

Then you can execute pmake.cmd in order to generate the project solution.
The PS4Delta solution will be placed in the build folder.