# PathOfLegends
Simulator for the Path of Legends

## Guiding Questions: 

- If 1% of players reach Ultimate Champ every season, how many games are played?
- If we remove *all* golden steps, how many steps are needed to ensure 1% of players reaching UC after *x* games?

## Roadmap:

### Experiments
- Find out how many battles are played to get a 1% UC
- Solved. Roughly 62 million. 
- Experiment with shortening the path and cutting down golden stps
	- 5-10 trials of 3-4 settings sims, probably on 10% scale: 
	- Path Length: 60,  no gold steps in leagues
	- Path Length: 50, dropping leagues is allowed (Set max multiplier to 4 or 5.)
	-  Path length 30: No golden steps, (Multiplier 3-4)
- Useful Data Points: 
	- wins for UC players
	- W/L ratio (overall, cannot adhere to master 1+ counts rule with league dropping allowed)
	- count of UC players. 

Command to run experiments from a path configuration `pathSettings.txt`in samplePath folder is
`sh experiment.sh pathSettings`. 

Script will: 

- Conduct Experiment (Does 2 lines of output every season for 5 seasons and 'Trial n done' at the end of the trial. Runs 10 trials and puts raw data and config files in `data/rawdata`
- Conduct an analysis of the data and output the analysis results (avgs and stdevs) in a file in `data/summary`
- Creates plots of the data and saves to `graphs` folder
- Opens the graph

## Contents of this Repo: 

`Player.*pp`: Interface and implementation for the player class.  Mostly just storing data about the player. 

`hashQueue.*pp`: Interface and implementation for Hashqueue class. The hash queue is a "hash table" but the hash function is the identity function. The table itself is an array that stores the indices of the players in the mm queue. Constant time to find an opponent.  *This is a significant speed up for a PoL simulatior over the regular ladder simulator which uses a random BST*.

Stores an array of players in queue, a vector of player objects. The interface is just `findOpponent()`, taking in an index in the player vector. If there is an opponent, it returns it. If not, it returns the number of players (a known number in the simulation) and adds the player to the queue. 

`simulation.*pp` Interface and Implementation of the simulation. Constructor reads in a file with the parameters for the simulation. 

### Reading in a Settings file:

The settings file _MUST_ contain 9 pairs of numbers: The minimum number of steps required to reach a league and the frequency of the golden step, separated by a whitespace. `10 3` on the second line means "The second league begins at step 10 and has a golden step every 3 wins". Zero in the steps column means that there are no golden steps.  The number at the bottom is the end of the last league, in the current season, this is 93 steps. The penuultimate number is 1 or 0, if dropping leagues is allowed (if the golden step at the begining of each league will be turned on). Last number is the maximum multiplier. Partiulaly useful with investigating shorter paths.  

`settings.txt` and `shortpath.txt` are testing files for paths. `shortpath` has 35 steps and lots of golden steps. `settings.txt` is the current path. 

### Final notes

There are overloaded `<<` operators for player and hashqueue classes for debugging. Determining who wins a match is definitely my biggest assumption. Unfortunately there is near zero api data about the path of legends so its really hard to figure out who wins. I've tried multiple linear regression and found nothing, best I can do is something with legacy PBs. 

The performance goal is 1 million battles per second played with 500k players (I think this is about 10% of what the actual CR playerbase is since it was ~7.4 million in January 2022, but has been shrinking). Running the simulator on my machine with 500k players results in about 2.1 million battles per second. Memory usage was around 25KB, which is very reasonable. 