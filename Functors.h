#ifndef FUNCTORS_H
#define FUNCTORS_H

#include "Player.h"
#include <memory>

#define FIRST_IS_SMALLER -1
#define EQUAL_NUMBERS

using std::shared_ptr;

namespace DS{

    const int left_is_smaller = -1;
    const int right_is_smaller = 1;
    const int equal = 0;

    /*class comparePlayers{
    public:
        comparePlayers() = default;
        int operator()(shared_ptr<Player> first, shared_ptr<Player> second) const{
            if(*first < *second){
                return left_is_smaller;
            }
            else if(*first == *second){
                return equal;
            }
            return right_is_smaller;
        }
    };*/

    class compareInts{
    private:
    public:
        compareInts() = default;
        int operator()(int first, int second) const{
            if(first < second){
                return left_is_smaller;
            }
            else if(first == second){
                return equal;
            }
            return right_is_smaller;
        }
    };
    /*
    void printPlayersByLevel(AVLtree<shared_ptr<Player>,shared_ptr<Player>,comparePlayers>* tree,
                                 AVLnode<shared_ptr<Player>,shared_ptr<Player>>* node, int** Players){
        if(node == nullptr){
            return;
        }
        printPlayersByLevel(tree, node->right_son, Players);
        int player_id_to_insert = node->key.get()->getPlayerID();
        (*Players)[tree->getIterator()] = player_id_to_insert;
        tree->setIterator(tree->getIterator() + 1);
        printPlayersByLevel(tree, node->left_son, Players);
    }
    void printMaxPlayersInGroups(AVLtree<int,shared_ptr<Group>,compareInts>* tree,
                                    AVLnode<int,shared_ptr<Group>>* node, int** Players){
        if(node == nullptr){
            return;
        }
        printMaxPlayersInGroups(tree, node->right_son, Players);
        int player_id_to_insert = (*(node->value)).getHighestPlayer();
        (*Players)[tree->getIterator()] = player_id_to_insert;
        tree->setIterator(tree->getIterator() + 1);
        printMaxPlayersInGroups(tree, node->left_son, Players);
    }*/


    
}

#endif