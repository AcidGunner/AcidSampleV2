# AcidSampleV2
The PS3 Sample using some of my knowledge, I don't know :P

# How to compile?
You need a machine running Linux. WSL is supported.

1. Get [PS3DEV](https://github.com/ps3dev/ps3dev) and it's dependencies.
2. Copy `/usr/local/ps3dev/portlibs/ppu/include/freetype2/freetype` folder to `/usr/local/ps3dev/portlibs/ppu/include` cuz idk why they don't fix it.
3. Compile the homebrew using `make pkg` command.
4. Install PKG in your PS3 system.