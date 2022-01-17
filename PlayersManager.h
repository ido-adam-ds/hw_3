#ifndef PLAYERS_MANAGER
#define PLAYERS_MANAGER

#include "AVLRanktree.h"
#include "Group.h"
#include "DynamicHashTable.h"
#include "UF.h"

namespace DS{

    class PlayersManager{

    private:

        DynamicHashTable players;
        UF groups;
        AVLRanktree all_players_levels_tree;
        int scale;
        int k;

    public:

        PlayersManager(int k, int scale): players(), groups(k),
                 all_players_levels_tree(), scale(scale), k(k){}
        ~PlayersManager() = default;

        void MergeGroups(int GroupID1, int GroupID2);
        void AddPlayer(int PlayerID, int GroupID, int score);
        void RemovePlayer(int PlayerID);
        void IncreasePlayerIDLevel(int PlayerID, int LevelIncrease);
        void ChangePlayerIDScore(int PlayerID, int NewScore);
        void GetPercentOfPlayersWithScoreInBounds(int GroupID, int score, int lowerLevel, int higherLevel,
                                                    double * players);
        void AverageHighestPlayerLevelByGroup(int GroupID, int m, double * level);
        void GetPlayersBound(int GroupID, int score, int m,
                                                int * LowerBoundPlayers, int * HigherBoundPlayers);

        int getNumOfGroups() const{
            return k;
        }

        int getScale() const{
            return scale;
        }
    };

}

#endif