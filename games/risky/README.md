# Risky, a Toghap Game

Risky is a strategic turn-based game where two army fights for a territory.

## Installation

We assume that Toghap is correctly compiled, and that the command `risky-easy` was generated.

cf. [Toghap README file](../README.fr)

Well, there is nothing to do here...


## Try the game:

Togap game work as a server and the player has to connect to play the games.

For risky in a first terminal, start the server: 

```bash
cd  game-risky
./risky-easy
```

Then 2 players as to reach the games on port 2014, so in two different terminals:

```bash
telnet localhost 2014
```

Then the game begin.
Each player will receive a description of the world tabletop the nodes (or cells) and the possible movement modeled as edges.

At its turn each player gets a state of the game:
- player: the player configuration (its player id (1 or 2) the number of players (2) the current scores for player-1 and player-2 )
- Game: the number of turns before the end of the game and the number of miniatures (i.e. army) on the tabletop.
- Miniature: one at a time of the list of miniatures. The position node, the player owner, a type (always humans here) a list of 2 attributes (number of performed actions and strength).
After what the game would expect a decision.

Each player can perform one and only one action at it turns.

- moving: `move X Y STRENGH` to move `STRENGH` units from nodes `X` to node `Y`
- growing: `grow X` to grow the army on nodes `X`
- sleeping: `sleep` that reset the action counter to $0$ for all the miniatures.

To notice that:

- A miniature can perform only one action between to sleep.
- A wrong action request would end on a sleep action.
- A moving action on an occupied node would merge or fight the targeted node depending on the owner.
- A fight is always to the death of one of the two armies. 

## Your first AI:

The file `player.py` propose a sleeper AI with the required structure to play `risky`.
So copy this player and start to implement simple ideas...

```bash
cp pyrisky/player.py myAutoPlayer01.py
```

You can try your `player` by modifying the `launch.py` script:

replace:

```python
from pyrisky.player import Player
```

by the appropriate instruction, for instance:

```python
from myAutoPlayer01.py import myAutoPlayer as Player
```

And that it's, you can fight your AI, in a terminal:

```bash
python ./launch.py
```

To notice that the output of your auto-player would be written in `opoenent.log` file.


