/**
 * @file player.hpp
 * @author Nova Light (keymckeyface@gmail.com)
 * @brief Player class that supports a model of a CR ladder player
 * @version 0.1
 * @date 2023-03-11
 * 
 * @copyright Copyright (c) 2022. MIT lisence
 * 
 */

#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED

#include <iostream>
#include <ostream>
#include <random>
#include <algorithm>


class Player
{
private:

    size_t id_;
    // Percent of games that are not PoL ranked games. 
    float partyPct_; 
    short wins_;
    short losses_;
    short oldPB_;
    u_int8_t step_;
    u_int8_t kingTower_;
    u_int8_t cardLevel_;
    u_int8_t league_;
    u_int8_t multiplier_;
    // Remaining wins to gold step in current league
    u_int8_t winsToGold_;
    // Current gold step player can't fall below
    u_int8_t currentGold_;
    bool ultChamp_;

public:
    // constructors and destructors
    Player() = delete; // default constructor
    Player(size_t id, float partyPct, short oldPB, u_int8_t kingTower, 
            u_int8_t cardLevel, u_int8_t winsToGold);
    Player(const Player &other) = default; // copy constructor
    ~Player() = default;
    Player &operator=(const Player &other);

    // Methods

    // Getters
    u_int8_t getStep();
    size_t getId();
    u_int8_t getKt();
    u_int8_t getCl();
    float getPartyPct();
    short getWins();
    short getLosses();
    u_int8_t getLeague();
    bool isUC();

    /**
     * @brief Plays a match
     * 
     * @param other player to play against
     * @param randomVal Random value to determine who to win against
     * @param curToGold Steps to gold step in current league
     * @param nextToGold Steps to gold step in the next league
     * @param nextLeague number of steps required for the next league
     * @param dropLeague If there is golden steps at the end of each league
     * @param newUC If the winner is a new UC player, change to true. 
     * @note Potential bug if the player skips a full league and wins/gold step is incorrect
     */
    void playMatch(Player& other, double randomVal, u_int8_t* goldStepRules,
                     u_int8_t* leagueReqs, bool dropLeague, bool& newUC);

    /**
     * @brief updates players when a match is played
     * 
     * @param other the player that *this won against
     * @param curToGold number of wins for a golden step in this league
     * @param nextToGold number of wins for gold in next league
     * @param nextLeague number of steps required for next league
     * @param dropLeague If golden steps are at the bottom of each league
     * 
     */
    void winsMatch(Player &other, u_int8_t* goldStepRules,
                     u_int8_t* leagueReqs, bool dropleague);

    /**
     * @brief Checks if match is allowed. Function is never called...
     * 
     * @param other Player to match against
     * @return true Valid match
     * @return false Invalid match
     */
    bool matchAllowed(Player& other) const;

    /**
     * @brief Resets a player to league 1. Reset W/L? P
     * 
     * @param finishLeague League that the player finished the season in
     * @param maxMultplier The win multiplier for next season
     * @param winsToGold Wins to gold for the starting league. 
     */
    void reset(u_int8_t maxMultiplier, u_int8_t winsToGold);

    /**
     * @brief Checks if 2 players are identical
     *
     * @param other player to check against
     * @return true players are equal  (equal ids)
     * @return false players are distinct
     */
    bool operator==(const Player& other) const;

    bool operator!=(const Player& other) const;

    /**
     * @brief prints out the player.  
     * 
     * @param out ostream to modify
     * @note Will be changed after debugging to print wins and winPct for data analysis
     */
    void printToStream(std::ostream& out) const;
};

// Non Class methods

std::ostream& operator<<(std::ostream& outStream, const Player& p);

#endif