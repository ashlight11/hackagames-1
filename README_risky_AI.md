# This is part of MAD (Models and Algorithms for Decision) course at IMT Nord Europe 

## Code by Marianne De Poorter and Jules Descotes

This project is based on the Hackagames library. It is designed toimplemented Q-learning on the strategy game Risky.

## Q-values

We compute the Q-value for each game state, and for each meta-actions as defined below.
The resulting values are stored in the file : `trained_qvalues.json`.
After each training, we use the already trained Q-values and we modify it as we learn new situations. 

## Approach for meta-actions

To reduce the number of actions possible, we decided to group scenarios in 5 categories :
- Expand : moving soldiers from one node to an other that was not occupied before, expanding the territory.
- Attack : moving soldiers to a node owned by our opponent, knowing we have chances to win (forces > opponent forces + 2)
- Defend : moving soldiers between nodes that we own, strengthening our positions
- Grow : basic action, adding more soldiers to a node we own
- Sleep : basic action, resets all active nodes to ready to move

## Curse of dimensionality 

With these reductions, the size of the search space is still too large, and we see that we are not learning fast. In fact, it would require a lot of training to make the average best Q-value go up. 

