# Build dependencies

* A Windows machine with Visual Studio 2019 (Community Edition is fine too)
* Clang-9 for windows (available [here](http://releases.llvm.org/download.html))
* Clang-cl for windows (install it from your visual studio installer)
* Qt >= 5.12 (Ensure that the QTDIR environment variable is set)

# Setup the project

```
git clone https://github.com/Force67/ps4delta.git
cd ps4delta
git submodule update --init
```

#### Building the project

Execute pmake.cmd in order to generate the project solution. 

Afterwards you can open `build/PS4Delta.sln`. The recommended build configuration is `Release` for all purposes.
