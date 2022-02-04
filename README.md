# HackaGame - An Hackable Game Engine

**HackaGame** aims to be a *C* game engine based on multi-media library *RayLib* and MQTT library *Mosquito*, accordingly to [KISS](https://fr.wikipedia.org/wiki/Principe_KISS)  (Keep It Stupid Simple) principle.
The main feature of this project is to model the world as a tabletop seen as a network of key positions (Nodes) on with 
interacting entities (Pieces) can be moved.
Players would be responsible for Pieces’ behaviors and can be implemented in independent programs in any language of your choice.
In other terms, **HackaGame** implement a simple client/server architecture to permit player AI, developped on any language, to take a seat on a game.

**HackaGame** is seens as an API for game development.
Several games are proposed with the API for the example:

- **RISKY**: A turn-based strategic game where players control armies fighting for a territory.
- **HacKRPG**:

## Concurency:

**HackaGame** is not wath you looking for ? Try those solutions:

- https://ludii.games/ "a general game system designed to play, evaluate and design a wide range of games" (JAVA)
- https://www.pommerman.com/ an hackable Bomberman game (Python)

## Licence

**HackaGame** is distributed under the [MIT license](./LICENCE.md).

## Installation (Linux)

**HackaGame** work natively on Linux systems and commands is given regarding Ubuntu-like distribution.

### Dependencies:

C/C++ development tools, CMake, Test, [RayLib dependencies](https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux) and ZeroMQ libs

```bash
sudo apt install -y \
  build-essential git cmake \
  libasound2-dev mesa-common-dev \
  libx11-dev libxi-dev xorg-dev \
  libgl1-mesa-dev libglu1-mesa-dev \
  libzmq3-dev libczmq-dev
```

### **HackaGame**

Clone then build the overall project.

```bash
git clone git@bitbucket.org:imt-mobisyst/hackagames.git
cd **HackaGame**
./bin/build.sh
```

## Getting started

The easiest way is to enter in one of the example games as [risky](./games/risky).

## In this repository

Directories:

- *bin* : stock the scripts for project management.
- *.git* : git version management repository.
- *dpd* : included dependencies (RayLib and potentionnaly in the future: Wanda / Igraph... )
- *src* : source code of the core library of the project (C language).
- *doc* : documentation du projet (to be generated).
- *games* : game examples on the top of **HackaGame** API.

Root Files:

- *README.md* : Your servitor.
- *LICENCE.md* : The Applied MIT license.
- *CMakefile* : Instructions for `CMake` construction

## Contribution guidelines

* Writing tests
* Code review
* Other guidelines
* [Learn Markdown](https://bitbucket.org/tutorials/markdowndemo)

### Contributors

- Permanent contributor:
  * **Guillaume LOZENGUEZ**
- 1st version of Risky game: **Ewen MADEC** and **Timothy LAIRD** (April, 2021)
