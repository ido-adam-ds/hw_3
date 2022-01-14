#ifndef PLAYER_H
#define PLAYER_H

namespace DS{
    
    class Player{
    
    private:

        unsigned int PlayerID;
        unsigned int level;
        unsigned int score;

    public:

        Player(unsigned int PlayerID, unsigned int level,
            unsigned int score): PlayerID(PlayerID), level(level), score(score){}
        ~Player() = default;
        
        unsigned int getPlayerID() const{
            return PlayerID;
        } 
    };


}


#endif