# Build dependencies

* A Windows machine with Visual Studio 2019 (Community Edition is fine too)
* Clang-cl for windows (install it from your visual studio installer)
* Qt >= 5.12 with `msvc2017_64` (Ensure that the `QTDIR` or `QT_DIR` environment variable are set). It can be found [here](https://www.qt.io/offline-installers)

# Setup the project

```
git clone https://github.com/Force67/ps4delta.git
cd ps4delta
git submodule update --init
```

#### Building the project

Execute pmake.cmd in order to generate the project solution. 

Afterwards you can open `build/PS4Delta.sln`. The recommended build configuration is `Release` for all purposes.

Don't forget to copy the QT runtime dlls in the output directory (can be found at `%QT_DIR%/bin`)
You will need for __release__ : `Qt5core.dll` `Qt5gui.dll` `Qt5Qml.dll` `Qt5Widgets.dll` `Qt5WinExtras.dll` `Qt53DRender.dll`
And for  __debug__ : `Qt5cored.dll` `Qt5guid.dll` `Qt5Qmld.dll` `Qt5Widgetsd.dll` `Qt5WinExtrasd.dll` `Qt53DRenderd.dll`

Then you will need to do the actual installation of the project by following these instructions [here](https://github.com/Force67/ps4delta/blob/master/docs/installation.md)

#### Troubleshooting

Q - I don't see any project solution in `/build` folder
A - You might need to download the Qt offline installer as shown above if you haven't got it yet. Also make sure you QT_DIR are set to something like `C:\Qt\Qt5.14.0\5.14.0\msvc2017_64`

Q - I got the error `MSB8020` when building the project with VS2019
A - It means that you haven't got any tools for building the solution, so you might need to the VS installer, modify VS2019 and add both `C++ Clang` and `C++ Clang-cl`. 

Q - When I start the app it shows `Unable to load core`
A - First check that you have built the entire solution and not just the host project. Then check if in the `bin/Debug` folder, there is a file named `dcore.dll`. Also, double check that you copied all the runtime dlls in the output folder.

Q - When I start the app in release it crashes and when I start the app in debug it shows `This application failed to start because no Qt platform plugin could be initialized.`
A - Copy the platforms folder from your `%QT_DIR%/plugins/` to your target folder (Debug or Release)

If you have other issues at the building process, feel free to ask them on our discord server [here](https://discord.gg/WqWjujt).