#include <iostream>
#include "simulation.hpp"
#include <thread>
using namespace std;

#define NUM_BATTLES 62747762
#define NUM_PLAYERS 500000
#define ULT_CHAMP_PCT 0.01f // 0.5-1% is reasonable UC pct

// Test command:  ./pathSim -p small.txt -o test -i settings.txt -d 70

void printCompilerCommands();
// void runSim(string &settings, string playerfile, size_t seed, string outputname);

int main(int argc, const char **argv){
    string playerFile = "small.txt";
    string settingsFile = "settings";
    string outputPrefix = "simulation";
    size_t seed = 70;
    size_t maxThreads = 1;

    // Process command line arguments. 
    vector<string> args(argv + 1, argv + argc);
    for (size_t i = 0; i < args.size(); ++i){
        string arg = args[i];
        if (arg == "-h"){
            printCompilerCommands();
            exit(0);
        } else if (arg == "-o"){
            ++i;
            outputPrefix = args[i];
        } else if (arg == "-i"){
            ++i;
            settingsFile = args[i];
        } else if (arg == "-p"){
            ++i;
            playerFile = args[i];
        }else if (arg == "-d"){
            ++i;
            seed = stoi(args[i]);
        }
        else if (arg == "-m") {
            ++i;
            maxThreads = min(stoi(args[i]), 8); // max of 8 threads used
        }
    }
    // vector<thread> simThreads;
    // cout << "max threads:  " << maxThreads << endl;
    // for (size_t sim = 0; sim < maxThreads; ++sim) {
    //     cout << "sim " << sim << endl;
    //     string outputFile = outputPrefix + "." + to_string(sim);
    //     std::thread t(runSim, ref(settingsFile), playerFile, sim, outputFile);
        
    //     simThreads.push_back(std::move(t));
    // }
    // for (thread& t: simThreads){
    //     t.join();
    // }
    
    Simulation pathSim{settingsFile, playerFile, seed, NUM_PLAYERS, outputPrefix};
    float result = pathSim.nBattlesSimulation(NUM_BATTLES);
    // size_t result = pathSim.ucPctSimulation(ULT_CHAMP_PCT);
    cout << result << endl;  
    return 0;
}

// void runSim(string& settings, string playerfile, size_t seed, string outputname ){
//     Simulation s{settings, playerfile, seed, NUM_PLAYERS, outputname};
//     s.ucPctSimulation(ULT_CHAMP_PCT);
//     return;
// }

void printCompilerCommands(){       
    cout << "Compiler Flags: \n" << endl;
    cout << "-h: Prints this help message" << endl;
    cout << "-o OUTPUT-PREFIX: The prefix of the results file. " << endl;
    cout << "   Ex: -o sim10k => sim10k.results" << endl;
    cout << "   Default: simulation.results" << endl;
    cout << "-i INPUT-PREFIX: The settings input file" << endl;
    cout << "   Ex: -i currentSettings.txt => readsfrom file \"currentSettings.txt\" " << endl;
    cout << "   Default: \"settings.txt\" (included in original repo)" << endl;
    cout << "-p FILENAME: The file where initial player data comes from" << endl;
    cout << "   Ex: -p 25kPlayers.txt => players come from file \"25kPlayers.txt\"" << endl;
    cout << "   Default: /ladderSample/25k.txt" << endl;
    cout << "-d SEED: Seed for RNG." << endl;
    cout << "    Default: 70" << endl;
    cout << "-m MAX_THREADS: Maximum number of threads for multithreading." << endl;
    cout << "   Default: 1, no multithreading. Max threads allowed is 8. Each thread will have a different seed" << endl;
}
