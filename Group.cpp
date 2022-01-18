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


void Group::insertPlayer(int score)
{
    levels_tree->addPlayer(0, score);
}

    
void Group::increaseLevel(int old_level, int new_level, int score)
{
    levels_tree->removePlayer(old_level, score);
    levels_tree->addPlayer(new_level, score);
}

    
void Group::removePlayer(int level, int score)
{
    levels_tree->removePlayer(level, score);
}


int Group::getNumOfPlayersInRange(int level1, int level2)
{
    return levels_tree->getNumOfPlayersInRange(level1, level2);
}

int Group::getNumOfPlayersInRangeWithScore(int level1, int level2, int score)

{
    return levels_tree->getNumOfPlayersWithScoreInRange(level1, level2 ,score);
}


int Group::getLevelOfPlayerM(int m)
{
    return levels_tree->getLevelOfPlayerM(m);
}

static void deleteTreeArray(AVLnode** array, int size){
    for(int i = 0; i < size; i++){
        delete array[i];
    }
    delete[] array;
}


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
    root->player_weight = array[mid]->player_weight;
    for(int score = 0; score < MAX_SIZE; score++){
        root->scores[score] = array[mid]->scores[score];
    }
    root->left_son = buildTreeFromSortedArrayAux(array, start, mid - 1);
    root->right_son = buildTreeFromSortedArrayAux(array, mid + 1, end);
    //root->weighted_sum += (root->key)*(root->player_weight);

    if(root->right_son){
        root->right_son->father = root;
        for(int score = 0; score < MAX_SIZE; score++){
            root->scores[score] += root->right_son->scores[score];
        }
        root->player_weight += root->right_son->player_weight;
        //root->weighted_sum += (root->right_son->key)*(root->right_son->player_weight);
    }
    if(root->left_son){
        root->left_son->father = root;
        for(int score = 0; score < MAX_SIZE; score++){
            root->scores[score] += root->left_son->scores[score];
        }
        root->player_weight += root->left_son->player_weight;
        //root->weighted_sum += (root->left_son->key)*(root->left_son->player_weight);
    }

    root->weighted_sum = getWeightedSum(root->right_son)+getWeightedSum(root->left_son) + getNumPlayers(root)*root->key;
    return root;
}

static shared_ptr<AVLRanktree> buildTreeFromSortedArray(AVLnode** array,
                        int* first_zero_level_players, int* second_zero_level_players,
                                int total_zero_level_players, int size){
    shared_ptr<AVLRanktree> result_tree(new AVLRanktree());
    if(array[0]){
        result_tree->setRoot(buildTreeFromSortedArrayAux(array, 0, size - 1));
        AVLnode* iter = result_tree->getRoot();
        //iter->player_weight= getNumPlayers(iter)*iter->key + getWeightedSum(iter->right_son) + getWeightedSum(iter->left_son);
        AVLnode* prev = iter;
        AVLnode* max_to_insert;
        while(iter){
            prev = iter;
            iter = iter -> right_son;
        }
        max_to_insert = prev;
        result_tree->setMax(max_to_insert);
        updateHeightsInTree(result_tree->getRoot());
    }
    for(int score = 0; score < MAX_SIZE; score++){
        result_tree->getZeroLevelPlayersArray()[score] =
             first_zero_level_players[score] + second_zero_level_players[score];
    }
    result_tree -> setIterator(0);
    result_tree->setNumOfZeroLevelPlayers(total_zero_level_players);
    result_tree->setSize(size);
    if(result_tree->getRoot())
    {

        result_tree->getRoot()->weighted_sum = getWeightedSum(result_tree->getRoot()->right_son)+getWeightedSum(result_tree->getRoot()->left_son) + getNumPlayers(result_tree->getRoot())*result_tree->getRoot()->key;
        assert(getWeightedSum(result_tree->getRoot()) == getWeightedSum(result_tree->getRoot()->right_son)+getWeightedSum(result_tree->getRoot()->left_son) + getNumPlayers(result_tree->getRoot())*result_tree->getRoot()->key);
    }
    return result_tree;
}

