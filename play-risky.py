#!env python3
import os, time

gameDir= os.path.dirname( os.path.realpath(__file__) ) + "/game-risky"

# Start the server:
os.system( gameDir+"/hg-risky > hg-risky.log &" )
time.sleep(0.5)

# Create an oponent:
os.system( "python3 "+ gameDir +"/simplePlayer.py > oponent.log &" )

# Start the simplest interfast possible:
os.system( "telnet localhost 14001" )
