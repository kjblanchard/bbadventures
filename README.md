# Blanchard Babes Adventures
- A short C++ RPG.
- Uses C self made libraries for physics, sound, tweening, content loading and engine, found in the Libraries section
- BGMs created in Garageband
- Play the game here, or likely watch its "progress" [Play!](https://rpg.supergoon.com)
- ![Build All Platforms]( https://github.com/kjblanchard/bbadventures/actions/workflows/build.yml/badge.svg)
- ![Status Picture](https://github.com/kjblanchard/bbadventures/blob/master/img/debug.gif?raw=true)

## Development
- Mostly developed on macos apple silicon
- Builds on Mac locally until game is playable, Universal Mac, Ubuntu, and Emscripten via github actions

## Goals
- Better at C
- Better at C++
- Learn about low-level programming for pictures (libpng) sound (ogg/openal), text (freetype) instead of using pre-made libraries
- Better at cross-platform building with cmake (Macos, Linux, Windows, Emscripten)
- Learn github actions to build and publish outside developer environment

### Stretch Goals
- Game build on Mobile (IOS priority)

## Components
- SDL2: Low Level handling of windowing, events, input
- OpenAL: Cross-Platform sound abstraction
- Freetype: Text rendering
- Emscripten: Build for web

## Tools
- VS Code
- Garage Band - BGM
- Audacity - Tweaks
- Aseprite - Animations / Pixel art
- Tiled - Levels
- Make: Streamlining building with one command
- CMake: The actual cross-platform build system.

## Licenses
### Full Game
- MIT

## Libraries
- [Supergoon Engine](https://github.com/supergoongaming/sg_engine) - MIT - OpenAL, libpng/freetype wrapper, SDL wrapper, base content types, etc
- [nlohmann json](https://github.com/nlohmann/json) - MIT - json parsing for appsettings file
- [EnTT ECS](https://github.com/skypjack/entt/tree/v3.13.2) - MIT - Entity Component System

## Shout Outs
- [Time Fantasy Sprites](https://finalbossblues.com/timefantasy/)
- [Typing Text sound](https://freesound.org/people/Sky_Motion/sounds/416777/) - Creative Commons

## Valgrind Checks
- valgrind --track-origins=yes --leak-check=yes --leak-resolution=low --show-leak-kinds=definite ./BbAdventures 2>&1 | tee memcheck.txt