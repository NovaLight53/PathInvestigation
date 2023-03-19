#include <fstream>
#include <iostream>
#include <cmath>
#include <numeric>
#include <string>
#include <vector>

using namespace std;

template <typename T>
float average(std::vector<T> const &v)
{
    if(v.empty()){
        return 0;
    }

    auto const count = static_cast<float>(v.size());
    return std::reduce(v.begin(), v.end()) / count;
}

template <typename T>
double variance(std::vector<T> const &v, float avg){
    double var = 0.0;
    if (v.size() == 0){
        return 0.0;
    }
    for (auto& value : v){
        var += pow((value - avg), 2);
    }
    return var / (v.size() - 1);
}

/**
 * @brief Take in the data files and give:
 * Avg and SD of step
 * Avg and SD of games Played to reach UC
 * Avg and SD of win pct of UC players
 * Avg UC pct
 */
int main(int argc, char** argv){
    vector<int> avgSteps;
    vector<int> avgGamesPlayed;
    vector<float> avgWinPct;

    vector<int> varSteps;
    vector<int> varGamesPlayed;
    vector<float> varWinPct;

    vector<float> ucPcts;
    string filenames[10];

    if (argc < 2){
        cerr << "Provide file name base" << endl;
        exit(1);
    }

    string filebase = argv[1];
    for (size_t seed = 0; seed < 10; ++seed){
        filenames[seed] = filebase + "." + to_string(seed) + ".results";
    }

    for (string filename : filenames){
        // id wins losses league step
        ifstream datafile{filename};
        vector<int> gamesPlayedVec; //wins for all UCs
        vector<float> winPctVec; // win pcts for all ucs
        vector<int> steps; // steps for everyone
        
        for (size_t playerId = 0; playerId < 500000; ++playerId)
        {
            int id, wins, losses, league, step;
            datafile >> id;
            datafile >> wins;
            datafile >> losses;
            datafile >> league;
            datafile >> step;
            if (league == 9){
                gamesPlayedVec.push_back(wins + losses);
                winPctVec.push_back(float(wins) / (wins + losses));
            }
            steps.push_back(step);

        }
        avgGamesPlayed.push_back(average(gamesPlayedVec));
        avgSteps.push_back(average(steps));
        avgWinPct.push_back(average(winPctVec));

 
        varWinPct.push_back(variance(winPctVec, avgWinPct.back()));
        varSteps.push_back(variance(steps, avgSteps.back()));
        varGamesPlayed.push_back(variance(gamesPlayedVec, avgGamesPlayed.back()));

        ucPcts.push_back(gamesPlayedVec.size() / 500000.0);
    }

    float winPct = average(avgWinPct);
    float gamesPlayed = average(avgGamesPlayed);
    float step = average(avgSteps);

    float winPctVar = sqrt(average(varWinPct));
    float gamesPlayedVar = sqrt(average(varGamesPlayed));
    float stepVar = sqrt(average(varSteps));

    float ucPct = average(ucPcts);

    cout << "Averages and Stdevs" << endl;
    cout << "Finishing Step: " << step << "\t" << stepVar << endl;
    cout << "UC Games Played: " << gamesPlayed << "\t" << gamesPlayedVar << endl;
    cout << "UC Win Percent: " << winPct << "\t" << winPctVar << endl;
    cout << "UC Percentage: " << ucPct << endl;

    return 0;
}