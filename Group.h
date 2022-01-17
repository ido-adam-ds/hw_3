#ifndef GROUP_H
#define GROUP_H

#include "AVLRanktree.h"
#include <memory>

using std::shared_ptr;

namespace DS{

     
    class Group{
    private:
    
        int GroupID;
        Group* father;
        shared_ptr<AVLRanktree> levels_tree;
        int group_size;

    public:
        explicit Group(int GroupID) : GroupID(GroupID),
         levels_tree(new AVLRanktree()), father(nullptr), group_size(0){}
        /*Group(int GroupID, shared_ptr<AVLRanktree> tree_to_insert): 
                GroupID(GroupID), levels_tree(tree_to_insert), father(nullptr), group_size(tree_to_insert->getSize()){}*/
        Group(const Group& other) = default;
        ~Group() = default;

        int getGroupId() const{
            return GroupID;
        }

        void setGroupId(int new_id){
            GroupID = new_id;
        }

        int getNumOfPlayers() const{
            return levels_tree->getNumOfPlayersInTree();
        }

        int getSize() const{
            return group_size;
        }

        void insertPlayer(int score);

        void increaseLevel(int old_level, int new_level, int score);

        void removePlayer(int level, int score);

        int getNumOfPlayersInRange(int level1, int level2);

        int getNumOfPlayersInRangeWithScore(int level1, int level2, int score);

        int getLevelOfPlayerM(int m);

        Group* getFather() const{
            return father;
        }

        void setFather(Group* new_father){
            father = new_father;
        }

        shared_ptr<AVLRanktree> getLevelsTree() const{
            return levels_tree;
        }

        void setLevelsTree(shared_ptr<AVLRanktree> new_levels_tree){
            levels_tree = new_levels_tree;
        }

        void mergeLevelsTreeWithAnotherGroup(Group* other_group);
    };
    

}

#endif