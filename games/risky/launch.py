#!env python3
import os, time, random
import pyrisky.client as risky
import simplePlayer as player

# Start the server:
os.system( "./hg-risky > risky.log &" )
time.sleep(0.5)

# create an oponent:
os.system( "python3 simplePlayer.py > oponent.log &" )

# create an oponent:
player.takeASeat()
