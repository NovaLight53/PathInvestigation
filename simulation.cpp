/**
 * @file simulation.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-11
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "simulation.hpp"
using namespace std;

Simulation::Simulation(string settings, string playerFile, size_t seed, size_t numPlayers, string outputPrefix){
    numPlayers_ = numPlayers;
    seed_ = seed;
    unsigned kt, cl;
    short oldPB;
    float partyPct;
    // Must treat as unsigned ints to properly read in numbers
    unsigned toGoldStep;
    unsigned stepReq;
    bool dropLeague;
    unsigned mult;
    ifstream infile(settings);
    ifstream playersIn(playerFile);
    ofstream configOut(outputPrefix + ".cfg");
    for (size_t stepRule = 0; stepRule < 9; ++stepRule) {
        //steps req, gold step, lvl cap
        infile >> stepReq;
        stepRequirements_[stepRule] = stepReq;
        infile >> toGoldStep;
        if (toGoldStep == 0){
            toGoldStep = 255;
        }
        goldStepRules_[stepRule] = toGoldStep;
    }
    size_t id = 0;
    for (size_t id = 0; id < numPlayers_; ++id) {
        playersIn >> kt;
        playersIn >> oldPB;
        playersIn >> partyPct;
        playersIn >> cl;
        players_.push_back(new Player{id, partyPct, oldPB, u_int8_t(kt), u_int8_t(cl), goldStepRules_[0]});
    }
    // Need to get UC step requirement.
    infile >> stepReq;
    stepRequirements_[9] = stepReq;
    // cout << "UC requirement: " << int(stepRequirements_[9]) << endl;
    infile >> dropLeague;
    infile >> mult;
    goldStepRules_[9] = 255;
    queue_ = new HashQueue(players_);
    rng_ = mt19937(seed_);
    outStream_ = ofstream(outputPrefix+".results");
    maxMultiplier_ = mult;
    configOut << "Seed: " << seed_ << "\n"
              << "Player file: " << playerFile << "\n"
              << "Number of Players: " << numPlayers_ << "\n"
              << "Steps Per Golden Step: ";
    for (size_t league = 0; league < 9; ++league){
        configOut << int(goldStepRules_[league]) << " ";
    }
    configOut << "\nSteps required for each league: ";
    for (size_t league = 0; league < 9; ++league){
        configOut << int(stepRequirements_[league]) << " ";
    }
    configOut << "\nSteps for Ultimate Champ: " << int(stepRequirements_[9]) << endl;
    configOut << "Dropping Leagues: " << dropLeague << endl;
    configOut << "Max multiplier: " << int(maxMultiplier_) << endl;
}

Simulation::~Simulation()
{
    delete queue_;
    for (Player* p : players_){
        delete p;
    }
}

float Simulation::nBattlesSimulation(size_t numBattles){ 
    uniform_real_distribution<double> doubleDist(0, 1);
    uniform_int_distribution<size_t> playerDist(0, numPlayers_ - 1);
    // Running 4 seasons to get effect of multiplier
    for (size_t season = 0; season < 5; ++season)
    {
        seasonReset();
        ultChamps_ = 0;
        size_t battlesPlayed = 0;
        auto seasonStart = chrono::high_resolution_clock::now();
        while (battlesPlayed < numBattles)
        {
            playBattle(doubleDist, playerDist, battlesPlayed);
        }
        auto curTime = chrono::high_resolution_clock::now();
        std::cout << "Season " << season << " complete with " << ultChamps_ << " Ultimate Champs"
                  << "\n";
        chrono::duration<double> timeSoFar = curTime - seasonStart;
        std::cout << "Avg Battles/sec: " << battlesPlayed / timeSoFar.count() << "\n"
                  << endl;
        }
    // Print players after 5 seasons
    printAllPlayers();
    return ultChamps_ / float(numPlayers_);
}

void  Simulation::playBattle(uniform_real_distribution<double>& doubleDist, 
                    uniform_int_distribution<size_t>& playerDist, size_t& battlesPlayed){
        size_t p1 = playerDist(rng_);
        size_t opponent = queue_->findOpponent(p1);
        // play battle
        if (opponent != numPlayers_ and doubleDist(rng_) > players_[p1]->getPartyPct()) {
            double randomVal = doubleDist(rng_);
            u_int8_t league = players_[p1]->getLeague();
            u_int8_t curToGold = 0;
            bool newUC = false;
            players_[p1]->playMatch(*players_[opponent], randomVal, goldStepRules_,
                                    stepRequirements_, dropLeague_, newUC);
            ultChamps_ += newUC;
            ++battlesPlayed;
            // cout << *players_[p1] << " vs " << *players_[opponent] << endl;
            // Infinite loop case. full queue, all others in UC. Very rare
            if (ultChamps_ == (numPlayers_ - 9)) {
                std::cout << "Max ultimate champs reached in " << battlesPlayed << " battles" << endl;
                printAllPlayers();
                exit(0);
            }
        }
    }

size_t Simulation::ucPctSimulation(float ultPct){
    size_t battlesPlayed = 0;
    size_t ultChampsRequired = ultPct * numPlayers_;
    uniform_real_distribution<double> doubleDist(0, 1);
    uniform_int_distribution<size_t> playerDist(0, numPlayers_ - 1);
    for (size_t season = 0; season < 5; ++season){
        seasonReset();
        battlesPlayed = 0;
        ultChamps_ = 0;
        // auto seasonStart = chrono::high_resolution_clock::now();
        while (ultChamps_ < ultChampsRequired) {
            playBattle(doubleDist, playerDist, battlesPlayed);
        }
        auto curTime = chrono::high_resolution_clock::now();
        // std::cout << "Season " << season << " complete in " << battlesPlayed << " battles played" << "\n";
        // chrono::duration<double> timeSoFar = curTime - seasonStart;
        // std::cout << "Avg Battles/sec: " << battlesPlayed / timeSoFar.count() << "\n" <<  endl;
    }
    return battlesPlayed; // returns the number of battles for the LAST SEASON
}


void Simulation::seasonReset(){
    for (Player* p : players_){
        p->reset(maxMultiplier_, goldStepRules_[0]);
    }
    queue_->reset();
}

void Simulation::printAllPlayers(){
    for (auto &p : players_) {
        outStream_ << *p << endl;
    }
    outStream_ << endl;
}
