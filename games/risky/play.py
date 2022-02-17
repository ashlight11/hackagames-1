#!env python3
import os, time, random
import pyrisky.client as risky

# Start the server:
os.system( "./risky > risky.log &" )
time.sleep(0.5)

# Create an oponent:
os.system( "python3 simplePlayer.py > oponent.log &" )

# Start the simplest interfast possible:
os.system( "telnet localhost 2014" )
