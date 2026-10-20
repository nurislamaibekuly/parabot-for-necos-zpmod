# Parabot for necos-zpmod

Fork of [Parabot](https://github.com/nekonomicon/Parabot) patched to play
[necos-zpmod](https://github.com/nurislamaibekuly/necos-zpmod) (Zombie Plague):
ZP item awareness (molotov, freeze/infection bomb, turret kit), zombie turret
attacks, and admin bot commands (`pb_botcount`, `pb_addbot`, `pb_removebot`).
Built by the main project's CMake (`external/parabot`), not by these makefiles.

Human-like artificial player for Half-Life and some other games on GoldSrc and Xash3D game engines.

- [Full information](https://github.com/nekonomicon/Parabot/blob/master/addons/parabot/Readme.txt)

## How to build

### Windows

#### Visual Studio

TODO

#### MinGW

```
	$ cd dlls
	$ make
```

### Linux/OS X

```
	$ cd dlls
	$ make
```

### *BSD

```
	% cd dlls
	% gmake
```

### Android

Look **android/build.sh** script.

### GNU Make build options

`PB_64BIT` - 64-bit build(Actually for amd64/aarch64)

`DEBUG` - Debug build.

`AGGR_OPT` - Aggressive optimizations.

`SIZE_OPT` - Size optimizations.

`LTO` - Link-Time optimization(required [gold](http://llvm.org/docs/GoldPlugin.html)).

`NO_EXCEPTIONS` - Cut any debug information(frame-pointers, exception and unwind tables).

`STATIC_GCCLIBS` - static link with libgcc and libstd++(Actually on linux for portable builds when using gcc).

#### Example

```
	$ make PB_64BIT=0 DEBUG=0 AGGR_OPT=1 NO_EXCEPTIONS=1
```
