# Risky, an HackaGames Game

Risky is a strategic turn-based game where two army fights for a territory.

## Installation

**Risky** is part of **HackaGames** project. Please refert to **HackaGames** intallation procedure.

Then **Risky** can be compiled with `cmake` and `make`:

```sh
cmake .
make
```

## Try the game:

**HackaGames** game work as a server and the player has to connect to play the games.

### Simple start

The `play-risky.py` script launch a risky game server with a simple AI and and allow a human player to play as the second player with `telnet` program.

In a terminal:

```sh
./play.py
```

Each player can perform one and only one action at it turns.

- moving: `move X Y STRENGH` to move `STRENGH` units from cell `X` to cell `Y`
- growing: `grow X` to grow the army on nodes `X`
- sleeping: `sleep` that reset the action counter to $0$ for all the armies (each army need to sleep between 2 move or grow actions).

### Game rules

At the begining of the game, each player receives a description of the tabletop, the cells and the edges connection.
The edges model the posible movements.

At its turn, each player gets a state of the game:
- player: the player configuration (its player id (1 or 2) the number of players (2) the current scores for player-1 and player-2 )
- Game: the number of turns before the end of the game and the number of pieces (i.e. armies) on the tabletop.
- Pieces: the list of pieces on the tabletop. The position cell, the player owner, a type (always soldier here) a list of 2 attributes (strength and number of performed actions).
After what the game would expect a decision.

Each player can perform one and only one action at it turns.

- moving: `move X Y STRENGH` to move `STRENGH` units from nodes `X` to node `Y`
- growing: `grow X` to grow the army on nodes `X`
- sleeping: `sleep` that reset the action counter to $0$ for all the miniatures.

To notice that:

- A piece can perform only one action between to sleep.
- A wrong action request would end on a sleep action.
- A moving action on an occupied cell would merge or fight the targeted cell depending on the owner.
- A fight is always to the death of one of the two pieces. 


### Manual start

First start `risky` server in a first terminal: 

```sh
./hg-risky
```

Then 2 players as to reach the games on port 14001, so in two different terminals:

For human player:

```sh
telnet localhost 2014
```

For AI player (in python in the example):

```sh
python3 simplePlayer.py
```

## Your first AI:

The file `simplePlayer.py` propose a first random AI with the required structure to play `risky`.
So copy this player and start to implement simple ideas...

```bash
cp simplePlayer.py myBeatifullAI.py
```

You can try your *AI* by modifying the `play.py` or `confront.py` scripts:

- `play.py` start a player versus AI game, the output of your AI would be written in `opoenent.log` file.
- `confront.py` permit tow AIs to play a several games to draw statistics.
