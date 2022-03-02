# Project Hackagames

This document lists the project components in a similar way than a _backlog-product_ from the [*SCRUM*](https://fr.wikipedia.org/wiki/Scrum_(d%C3%A9veloppement)#Glossaire) terminology.

**HackaGames** aims to become a _KISS_ (keep it stupid simple) Game-engine where games are reachable by the player throught the network. This way, players can be developed in any language while the player program respect the communication protocol.

The development is organized over several functionalities listed here from the most mandatory to the less important:
Each Functionnality groups several components to develop as UserStories under the responsibility of one or two main developers.

## Fct.0 - Project Framing.

This functionality cover a framework setup for the developers.

- A developer can share a working directory (git, gitlab, ...)
- Developers can communicate with others (mailing list, discord, ...)
- Hello World’s programmes can be completed and executed.
- Everyone can refer to the documentation.
- A realize by merging contributions can be generated.
- A solution for Unit Test is operational.
- All developers can refer to a good-practice doc for code.
- Developers would use a proper C unit-test framework.


## Fct-1: Game API

**HackaGames** is first a KIS game API to develop turn based games.
A **HackaGames** game included mainly a tabletop composed of interconnected cells where pieces can be positionned. 
However, tabletop, cells and pieces would refer to the main API strucuture: an **Organism**.

- A Tabletop is composed of cells at specific position (x,y). **DONE**
- Cells are connected to other with edges. **DONE**
- Cells can contain Pieces. **DONE**
- It is possible to generate a tabletop randomly (example: random cells and Gabriel graph).
- An algorithm provides paths in the tabletop between two positions (A*).
- Long edges can be easely subdivided with intermediate cells in order to generate quite regular game tabletop.
- A Piece is on a specific cell.
- Pieces see the different possible movements (edges from the nodes) and can try one. The movement succeed only if the node is not over populated.
- Pieces can act on the NetWorld (change the colour of a node for instance).
- Pieces can act over other entities (damaging them for instance).
- **Tanks**: Digit represent a discrete value in a fixed capacity tank (min, max, number of subdivition)
- **Cards** a set of tanks.

## Fct-2: Networks Interface

Games developped with **HackaGames** run as a server on the machin that the AIs or player interfaces will conect.

The goal here is to distribute the game process.

- A player owns entities.
- The player process can be executed on an independent computer process (network)
- The NetWorld simulation can be distributed over processes.
- The game is reachable from a web interface.

## Fct-3: System tools

- Games command better handle command options.

## Fct-4: Viewer

This functionality focus on a graphical rendering of a **HackaGames** game and to provide a control through the mouse and keyboard.
It is based on _RayLib_ frontend.

- The program starts on welcome menu
- It is possible to launch and visualize a NetWorld and its Entities
- Players can save and load a game 
- Players can select its own entities and provide them for an expected location.
- Animations are triggered when an entity move from a node to another.


## H-Game: Risky

## H-Game: 421


## H-Game: morpion

Morpion games (m,n,k-game) a basic logic tow player position game


## H-Game: fluxctrl

## H-Game: squad

Squad, an HackaGames Game.

Squad is a strategic turn-based game where two squad fights until death.

Fighter Attribute: 

- Vitality
  + action
  + range
  + charge
- Fight
  + attack
  + esquiv
  + critik
- Brutality
  + degas
  + armor
  + shield
- Strategic
  + speed
  + initiative
  + energy

## H-Game: navgraph


