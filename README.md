# RTS

RTS engine written in C++ using only open source libraries. Features:

- Unit editor.
- Path finding.
- Technology tree.
- Diplomacy.
- Order queue.
- Skills.
- Destructible terrain.
- Custom memory manager.
- UI based on HTML and CSS.

## Media

<img src="bin/docs/wargame_2008_07_13_10_57.png" width="640" />
<img src="bin/docs/wargame_2008_08_09_20_08.png" width="640" />
<img src="bin/docs/wargame_2008_08_09_20_09.png" width="640" />
<img src="bin/docs/wargame_2008_08_10_13_11.png" width="640" />
<img src="bin/docs/wargame_2008_08_10_13_12.png" width="640" />
<img src="bin/docs/wargame_2008_08_13_11_00.png" width="640" />
<img src="bin/docs/wargame_2008_08_13_11_01.png" width="640" />

## Repository

Install Git LFS.

## Libraries

Use Microsoft Visual Studio 2026 (v146).

### Boost 1.90.0

```bash
.\bootstrap
.\b2 install --prefix=/sdk/Boost
```

### Bullet 3.25

```bash
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=/sdk/Bullet ..
```

### Google Test 1.17.0

```bash
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=./sdk/GoogleTest ..
```

### JsonCpp 1.9.6

```bash
mkdir build
cd build
cmake -DCMAKE_DEBUG_POSTFIX=d -DCMAKE_INSTALL_PREFIX=/sdk/JsonCpp ..
```

### Ogre3D 14.5.1

```bash
mkdir build
cd build
cmake -DOGRE_BUILD_PLUGIN_ASSIMP=OFF -DOGRE_BUILD_PLUGIN_DOT_SCENE=OFF -DOGRE_BUILD_TOOLS=OFF -DCMAKE_INSTALL_PREFIX=/sdk/Ogre ..
```

### OIS 1.5.1

```bash
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=/sdk/OIS ..
```

### RmlUi 6.2

```bash
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=/sdk/RmlUi ..
```

## Build

Use Visual Studio Express 2019 (v142).

```bash
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

## Code formatting

### Class

```cpp
namespace Gecko
{
    class GameObject :
        public Updatable
    {
    // From Updatable.
    public:
        void update(float time) override;

    // Constructors, destructors.
    public:
        GameObject();

    // Methods.
    public:
        void move();
        void rotate();

    // Getters.
    public:
        int get_damage() const;
        int get_size() const;
        int get_speed() const;

    // Setters.
    public:
        void set_damage(int damage);
        void set_size(int size);
        void set_speed(int speed);

    // Operators.
    public:
        operator bool() const;

    // Protected members.
    protected:
        int m_damage = 0;
        int m_speed = 0;

    // Private members.
    private:
        int m_size = 0;
    };
}
```
