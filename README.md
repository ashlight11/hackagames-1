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
