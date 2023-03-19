#include "player.hpp"
#include <iostream>
#include <ostream>
#include <algorithm>

using namespace std;


Player::Player(size_t id, float partyPct, short oldPB, 
                u_int8_t kingTower, u_int8_t cardLevel, u_int8_t winsToGold):
    id_{id}, partyPct_{partyPct}, wins_{0}, losses_{0}, oldPB_{oldPB}, step_{0},
    kingTower_{kingTower}, cardLevel_{cardLevel},league_{0}, multiplier_{1},
    winsToGold_{winsToGold}, currentGold_{0}, ultChamp_{false}
    {
        //nothing to do here
    }

u_int8_t Player::getStep(){
    return step_;
}

size_t Player::getId(){
    return id_;
}

u_int8_t Player:: getKt(){
    return kingTower_;
}

u_int8_t Player::getCl(){
    return cardLevel_;
}

float Player::getPartyPct(){
    return partyPct_;
}

short Player::getWins(){
    return wins_;
}

short Player::getLosses() {
    return losses_;
}

u_int8_t Player::getLeague() {
    return league_;
}

bool Player::isUC(){
    return ultChamp_;
}

void Player::reset(u_int8_t maxMultiplier, u_int8_t winsToGold) {
    ultChamp_ = false;
    u_int8_t mult = league_ + 1;
    multiplier_ = min(mult, maxMultiplier);
    wins_ = 0;
    step_ = 0;
    losses_ = 0;
    winsToGold_ = winsToGold;
    league_ = 0;
    currentGold_ = 0;
}

bool  Player::operator==(const Player& other) const {
    return id_ == other.id_;
}

bool Player::operator!=(const Player& other) const {
    return !(*this == other);
}

bool Player::matchAllowed(Player& other) const {
    // Can only play other players in their league
    if (*this == other){ //same player
        return false;
    }
    return league_ == other.league_;
}
void Player::playMatch(Player& other, double randomVal, u_int8_t* goldStepRules,
                        u_int8_t* leagueReqs, bool dropLeague, bool& newUC){
    short pbDiff = abs(oldPB_ - other.oldPB_);
    long double higherPBwins = -0.0000001097213 * pbDiff * pbDiff + 0.00030971 * pbDiff + 0.48544;

    //levels are *very close* on the path.
    // Assume that low levels wrapped up in low old pb
    if ((oldPB_ > other.oldPB_ and randomVal < higherPBwins) or (oldPB_ < other.oldPB_ and randomVal > higherPBwins)){ 
        // ^ player is higher pb and higher pb wins^^        ^ player is lower pb and higher pb loses
        // cout << "player 1 wins" << endl;
        winsMatch(other, goldStepRules, leagueReqs, dropLeague);
        if (league_ == 9){
            ultChamp_ = true;
            newUC = true;
        }
    } else {
        // cout << "player2 wins" << endl;
        other.winsMatch(*this, goldStepRules, leagueReqs, dropLeague);
        if (other.league_ == 9){
            other.ultChamp_ = true;
            newUC = true;
        }
    }    
}
void Player::winsMatch(Player& other, u_int8_t* goldStepRules,
                     u_int8_t* leagueReqs,  bool dropLeague){
    // Handle winner
    step_ += multiplier_;
    if (multiplier_ > 1){
        --multiplier_;
    }
    
    // Reach new league, refresh golden steps
    if (step_ >= leagueReqs[league_+1]){
        ++league_;
        winsToGold_ = goldStepRules[league_];

        // Reaching new league, if can't drop league, make current step gold
        if (!dropLeague){
            currentGold_ = step_;
        }
    } else if (winsToGold_ == 1) {
        // "next" win is a golden step
        currentGold_ = step_; // make next step gold
        winsToGold_ = goldStepRules[league_]; // reset wins req'd to get next golden step

        // If there are gold steps in the league, reduce steps to golden step
    } else if (winsToGold_ != 255) {
        --winsToGold_;
    } // No gold steps, no new league, just increase step count

    // Handle losing player case
    if (other.step_ != other.currentGold_){
        --other.step_;
    }
    if (other.step_ < leagueReqs[other.league_]){
        --other.league_;
    }
    ++wins_;
    ++other.losses_;
    return;
}

void Player::printToStream(ostream& out) const {
    // debugging player
    // out << "(" << id_ << ", " << int(wins_) << ", " << int(league_) << ", " << int(winsToGold_) << ", " << int(currentGold_) << ", " << int(step_) << ")";
    out << id_ << " " << int(wins_) << " " << int(losses_) << " "<< int(league_) << " " << int(step_);
}

ostream& operator<<(ostream& outStream, const Player& p){
    p.printToStream(outStream);
    return outStream;
}