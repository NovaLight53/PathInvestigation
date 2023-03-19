import pandas as pd
import matplotlib
from matplotlib import pyplot as plt
import seaborn as sns
import argparse
import regex as re

parser = argparse.ArgumentParser(description="Graph Simulation Data")
parser.add_argument("data", help = "Path to Data files")

args = parser.parse_args()
filebase = args.data
fnames = [filebase +"."+ str(trial) + ".results" for trial in range(10)]
filesL = [open(fname, "r") for fname in fnames]
allWinPcts, UCwinPcts, allLeagues, allsteps, UCGamesPlayed = [],[], [], [], []
for numPlayers in range(500000):
    wpAll, wpUC, leagues, steps, gpUC = [],[], [], [], []
    for f in filesL:
        line = f.readline()
        id, wins, losses, league, step = line.split()
        wins = int(wins)
        losses =int(losses)
        league = int(league)
        step = int(step)
        if league == 9:
            gpUC.append(wins + losses)
            wpUC.append(wins/(wins + losses))
        wpAll.append(wins/(wins + losses))
        leagues.append(league)
        steps.append(step)
    allWinPcts.append(sum(wpAll)/10)
    allLeagues.append(round(sum(leagues)/10))
    allsteps.append(sum(steps)/10)
    if len(gpUC) > 2: # if player reaches UC 3/10 times
        UCwinPcts.append(sum(wpUC)/len(gpUC))
        UCGamesPlayed.append(sum(gpUC)/len(gpUC))

toAllDf = []
for wp, l, s in zip(allWinPcts, allLeagues, allsteps):
    toAllDf.append([wp, l, s])

df = pd.DataFrame(toAllDf,columns=["Win Pcts",  "Leagues", "Steps"])
fig, axes = plt.subplots(nrows = 2, ncols = 2, figsize=(10,9))

histColors = matplotlib.cm.get_cmap('viridis', 9)

axes[0,0].set_title("Win Percents")
axes[0,1].set_title("Finish Steps")
axes[1,0].set_title("Games to UC")
axes[1,1].set_title("Win Pct to UC")
sns.histplot(ax = axes[0,0], data = df, x = "Win Pcts", hue="Leagues", palette = histColors )
sns.histplot(ax = axes[0,1], data=df, x="Steps", hue = "Leagues", discrete=True, palette=histColors)
sns.histplot(ax=axes[1,0], data=UCGamesPlayed, discrete=True, color="skyblue")
sns.histplot(ax=axes[1,1], data = UCwinPcts, color = "mediumturquoise")
outputFile = re.sub("data/rawdata", "graphs", filebase)
outputFile = re.sub("txt", "png", outputFile)

plt.tight_layout()
plt.savefig(outputFile)
