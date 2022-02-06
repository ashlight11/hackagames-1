# Play with mosquitto MQTT

The idea is to use cleint/server architecture to permt any code in any language to seat to a games (ie. connect to act as a player).
For that we choose to use a hight level messaging librairie and esitate between [mosquitto](https://mosquitto.org/) and [zeromq](http://czmq.zeromq.org/).

- **Mosquitto** is a MQTT standar light implementation. MQTT defines protocols for topic based process interconnection. a Broker server serves as a central point to connect all the other node.
- **zeromq** follows its own protocols and propose a high level interface to interprocess communication via difference communication architecture and diferent low level protocols including in-procecus communication.
