# Ship of Harkinian on Haiku

This is a port of Ship of Harkinian to [Haiku](https://www.haiku-os.org/) (x86_64). It uses SDL2 for
windowing, input and audio, and OpenGL through Haiku's Mesa.

## Requirements

- Haiku R1/beta5 or a recent nightly, **x86_64** (32-bit x86 is not supported)
- About 4 GB of free space for the build
- Your own Ocarina of Time ROM. Supported versions are listed at https://ship.equipment/

## Building

```sh
git clone https://github.com/Techniix/Shipwright_HaikuOS.git
cd Shipwright_HaikuOS

# First time: installs the dependencies in scripts/haiku/pkgman.txt, then builds
scripts/haiku/build.sh --deps

# Later builds
scripts/haiku/build.sh
```

The game ends up in `build-haiku/soh/`. To make a zip you can copy to another machine, run
`scripts/haiku/build.sh --package`; it is written to `_packages/`.

Building by hand works the same way as on Linux:

```sh
git submodule update --init
cmake -S . -B build-haiku -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build-haiku --target GenerateSohOtr
cmake --build build-haiku
```

If `pkgman` can't find one of the packages, its name may have changed in HaikuPorts.
`pkgman search <name>` finds the current one.

## Running

Double-click `soh` in Tracker or run `build-haiku/soh/soh` from Terminal. On first launch the game asks
for your ROM and builds `oot.o2r` (or `oot-mq.o2r`) from it. You can also put the ROM next to `soh` before
launching, or pass it on the command line: `soh /path/to/rom.z64`.

### Where files go

Settings, saves, mods and the generated `.o2r` archives are kept **next to the `soh` executable**, so the
folder is self-contained, as with the Windows and Linux zips.

- Set `SHIP_HOME=/some/folder` to keep them somewhere else.
- If the executable's folder isn't writable (for example a packaged install under `/boot/system/apps`),
  the game uses `~/config/settings/soh/` instead.

## Graphics

Haiku's default OpenGL is Mesa's software renderer (llvmpipe), which runs everything on the CPU. Expect
full speed only on a fast CPU at a modest window size. Things that help:

- Lower the internal resolution in **Settings > Graphics**.
- Leave MSAA off.
- With experimental GPU drivers (for example NVK + Zink on NVIDIA), the game uses them automatically
  through the same OpenGL path.

## Haiku-specific behaviour

- **ROM picker** is a native Tracker file panel, filtered to `.z64`, `.n64` and `.v64`.
- **Menus stay inside the game window.** ImGui's pop-out windows (viewports) are turned off on Haiku
  because SDL gives every window its own GL context there. Set `SHIP_HAIKU_VIEWPORTS=1` to try them anyway.
- **Audio** uses SDL's Media Kit backend.
- **Controllers** use SDL's Haiku joystick support. Keyboard and mouse work as usual.
- **Text to speech** only works when the game is built with `espeak-ng` installed.
- **Crashes** go to Haiku's debugger instead of SoH's crash report window.

## How the port is put together

- Haiku support for **libultraship** is kept as a patch, `scripts/haiku/patches/libultraship-haiku.patch`, so the
  submodule stays on its upstream commit. CMake applies it automatically when configuring on Haiku
  (`CMake/HaikuPatches.cmake`). If upstream moves the submodule and the patch stops applying, configure stops
  with an error saying so; refresh the patch against the new commit.
- Ship of Harkinian's own Haiku changes are ordinary commits in this repository, guarded by `__HAIKU__`
  or CMake `Haiku` checks so other platforms build exactly as before.

## Troubleshooting

- **Black window or no menus:** check that Mesa is installed (`pkgman install mesa`). Running from
  Terminal shows the OpenGL renderer and any errors in the log.
- **"Missing soh.o2r":** the build didn't produce `soh.o2r`. Run
  `cmake --build build-haiku --target GenerateSohOtr` and check that `build-haiku/soh/soh.o2r` exists.
- **The game uses the wrong data folder:** check whether `SHIP_HOME` is set in your environment.
