#ifndef SIMULATION_HPP_INCLUDED
#define SIMULATION_HPP_INCLUDED

#include "player.hpp"
#include "hashQueue.hpp"
#include <ostream>
#include <fstream>
#include <string>
#include <chrono>
#include <random> 

class Simulation
{
private:
    std::mt19937 rng_;
    std::vector<Player*> players_;
    u_int8_t goldStepRules_[10];
    u_int8_t stepRequirements_[10];
    size_t ultChamps_ = 0;
    size_t numPlayers_;
    HashQueue* queue_;
    size_t seed_;
    u_int8_t maxMultiplier_;
    bool dropLeague_;
    std::ofstream outStream_;

public:

    // Constructors
    Simulation(std::string settings, std::string playerFile, size_t seed, size_t numPlayers, std::string outputFile);
    ~Simulation();
    Simulation(Simulation &other) = delete;

    /**
     * @brief Simulates the PoL until there are enough players in UC. 
     * 
     * @param ultPct Percentage of UCs. Target value should be between 0.005 and 0.02
     * @return size_t Number of battles required to reach this percent UC. 
     */
    size_t ucPctSimulation(float ultPct);

    /**
     * @brief Simulates a large amount of battles from starting players
     * 
     * @param numBattles Number of battles to play
     * @return float Percentage of players in UC. 
     */
    float nBattlesSimulation(size_t numBattles);

    /**
     * @brief Plays a battle. Used to eliminate code duplication. 
     * 
     * @param doubleDist 
     * @param playerDist 
     */
    void playBattle(std::uniform_real_distribution<double>& doubleDist, 
                    std::uniform_int_distribution<size_t>& playerDist, size_t& battlesPlayed);

    /**
     * @brief Resets all players
     *
     */
    void seasonReset();
    /**
     * @brief Prints all players to the outstream_
     *
     */
    void printAllPlayers();
};

#endif