static void mergeArraysAux(AVLnode** first_array, AVLnode** second_array,
                         int size_of_first, int size_of_second, AVLnode** merged){
    int i = 0, j = 0, k = 0;
    while(i < size_of_first && j < size_of_second &&
         first_array[i] && second_array[j]){
        if(first_array[i]->key < second_array[j]->key){
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
           // node_to_insert->weighted_sum = first_array[i]->weighted_sum + second_array[j]->weighted_sum;
            merged[k++] = node_to_insert;
            i++;
            j++;
        }
    }
    while(i < size_of_first && first_array[i]){
        //merged[k++] = first_array[i++];
        AVLnode* node_to_insert = new AVLnode(first_array[i]->key);
        for(int score = 0; score < MAX_SIZE; score++){
            node_to_insert->scores[score] += first_array[i]->scores[score];
        }
        node_to_insert->player_weight = first_array[i]->player_weight;
        merged[k++] = node_to_insert;
        i++;
    }
    while(j < size_of_second && second_array[j]){
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
    AVLnode** merged = new AVLnode*[size_of_first+size_of_second+1];
    for(int i = 0; i < size_of_first+size_of_second+1; i++){
        merged[i] = nullptr;
    }
    mergeArraysAux(first_array, second_array, size_of_first, size_of_second, merged);
    return merged;
}

static void traverseTreeToListAux(shared_ptr<AVLRanktree> tree, AVLnode** array, AVLnode* node){
    if(node == nullptr){
        return;
    }
    traverseTreeToListAux(tree, array, node->left_son);
    AVLnode* node_to_insert = new AVLnode(node->key);
    for(int score = 0; score < MAX_SIZE; score++){
        node_to_insert->scores[score] = getNumPlayersWithScore(node, score);
    }
    node_to_insert->player_weight = getNumPlayers(node);
    array[tree->getIterator()] = node_to_insert;
    tree->setIterator(tree->getIterator() + 1);
    traverseTreeToListAux(tree, array, node->right_son);
}

static AVLnode** traverseTreeToList(shared_ptr<AVLRanktree> tree){
    AVLnode** array = new AVLnode*[tree->getSize()+1];
    for(int i = 0; i < tree->getSize()+1; i++){
        array[i] = nullptr;
    }
    traverseTreeToListAux(tree, array, tree->getRoot());
    tree->setIterator(0);
    return array;
}

static shared_ptr<AVLRanktree> buildTreeFromTrees(shared_ptr<AVLRanktree> first_tree,
            shared_ptr<AVLRanktree> second_tree){
    AVLnode** first_array = traverseTreeToList(first_tree);
    AVLnode** second_array = traverseTreeToList(second_tree);
    AVLnode** merged = mergeArrays(first_array, second_array,
        first_tree->getSize(), second_tree->getSize());
    int actual_size = 0;
    for(int j = 0; j < first_tree->getSize()+second_tree->getSize(); j++){
        if(merged[j] != nullptr){
            actual_size++;
        }
    }
    shared_ptr<AVLRanktree> result_tree = buildTreeFromSortedArray(merged,
         first_tree->getZeroLevelPlayersArray(), second_tree->getZeroLevelPlayersArray(),
          first_tree->getNumOfZeroLevelPlayers()+second_tree->getNumOfZeroLevelPlayers(),
            actual_size);
    deleteTreeArray(first_array, first_tree->getSize()+1);
    deleteTreeArray(second_array, second_tree->getSize()+1);
    deleteTreeArray(merged, first_tree->getSize() + second_tree->getSize() + 1);
    return result_tree;
}

void Group::mergeLevelsTreeWithAnotherGroup(Group* other_group){
    if(getGroupId()==7||other_group->getGroupId() == 7)
        printf("");
    shared_ptr<AVLRanktree> merged_tree = buildTreeFromTrees(levels_tree,
         other_group->getLevelsTree());
    group_size += other_group->getSize();
    levels_tree = merged_tree;
}