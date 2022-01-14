#include "Group.h"

using namespace DS;

static void traverseTreeToListAux(shared_ptr<AVLRanktree> tree, AVLnode** array, AVLnode* node);

static AVLnode** traverseTreeToList(shared_ptr<AVLRanktree> tree);

static void mergeArraysAux(AVLnode** first_array, AVLnode** second_array,
                         int size_of_first, int size_of_second, AVLnode** merged);

static AVLnode** mergeArrays(AVLnode** first_array, AVLnode** second_array,
                                         int size_of_first, int size_of_second);

static AVLnode* buildTreeFromSortedArrayAux(AVLnode** array, int start, int end);

static shared_ptr<AVLRanktree> buildTreeFromSortedArray(AVLnode** array,
                 int* first_zero_level_players, int* second_zero_level_players,
                                                             int total_zero_level_players, int size);

static shared_ptr<AVLRanktree> buildTreeFromTrees(shared_ptr<AVLRanktree> first_tree,
                                 shared_ptr<AVLRanktree> second_tree);

static void deleteTreeArray(AVLnode** array, int size);


static int updateHeightsInTree(AVLnode* node){
    if(node == nullptr){
        return -1;
    }
    if(node -> left_son){
        node->left_son->height = updateHeightsInTree(node->left_son);
    }
    if(node -> right_son){
        node->right_son->height = updateHeightsInTree(node->right_son);
    }
    node->height = maximum(getHeight(node->right_son), getHeight(node->left_son))+1;
    return maximum(getHeight(node->right_son), getHeight(node->left_son))+1;
}


static AVLnode* buildTreeFromSortedArrayAux(AVLnode** array, int start, int end){
    if(start > end){
        return nullptr;
    }
    int mid = (start + end)/2;
    AVLnode* root = new AVLnode(array[mid]->key);
    root->left_son = buildTreeFromSortedArrayAux(array, start, mid - 1);
    root->right_son = buildTreeFromSortedArrayAux(array, mid + 1, end);
    root->weighted_sum += (root->key)*(root->player_weight);
    if(root->right_son){
        root->right_son->father = root;
        for(int score = 0; score < MAX_SIZE; score++){
            root->scores[score] += root->right_son->scores[score];
        }
        root->player_weight += root->right_son->player_weight;
        root->weighted_sum += (root->right_son->key)*(root->right_son->player_weight);
    }
    if(root->left_son){
        root->left_son->father = root;
        for(int score = 0; score < MAX_SIZE; score++){
            root->scores[score] += root->left_son->scores[score];
        }
        root->player_weight += root->left_son->player_weight;
        root->weighted_sum += (root->left_son->key)*(root->left_son->player_weight);
    }
    return root;
}

static shared_ptr<AVLRanktree> buildTreeFromSortedArray(AVLnode** array,
                        int* first_zero_level_players, int* second_zero_level_players,
                                int total_zero_level_players, int size){
    shared_ptr<AVLRanktree> result_tree(new AVLRanktree());
    result_tree->setRoot(buildTreeFromSortedArrayAux(array, 0, size - 1));
    AVLnode* iter = result_tree->getRoot();
    AVLnode* prev = iter;
    AVLnode* max_to_insert;
    while(iter){
        prev = iter;
        iter = iter -> right_son;
    }
    max_to_insert = prev;
    result_tree->setMax(max_to_insert);
    result_tree->setSize(size);
    result_tree -> setIterator(0);
    updateHeightsInTree(result_tree->getRoot());
    for(int score = 0; score < MAX_SIZE; score++){
        result_tree->getZeroLevelPlayers()[score] = first_zero_level_players[score] + second_zero_level_players[score];
    }
    return result_tree;
}

