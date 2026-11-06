# Licensing and legal notice for Shipwright_HaikuOS

This repository is an **unofficial** fork of [HarbourMasters/Shipwright](https://github.com/HarbourMasters/Shipwright)
(Ship of Harkinian) that adds support for [Haiku](https://www.haiku-os.org/). It is not made, endorsed or supported by
HarbourMasters, and it is not affiliated with or endorsed by Nintendo.

*The Legend of Zelda* and *Ocarina of Time* are trademarks of Nintendo. This repository contains no Nintendo game
assets. To play, you need your own legally obtained copy of the game; assets are extracted from it on your machine.

## What is licensed how

This tree mixes code under different terms. Nothing in this fork changes the license of anyone else's code.

| Part | Where | License | Copyright |
|---|---|---|---|
| Ship of Harkinian (upstream code) | everything not listed below | **No license published upstream** (see below) | HarbourMasters and Shipwright contributors |
| Haiku port additions | listed under "Haiku port files" | MIT (text below) | 2026 Techniix |
| libultraship (submodule) | `libultraship/` | MIT | 2022 kenix3 and contributors |
| Fast3D renderer | `libultraship/src/fast/` | MIT | 2020 Emill, MaikelChan |
| Torch (submodule) | `torch/` | MIT | 2023 Lywx and contributors |
| StormLib (in Torch) | `torch/lib/StormLib/` | MIT | Ladislav Zezula |
| 3DS randomizer code | `soh/soh/Enhancements/randomizer/3drando/` | MIT | 2017 Amazing Ampharos (OoT-Randomizer) |
| portable-file-dialogs | `soh/soh/Extractor/portable-file-dialogs.h` | WTFPL 2 | 2018–2022 Sam Hocevar |
| Fonts | `soh/assets/custom/fonts/` | SIL OFL 1.1 (Montserrat, Inconsolata, Press Start 2P, Noto Sans JP); Fipps: see note | their respective authors |
| Font Awesome 4 icon font | inside libultraship | SIL OFL 1.1 | Dave Gandy |

Libraries downloaded at build time keep their own licenses, including Dear ImGui (MIT), spdlog (MIT),
yaml-cpp (MIT), tinyxml2 (zlib), Microsoft GSL (MIT), libgfxd (MIT), stb_image (MIT or public domain),
dr_libs (public domain or MIT-0), SDL2 and SDL2_net (zlib), libzip (BSD-3-Clause), zlib (zlib),
Ogg, Vorbis and Opus (BSD-3-Clause) and nlohmann/json (MIT).

**Fipps font:** the tree doesn't include its license. Check the font author's terms before redistributing
binaries that contain it.

### Ship of Harkinian has no published license

The upstream repository has no LICENSE file and GitHub shows no license for it. Without a license, copyright law
reserves all rights to the authors. Making a public fork on GitHub is allowed by GitHub's Terms of Service, so this
repository can host the source. **No license grants the right to redistribute Ship of Harkinian outside GitHub**,
for example as a built package on HaikuDepot or on another site. Get permission from HarbourMasters before doing that.

## Haiku port files

These files were added for the Haiku port and are released under the MIT license below:

- `CMake/HaikuPatches.cmake`
- `docs/HAIKU.md`
- `scripts/haiku/` (including `patches/libultraship-haiku.patch`, which modifies MIT-licensed libultraship)
- `soh/haiku/`
- `soh/soh/platform/haiku/`

The small Haiku-specific edits to existing upstream files (`#ifdef __HAIKU__` blocks and CMake `Haiku` branches)
are offered under the same MIT terms, to the extent the port's author holds rights in them.

```
MIT License

Copyright (c) 2026 Techniix

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```
