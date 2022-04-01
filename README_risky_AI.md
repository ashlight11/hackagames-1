# This is part of MAD (Models and Algorithms for Decision) course at IMT Nord Europe 

## Code by Marianne De Poorter and Jules Descotes

This project is based on the Hackagames library. It is designed to implement Q-learning on the strategy game Risky.

## Q-values

We compute the Q-value for each game state, and for each meta-actions as defined below.
The resulting values are stored in the file : `trained_qvalues.json`.
After each training, we use the already trained Q-values and we modify it as we learn new situations. 

Initially, we put every state of the tabletop inside the Q-values dictionary. Then, we decided to try another approach, see section **State Reduction**.

## Approach for meta-actions

To reduce the number of actions possible, we decided to group scenarios in 5 categories :
- *Expand* : moving soldiers from one node to an other that was not occupied before, expanding the territory.
- *Attack* : moving soldiers to a node owned by our opponent, knowing we have chances to win (forces > opponent forces + 2)
- *Defend* : moving soldiers between nodes that we own, strengthening our positions
- *Grow* : basic action, adding more soldiers to a node we own
- *Sleep* : basic action, resets all active nodes to ready to move

## Curse of dimensionality 

With these reductions, the size of the search space is still too large, and we see that we are not learning fast. In fact, it would require a lot of training to make the average best Q-value go up. 

## State reduction : basic approach

To reduce the number of states explorable, we decided to describe the states according to strategic heuristics : 
- *win* : the situation leads to a win
- *defeat* : the situation leads to a defeat
- *slight disadvantage* : the situation is not favorable, but not lost
- *slight advantage* : the situation is favorable, but not much
- *large disadvantage* : the situation is critically not favorable
- *large advantage* : the situation is largely favorable

This leads to very very few possibilities (probably not enough), and thus, training is faster and we can obtain better results. 

The states are classified with scikit-learn Decision Tree Classifier and were trained with data generated with the initial trial on Q-Learning. The datasets were created as follows : 
- `wins.txt` : states when the result was a win (after game)
- `defeats.txt` : states when the result was a defeat (after game)
- `slight_disadvantages.txt` : states when the difference between our score and the opponent's was between -50 and 0 (in game)
- `slight_advantages.txt` : states when the difference between our score and the opponent's was between 0 and + 50 (in game)
- `large_disadvantages.txt` : states when the difference between our score and the opponent's was < -50 (in game)
- `large_advantages.txt` : states when the difference between our score and the opponent's was > 50 (in game)

## Structure of our Python file 

There are two classes in the Python file. 
- `Player` is the basic Q-learning approach (will generate `trained_qvalues.json` if not already present). It also generates the files mention in the section above. 
- `Player_Classified` is the approach with state reduction (will generate `trained_qvalues_bis.json` if not already present)

If you wish to change the approach, please modify the class instianted in the `main` function.
Please try Player first if you intend on trying Pkayer_Classified (necessary to generate sample files).

*Note : the results with the classification of states is disappointing. More states should be considered as this is a strategy game. Please understand that we tried to showcase available options and functionalities.*