# RTS

RTS engine.

![Alt text](/docs/wargame_2008_07_13_10_57.png)
![Alt text](/docs/wargame_2008_08_09_20_08.png)
![Alt text](/docs/wargame_2008_08_09_20_09.png)
![Alt text](/docs/wargame_2008_08_10_13_11.png)
![Alt text](/docs/wargame_2008_08_10_13_12.png)
![Alt text](/docs/wargame_2008_08_13_11_00.png)
![Alt text](/docs/wargame_2008_08_13_11_01.png)

## Repository

Install Git LFS.

## Libraries

Use Visual Studio Express 2019 (v142).

### Boost 1.75.0

```bash
.\bootstrap
.\b2 install --prefix=..\..\lib\Boost
```

### Bullet 3.06

```bash
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=..\..\..\lib\Bullet ..
```

### DirectX

Use installer.

### Google Test 1.10.0

```bash
mkdir build
cd build
```

Change `Multi-threaded` to `Multi-threaded DLL`, to avoid linking conflicts.

```bash
cmake -DCMAKE_INSTALL_PREFIX=..\..\..\lib\GoogleTest ..
```

### JsonCpp 1.9.4

```bash
mkdir build
cd build
cmake -DCMAKE_DEBUG_POSTFIX=d -DCMAKE_INSTALL_PREFIX=..\..\..\lib\JsonCpp ..
```

### Ogre3D 1.12.11

```bash
mkdir build
cd build
cmake -DOGRE_BUILD_PLUGIN_ASSIMP=OFF -DOGRE_BUILD_PLUGIN_DOT_SCENE=OFF -DOGRE_BUILD_TOOLS=OFF -DCMAKE_INSTALL_PREFIX=..\..\..\lib\Ogre ..
```

In CG project replace:

```C:\Program Files (x86)\NVIDIA Corporation\Cg\lib\cg.lib```

with:

```C:\Program Files (x86)\NVIDIA Corporation\Cg\lib.x64\cg.lib```

### OIS 1.5

```bash
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=..\..\..\lib\OIS ..
```

### Ultralight 1.2.1

```bash
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=..\..\..\lib\Ultralight ..
```

## Build

Use Visual Studio Express 2019 (v142).

```bash
mkdir build
cd build
cmake .. \
    -DBOOST_ROOT="D:/Projects/rts/lib/Boost" \
    -DCMAKE_INSTALL_PREFIX="../bin" \
    -DGOOGLETEST_ROOT="D:/Projects/rts/lib/GoogleTest" \
    -DJSONCPP_ROOT="D:/Projects/rts/lib/JsonCpp" \
    -DOGRE_ROOT="D:/Projects/rts/lib/Ogre" \
    -DOIS_ROOT="D:/Projects/rts/lib/OIS" \
    -DSDL_ROOT="D:/Projects/rts/dependencies/ogre-1.12.11/build/Dependencies" \
    -DULTRALIGHT_ROOT="D:/Projects/rts/lib/Ultralight"
```

## Cube

Order of sides:

- top,
- bottom,
- front,
- back,
- left,
- right.

Indices:

```bash
 0,  1,  2,  0,  2,  3,
 4,  5,  6,  4,  6,  7,
 8,  9, 10,  8, 10, 11,
12, 13, 14, 12, 14, 15,
16, 17, 18, 16, 18, 19,
20, 21, 22, 20, 22, 23
```

Normals:

```bash
 0.0,  1.0,  0.0,
 0.0,  1.0,  0.0,
 0.0,  1.0,  0.0,
 0.0,  1.0,  0.0,

 0.0, -1.0,  0.0,
 0.0, -1.0,  0.0,
 0.0, -1.0,  0.0,
 0.0, -1.0,  0.0,

 0.0,  0.0, -1.0,
 0.0,  0.0, -1.0,
 0.0,  0.0, -1.0,
 0.0,  0.0, -1.0,

 0.0,  0.0,  1.0,
 0.0,  0.0,  1.0,
 0.0,  0.0,  1.0,
 0.0,  0.0,  1.0,

-1.0,  0.0,  0.0,
-1.0,  0.0,  0.0,
-1.0,  0.0,  0.0,
-1.0,  0.0,  0.0,

 1.0,  0.0,  0.0,
 1.0,  0.0,  0.0,
 1.0,  0.0,  0.0,
 1.0,  0.0,  0.0
```

Vertices:

```bash
-0.5,  0.5, -0.5,
-0.5,  0.5,  0.5,
 0.5,  0.5,  0.5,
 0.5,  0.5, -0.5,

-0.5, -0.5, -0.5,
 0.5, -0.5, -0.5,
 0.5, -0.5,  0.5,
-0.5, -0.5,  0.5,

-0.5, -0.5, -0.5,
-0.5,  0.5, -0.5,
 0.5,  0.5, -0.5,
 0.5, -0.5, -0.5,

-0.5, -0.5,  0.5,
 0.5, -0.5,  0.5,
 0.5,  0.5,  0.5,
-0.5,  0.5,  0.5,

-0.5, -0.5, -0.5,
-0.5, -0.5,  0.5,
-0.5,  0.5,  0.5,
-0.5,  0.5, -0.5,

 0.5, -0.5, -0.5,
 0.5,  0.5, -0.5,
 0.5,  0.5,  0.5,
 0.5, -0.5,  0.5
```
