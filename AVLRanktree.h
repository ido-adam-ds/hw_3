//
// Created by Iddo Petrank on 30/11/2021.
//

#ifndef WET1_AVLRanktree_H
#define WET1_AVLRanktree_H

#include <string>
#include <iostream>
#include "AVLRanknode.h"
#include "Functors.h"
#define MAX_SIZE 201




namespace DS{

    static int maximum(int a, int b)
    {
        return a>b?a:b;
    }

    static void copyArray(int *dst, int *src)
    {
        for (int i = 0; i < MAX_SIZE; ++i) {
            dst[i] = src[i];
        }
    }




    
    class AVLRanktree {
        typedef AVLnode* Node;
    private:
        Node root;
        Node max;
        int size;
        compareInts compare;
        int num_of_zero_level_players;
        int* zero_level_scores;

        int iterator;

        void printPlayersByLevelAux(Node node, int** Players);
        void printMaxPlayersInGroupsAux(Node node, int** Players, int numOfGroups);

    public:
        Node RRrotate(Node node);
        Node LLrotate(Node node);
        Node LRrotate(Node node);
        Node RLrotate(Node node);
        Node search(Node iter,int key, bool update);
        int numOfPlayesWithScoreAndLowerLevel(int level, int score);
        int numOfPlayersWithLowerLevel(int level);
        Node handleRoll(Node node);
        Node nextNodeInOrder(Node node);
      //  Node handleRemove(Node to_remove);
        void preOrder(Node node);
        void inOrder(Node node);
        void postOrder(Node node);
        void deleteLeaf(Node  node);
        void handleMax(Node to_remove);
        void addPlayer(int level, int score);
        void removePlayer(int level, int score);
        void updateWeights(int level, int score, bool insert);
        void updateWeightsForSinglePlayer(int level, int score, bool insert);
        int getNumOfPlayersInTree();
        int getNumOfPlayersWithScoreInRange(int level1, int level2, int score);
        int getNumOfPlayersInRange(int level1, int level2);
        int getLevelOfPlayerM(int m);
        void swapWeights(AVLnode* root, AVLnode* new_root);
        void swapPlayerScores(AVLnode* root, AVLnode* new_root);
        void swapPlayerSumWeight(AVLnode* root, AVLnode* new_root);
        void swapPlayerWeight(AVLnode* root, AVLnode* new_root);
        void handleMiddlePlayerScores( Node iter,  int *dst_scores);
        void handleMiddlePlayerSumWeight( Node iter,  int dst_players);
        void handleMiddlePlayerWeight( Node iter,  int dst_players);
        void handleWeights(Node node, Node dst);









        //public:
        AVLRanktree(/*AVLRanktree *pRanktree*/) : root(nullptr) , max(nullptr), size(0), iterator(0),
         num_of_zero_level_players(0), compare(), zero_level_scores(new int[MAX_SIZE])
        {
            for (int i = 0; i < MAX_SIZE; ++i) {
                zero_level_scores[i] = 0;
            }
        };
        ~AVLRanktree(){
            delete[] zero_level_scores;
            clear();
        }
        void insert(int new_key);
        void remove(int key);
        void clear();
        void print();

        int getSize() const{
            return size;
        }

        void setSize(int new_size){
            size = new_size;
        }

        Node doesKeyExists(int key);

        const Node getMax() const{
            return max;
        }

        const Node getRoot() const{
            return root;
        }

        void setRoot(Node new_root){
            root = new_root;
        }

        void setMax(Node new_max){
            max = new_max;
        }

        int getIterator() const{
            return iterator;
        }

        void setIterator(int index){
            iterator = index;
        }

        void printPlayersByLevel(int** Players, int* numOfPlayers){
            *numOfPlayers = size;
            *Players = (int*)malloc(*numOfPlayers * sizeof(int));
            if(*Players == NULL){
                return;
            }
            printPlayersByLevelAux(root, Players);
            this->setIterator(0);
        }

        void printMaxPlayersInGroups(int** Players, int* numOfGroups){
            *Players = (int*)malloc(*numOfGroups * sizeof(int));
            if(*Players == NULL){
                return;
            }
            printMaxPlayersInGroupsAux(root, Players, *numOfGroups);
            this->setIterator(0);
        }

        int getNumOfZeroLevelPlayers() const{
            return num_of_zero_level_players;
        }

        int* getZeroLevelPlayersArray() const{
            return zero_level_scores;
        }

        void setNumOfZeroLevelPlayers(int new_num){
            num_of_zero_level_players = new_num;
        }

    };


}






#endif //WET1_AVLRanktree_H
