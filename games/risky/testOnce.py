#!env python3
import os, time, random
import pyrisky.client as risky
from simplePlayer import Player

# Start the server:
os.system( "./risky-easy 30 1 123 > risky.log &" )
time.sleep(0.5)

# create an oponent:
os.system( "python3 risky/simplePlayer.py 12> oponent.log &" )

# create an oponent:
risky.takeASeat( 'localhost', 2014, Player() )
