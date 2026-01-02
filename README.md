# RTS

RTS engine.

![Alt text](bin/docs/wargame_2008_07_13_10_57.png)
![Alt text](bin/docs/wargame_2008_08_09_20_08.png)
![Alt text](bin/docs/wargame_2008_08_09_20_09.png)
![Alt text](bin/docs/wargame_2008_08_10_13_11.png)
![Alt text](bin/docs/wargame_2008_08_10_13_12.png)
![Alt text](bin/docs/wargame_2008_08_13_11_00.png)
![Alt text](bin/docs/wargame_2008_08_13_11_01.png)

## Repository

Install Git LFS.

## Libraries

Use Visual Studio Express 2026 (v146).

### Boost 1.90.0

```cmd
.\bootstrap
.\b2 install --prefix=/sdk/Boost
```

### Bullet 3.25

```cmd
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=/sdk/Bullet ..
```

### Google Test 1.17.0

```cmd
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=./sdk/GoogleTest ..
```

### JsonCpp 1.9.6

```cmd
mkdir build
cd build
cmake -DCMAKE_DEBUG_POSTFIX=d -DCMAKE_INSTALL_PREFIX=/sdk/JsonCpp ..
```

### Ogre3D 1.14.1

```cmd
mkdir build
cd build
cmake -DOGRE_BUILD_PLUGIN_ASSIMP=OFF -DOGRE_BUILD_PLUGIN_DOT_SCENE=OFF -DOGRE_BUILD_TOOLS=OFF -DCMAKE_INSTALL_PREFIX=/sdk/Ogre ..
```

### OIS 1.5.1

```cmd
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=/sdk/OIS ..
```

### RmlUi 6.1

```cmd
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=/sdk/RmlUi .. ```

## Build

Use Visual Studio Express 2019 (v142).

```cmd
mkdir build
cd build
cmake .. \
    -DBBoost_DIR="/sdk/Boost/lib64-msvc-14.3/cmake/Boost-1.90.0" /
    -DFreetype_ROOT="/lib/ogre/build/Dependencies" /
    -DGTest_DIR="/sdk/GoogleTest/lib/cmake/GTest" /
    -Djsoncpp_DIR="/sdk/JsonCpp/lib/cmake/jsoncpp" /
    -DOGRE_DIR="/sdk/Ogre/CMake" /
    -DOIS_DIR="/sdk/OIS" /
    -DRmlUi_DIR="/sdk/RmlUI/lib/cmake/RmlUi"
```

## Meshes

### Cube

Order of sides:

- top,
- bottom,
- front,
- back,
- left,
- right.

Indices:

```cmd
 0,  1,  2,  0,  2,  3,
 4,  5,  6,  4,  6,  7,
 8,  9, 10,  8, 10, 11,
12, 13, 14, 12, 14, 15,
16, 17, 18, 16, 18, 19,
20, 21, 22, 20, 22, 23
```

Normals:

```cmd
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

```cmd
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
