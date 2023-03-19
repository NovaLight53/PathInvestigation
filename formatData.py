# Simple script to change the format of the settings and analysis files
# into markdown table format to make the writeup easier. 

import argparse

parser = argparse.ArgumentParser(description="Convert Analysis data to Markdown Table.")
parser.add_argument("filename", help="file to convert" )
parser.add_argument("filetype" ,  help="path settings; p, anything else: analysis", default=False)

args = parser.parse_args()
filename = args.filename
if (args.filetype == "p"):
    s = "| League | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | UC | \n| --- | --- |  --- | --- | --- | ---- | --- | --- | --- | --- | --- |"
    print(s)
    stepReqs = "| Step Requirement | "
    goldStepRules = "| Gold Step rule | "
    dropLeagues = "Drop Leagues: "

    with open(filename, "r") as f:
        for line in range(9):
            step, rule = f.readline().split()
            step += " | "
            rule += " | "
            stepReqs += step
            goldStepRules += rule
        
        ucReq = f.readline().rstrip()
        stepReqs += ucReq + " |"
        goldStepRules += " N/A |"
        print(stepReqs)
        print(goldStepRules)
        drop = f.readline()
        if drop == "1\n":
            print("Drop Leagues: True")
        else: 
            print("Drop Leagues: False")
        maxMult = f.readline()
        print()
        print(f"Max Multiplier: {maxMult} ")  
else:
    print("| Statistic | Average | St Dev. |")
    print("| ------| --- | --- |")
    with open(filename, "r") as f:
        f.readline()
        fs1, fs2, avg, stdev = f.readline().split()
        print(f"| Finishing Step | {avg} | {stdev} |")
        data = f.readline().split()
        print(f"| UC Games Played | {data[-2]} | {data[-1]} |")
        data = f.readline().split()
        print(f"| UC Win Percent | {data[-2]} | {data[-1]} |")
        data = f.readline().split()
        print(f"Percent UC:  {100*float(data[-1])}")
    



    





