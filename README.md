
# Momentum Mod Workshop Upload Tool
A desktop tool for uploading Momentum Mod maps and other UGC to the Steam Workshop. Shipped with Momentum Mod on Steam.

## Building

#### Dependencies:
```
cmake 3.0 or newer
C++14 compiler
Qt5.6 or newer (Any version of Qt5 *should* work though)
Steamworks SDK (see steamworks/readme.txt)
```

Don't forget: you must put steam_appid.txt with the correct Momentum Mod appid in the same directory as the executable! 

To build and run on Linux/OSX: 

```
$ cd workshop-tool
$ cmake -H. -Bbuild
$ cmake --build build --config release -- -j4
...
$ cd build
$ ./momentum-workshop-tool
```  

On Windows:

```
_ cmake -G "Visual Studio 15 Win64" -H. -Bbuild
_ cmake --build build --config release
...
_ cd build/Release
_ momentum-workshop-tool
```
