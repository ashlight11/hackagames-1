# BOARD of the project

- Move to `HackaGames` with `hackagames-geometry`, `hackagames-tabletop` and `hackagames-server` as the 3 component of HackaGames API
- Add test procedures
- `hackagame-server` based on [mosquitto](https://mosquitto.org/) or [zeromq](http://czmq.zeromq.org/) ?
   * `hg-draft` - `hg-talk` - `hg-play`.
- `hackagame-server` based on [mosquitto](https://mosquitto.org/)
- Clean **RISKY** game as first example
- Raylib basic player
- jouer avec `screen` et voir la concurance.
- Wraps python
- Wrap Pharo

### Tabletop:

Replace Tabletop and Piece with Organism composed by Cells. Everything is Cells
aCell:

- **name**: a name (if necessary)
- **possition**: floating point 2 dimention variable (geometry.vector2)
- **shape**: a 2 dimention polygone (geometry.polygon)
- **color**: integer value
- **attrs**: integer attributs
- **cells**: neighbour cells
- **links**: list of cells composing the parent Cell.

Balance on game structure some of the function control over piece (ie cells of cells)

### Dynamic Organism:

The idea is to permit to ad or remove more easelly Cells

- **links -> linkTo linkFrom**: define links as a simple int array (i.e. each Organism knows its neirborhou)
- **cardinality** Proper cardinality int variable;
- **removing** clean removing of cells (i.e. including links...)


### An augmented human interface:

Implement a standard svg output.
Implement raylib-viewer as a separated modul, and integrate it on a client node.

- Interface to a game not an organism
- Intermediate telnet for instance (2014 -> 2015). Activable or not.

### Client/Server solution:

A server game with client player based on a more optimized communication (see client/server libs).

- Lib client/serveur:
	* https://zeromq.org/ (sans broker i.e. roscore)
	* mqtt plus simple que ROS. https://github.com/LiamBindle/MQTT-C avec https://mosquitto.org/

A simple human interface in C in place of telnet. 

### Game - Project  M.A.X. - Machines for Autonomous eXpand

- Based on Risky, but with more elaborated armies.
- 162 (11*12+30) cells' planets based on Decahedron.
- Ressources and evolution.
- War Fog.
- ...

### Vrac:

- Decompose tabletop file (and remove edge by keeping every thing in Nodes renamed Cells with paths)
- Implement svg drawing, and integrate it on a client node.
- Allows observators node.
- Tests in every directions...
- Developper chart...
- draw miniature from .png
- differencier str, structured-str, (et via des str-tools dans tools.h)
- send Player Id over nbPlayer to player.
- (with class) - binding C++
- binding Python, Pharo...
- Cells: values ?
- Cells: sprites.
- More interactive viewer ? maybe in specific games ?
- Handle standard arguments: argp ?

## Games to impléments:

- Toc (but card ?)
- navgraph
- WarBot (rp-tbs game)
- OpenDrive: Course de véhicule en environnement ouvert et dynamique. (Avec des aspects de dynamique des véhicules quasiment absents, mais la nécessité de prendre en compte aux mieux les autres véhicules pour optimiser sont choix de chemins).