static void mergeArraysAux(AVLnode** first_array, AVLnode** second_array,
                         int size_of_first, int size_of_second, AVLnode** merged){
    int i = 0, j = 0, k = 0;
    while(i < size_of_first && j < size_of_second){
        if(first_array[i]->key < second_array[j]->key){
            //merged[k++] = first_array[i++];
            AVLnode* node_to_insert = new AVLnode(first_array[i]->key);
            for(int score = 0; score < MAX_SIZE; score++){
                node_to_insert->scores[score] += first_array[i]->scores[score];
            }
            node_to_insert->player_weight = first_array[i]->player_weight;
            merged[k++] = node_to_insert;
            i++;
        }
        else if(first_array[i] -> key > second_array[j] -> key){
            //merged[k++] = second_array[j++];
            AVLnode* node_to_insert = new AVLnode(second_array[j]->key);
            for(int score = 0; score < MAX_SIZE; score++){
                node_to_insert->scores[score] += second_array[j]->scores[score];
            }
            node_to_insert->player_weight = second_array[j]->player_weight;
            merged[k++] = node_to_insert;
            j++;
        }
        else{ //same level to insert
            AVLnode* node_to_insert = new AVLnode(second_array[j]->key);
            for(int score = 0; score < MAX_SIZE; score++){
                node_to_insert->scores[score] = first_array[i]->scores[score] + 
                                                    second_array[j]->scores[score];
            }
            node_to_insert->player_weight = first_array[i]->player_weight + second_array[j]->player_weight;
            merged[k++] = node_to_insert;
            i++;
            j++;
        }
    }
    while(i < size_of_first){
        //merged[k++] = first_array[i++];
        AVLnode* node_to_insert = new AVLnode(first_array[i]->key);
        for(int score = 0; score < MAX_SIZE; score++){
            node_to_insert->scores[score] += first_array[i]->scores[score];
        }
        node_to_insert->player_weight = first_array[i]->player_weight;
        merged[k++] = node_to_insert;
        i++;
    }
    while(j < size_of_second){
        //merged[k++] = second_array[j++];
        AVLnode* node_to_insert = new AVLnode(second_array[j]->key);
        for(int score = 0; score < MAX_SIZE; score++){
            node_to_insert->scores[score] += second_array[j]->scores[score];
        }
        node_to_insert->player_weight = second_array[j]->player_weight;
        merged[k++] = node_to_insert;
        j++;
    }
}

static AVLnode** mergeArrays(AVLnode** first_array, AVLnode** second_array,
                                         int size_of_first, int size_of_second){
    AVLnode** merged = new AVLnode*[size_of_first+size_of_second];
    mergeArraysAux(first_array, second_array, size_of_first, size_of_second, merged);
    return merged;
}

static void traverseTreeToListAux(shared_ptr<AVLRanktree> tree, AVLnode** array, AVLnode* node){
    if(node == nullptr){
        return;
    }
    traverseTreeToListAux(tree, array, node->left_son);
    AVLnode* node_to_insert = new AVLnode(node->key);
    levelScores(node_to_insert->scores, node->left_son, node->right_son);
    node_to_insert->player_weight = getNumPlayers(node);
    array[tree->getIterator()] = node_to_insert;
    tree->setIterator(tree->getIterator() + 1);
    traverseTreeToListAux(tree, array, node->right_son);
}

static AVLnode** traverseTreeToList(shared_ptr<AVLRanktree> tree){
    AVLnode** array = new AVLnode*[tree->getSize()];
    traverseTreeToListAux(tree, array, tree->getRoot());
    tree->setIterator(0);
    return array;
}

static shared_ptr<AVLRanktree> buildTreeFromTrees(shared_ptr<AVLRanktree> first_tree, shared_ptr<AVLRanktree> second_tree){
    AVLnode** first_array = traverseTreeToList(first_tree);
    AVLnode** second_array = traverseTreeToList(second_tree);
    AVLnode** merged = mergeArrays(first_array, second_array,
                             first_tree->getSize(), second_tree->getSize());
    shared_ptr<AVLRanktree> result_tree = buildTreeFromSortedArray(merged,
         first_tree->getZeroLevelPlayers(), second_tree->getZeroLevelPlayers(),
          first_tree->getNumOfZeroLevelPlayers()+second_tree->getNumOfZeroLevelPlayers(),
                first_tree->getSize()+second_tree->getSize());
    deleteTreeArray(first_array, first_tree->getSize());
    deleteTreeArray(second_array, second_tree->getSize());
    deleteTreeArray(merged, first_tree->getSize() + second_tree->getSize());
    return result_tree;
}

void Group::mergeLevelsTreeWithAnotherGroup(Group* other_group){
    shared_ptr<AVLRanktree> merged_tree = buildTreeFromTrees(levels_tree, other_group->getLevelsTree());
    group_size += other_group->getSize();
    levels_tree = merged_tree;
}