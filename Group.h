#ifndef GROUP_H
#define GROUP_H

#include "AVLRanktree->h"
#include "Player->h"

namespace DS{

     
    class Group{
    private:
    
        int GroupID;
        int scale;
        std::shared_ptr<Group> father;
        std::shared_ptr<AVLRanktree> levels_tree;
        int group_size;
        //shared_ptr<AVLtree<shared_ptr<Player>, int, comparePlayers>> players;

    public:
        explicit Group(int GroupID, int scale):scale(scale), GroupID(GroupID), levels_tree(new AVLRanktree(nullptr)), father(nullptr), group_size(1){}
        Group(int GroupID,int scale, shared_ptr<AVLRanktree> tree_to_insert):scale(scale),
                GroupID(GroupID), levels_tree(tree_to_insert), father(nullptr), group_size(1) {}
        Group(const Group& other) = default;
        ~Group() = default;

        void insertPlayer(int PlayerID, int score);
        void increseLevel(int level, int score);
        int getGroupId() const{
            return GroupID;
        }

        void setGroupId(int new_id){
            GroupID = new_id;
        }

        int getNumOfPlayers() const{
            return levels_tree->getNumOfPlayers();
        }

 /*       void addPlayer(shared_ptr<Player> player_to_insert){
            players->insert(player_to_insert, player_to_insert->getPlayerID());
        }

        void RemovePlayer(shared_ptr<Player> player_to_remove){
            players->remove(player_to_remove);
        }

        void getHighestPlayer(int* PlayerID) const{
            *PlayerID = players->getMax()->key->getPlayerID();
        }

        int getHighestPlayer() const{
            return players->getMax()->key->getPlayerID();
        }

        void getAllPlayersByLevel(int** Players, int* numOfPlayers){
            players->printPlayersByLevel(Players, numOfPlayers);
        }

        shared_ptr<AVLtree<shared_ptr<Player>, int, comparePlayers>> getPlayersTree(){
            return players;
        }
*/


        void insertPlayer(int score);

        void increseLevel(int old_level, int new_level, int score);

        void removePlayer(int level, int score);
    };

     
    void Group::insertPlayer(int score)
    {
        levels_tree->addPlayer(0, score);
    }

     
    void Group::increseLevel(int old_level, int new_level, int score)
    {
        levels_tree->removePlayer(old_level, score);
        levels_tree->addPlayer(new_level, score);
    }

     
    void Group::removePlayer(int level, int score)
    {
        levels_tree->removePlayer(level, score);
    }



}

#endif