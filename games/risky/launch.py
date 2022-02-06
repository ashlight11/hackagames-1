#!env python3
import os, time, random
import pyrisky.client as risky
from pyrisky.player import Player

# Start the server:
os.system( "./risky > risky.log &" )
time.sleep(0.5)

# create an oponent:
os.system( "python3 simplePlayer.py > oponent.log &" )

# create an oponent:
os.system( "telnet localhost 2014" )
