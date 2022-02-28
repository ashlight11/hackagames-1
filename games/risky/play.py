#!env python3
import os, time

# Start the server:
os.system( "./hg-risky > hg-risky.log &" )
time.sleep(0.5)

# Create an oponent:
os.system( "python3 simplePlayer.py > oponent.log &" )

# Start the simplest interfast possible:
os.system( "telnet localhost 2014" )
