#ifndef PLAYER_H
#define PLAYER_H

namespace DS{
    
    class Player{
    
    private:

        int PlayerID;
        int GroupID;
        int level;
        int score;

    public:

        Player(int PlayerID, int GroupID, int score): PlayerID(PlayerID),
             GroupID(GroupID), level(0), score(score){}
        ~Player() = default;
        
        int getPlayerID() const{
            return PlayerID;
        } 

        int getPlayerLevel() const{
            return level;
        }

        void setPlayerLevel(int new_level){
            level = new_level;
        }

        int getPlayerScore() const{
            return score;
        }

        void setPlayerScore(int NewScore){
            score = NewScore;
        }

        int getGroupID() const{
            return GroupID;
        }

    };


}


#endif