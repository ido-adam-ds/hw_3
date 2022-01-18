#include "AVLRanktree.h"

#include <stdio.h>

using std::cout;

using namespace DS;

AVLnode* AVLRanktree::LLrotate(Node node) {
    Node left_son = node->left_son;

    int node_players_num = getNumPlayers(node);
    int node_level_sum = getSumOfLevel(node);
    int node_scores[MAX_SIZE];
    copyArray(node_scores, node->scores);
    levelScores(node_scores, node->right_son, node->left_son);


    int node_right_level_sum = getSumOfLevel(left_son);
    int node_right_players_num = getNumPlayers(left_son);
    int node_right_scores[MAX_SIZE];
    copyArray(node_right_scores, left_son->scores);
    levelScores(node_right_scores, left_son->right_son, left_son->left_son);



    node->left_son = left_son->right_son;
    if(left_son->right_son)
    {
        left_son->right_son->father = node;
    }
    left_son->father = node->father;
    node->father = left_son;
    left_son->right_son = node;
    //node->left_son = left_son->left_son;
    node->height = maximum(getHeight(node->right_son), getHeight(node->left_son))+1;
    left_son->height = maximum(getHeight(left_son->right_son), getHeight(left_son->left_son))+1;


    DS::updateWeights(node, node_players_num, node_level_sum, node_scores);
    DS::updateWeights(left_son, node_right_players_num, node_right_level_sum, node_right_scores);

    return left_son;


}


AVLnode* AVLRanktree::RRrotate(Node node) {
    Node right_son = node->right_son;

    int node_players_num = getNumPlayers(node);
    int node_level_sum = getSumOfLevel(node);
    int node_scores[MAX_SIZE];
    copyArray(node_scores, node->scores);
    levelScores(node_scores, node->right_son, node->left_son);


    int node_right_level_sum = getSumOfLevel(right_son);
    int node_right_players_num = getNumPlayers(right_son);
    int node_right_scores[MAX_SIZE];
    copyArray(node_right_scores, right_son->scores);
    levelScores(node_right_scores, right_son->right_son, right_son->left_son);


    node->right_son = right_son->left_son;
    if(right_son->left_son)
    {
        right_son->left_son->father = node;
    }
    right_son->father = node->father;
    node->father = right_son;
    right_son->left_son = node;
    node->height = maximum(getHeight(node->right_son), getHeight(node->left_son))+1;
    right_son->height = maximum(getHeight(right_son->right_son),getHeight(right_son->left_son))+1;


    DS::updateWeights(node, node_players_num, node_level_sum, node_scores);
    DS::updateWeights(right_son, node_right_players_num, node_right_level_sum, node_right_scores);
    return right_son;
}


AVLnode* AVLRanktree::LRrotate(Node node)
{
    node->left_son = RRrotate(node->left_son);
    return LLrotate(node);
}


AVLnode* AVLRanktree::RLrotate(Node node)
{
    node->right_son = LLrotate(node->right_son);
    return RRrotate(node);
}

//



AVLnode* AVLRanktree::search(Node iter, int key, bool update) {

    //bool update = update_remove||update_insert;

    if (iter == nullptr) {
        return iter;
    }
    if (compare(key, iter->key) == left_is_smaller) {
        if (update) {
            iter->left_son = search(iter->left_son, key, update);
        } else {
            if (iter->left_son) {
                return search(iter->left_son, key, update);
            }
            return iter;
        }

    } else if (compare(key, iter->key) == right_is_smaller) {
        if (update) {
            iter->right_son = search(iter->right_son, key, update);
        } else {
            if (iter->right_son) {
                return search(iter->right_son, key, update);
            }
            return iter;
        }

    }
    return handleRoll(iter);

}


AVLnode* AVLRanktree::doesKeyExists(int  key){
    Node in_tree = search(root, key, false);
    if(in_tree == nullptr){
        return nullptr;
    }
    if(compare(in_tree->key, key) == equal){
        return in_tree;
    }  
    return nullptr;
}



void AVLRanktree::insert(int new_key)
{
    Node new_node = new AVLnode(new_key);
    if(root == nullptr)
    {
        root = new_node;
        max = new_node;
    }
    else{
        Node new_node_father = search(root, new_key, false);
        if(compare(new_key, new_node_father->key) == equal)
        {
            delete new_node;
            return;
        }
        if(compare(max->key, new_key) == left_is_smaller)
        {
            max = new_node;
        }
        if(compare((*new_node_father).key, new_key) == right_is_smaller)
        {
            new_node_father->left_son = new_node;
            new_node->father = new_node_father;
        }
        else{
            new_node_father->right_son = new_node;
            new_node->father = new_node_father;
        }
    }

    size++;
    root = search(root, new_key, true);

}



void AVLRanktree::remove(int key)
{
    Node to_remove = search(root, key, false);
    Node to_swap = nullptr;

    if(compare(to_remove->key, key) != equal)
    {
        return;
    }
    handleMax(to_remove);
    bool has_right_son = (to_remove->right_son);
    bool has_left_son = (to_remove->left_son);
    int key_for_update = key;


    if(has_left_son && has_right_son)
    {
        to_swap = nextNodeInOrder(to_remove);
        handleWeights(to_remove, to_swap);

        swap(to_remove, to_swap);
        if (root == to_remove)
        {
            root = to_swap;
        }

    //   to_remove = to_swap;

    }

    if(to_remove->father)
    {
        key_for_update = to_remove->father->key;
    }


    has_right_son = (to_remove->right_son);
    has_left_son = (to_remove->left_son);
    bool is_root = (to_remove == root);

    if(is_root)
    {

        if(has_left_son)
        {
            root->left_son->father = nullptr;
            root = root->left_son;
        }
        else {
            if(has_right_son)
                root->right_son->father = nullptr;
            root = root->right_son;//if no right son (and no left) root will receive nullptr
        }
    }
    else{
        bool is_right_son = (to_remove->father->right_son==to_remove);
        if(has_right_son)
        {
            if(is_right_son)
            {
                to_remove->father->right_son = to_remove->right_son;
            }else{
                to_remove->father->left_son = to_remove->right_son;
            }
            to_remove->right_son->father = to_remove->father;
            // key_for_update = to_remove->right_son->key;


        }
        else if(has_left_son){
            if(is_right_son)
            {
                to_remove->father->right_son = to_remove->left_son;
            }else{
                to_remove->father->left_son = to_remove->left_son;
            }
            to_remove->left_son->father = to_remove->father;
        // key_for_update = to_remove->left_son->key;
        }
        else{
            if(is_right_son)
            {
                to_remove->father->right_son = nullptr;
            }
            else
            {
                to_remove->father->left_son = nullptr;
            }
        //  key_for_update = to_remove->father->key;
        }


    }
    delete to_remove;


    //if(search(root, key_for_update, false).key != key_for_update)
    if(compare(key, key_for_update) != equal)
    {
        root = search(root, key_for_update,  true);
    }
    size--;



}










AVLnode* AVLRanktree::nextNodeInOrder(Node node)
{
    if(node->right_son == nullptr)
    {
        return nullptr;
    }

    Node iter = ((node)->right_son);
    while((iter)->left_son)
    {

        iter = ((iter)->left_son);
    }
    return iter;

}

void AVLRanktree::handleWeights(Node node, Node dst)
{
    int dst_players = getNumPlayers(dst);

    int dst_player_sum = getSumOfLevel(dst);


    int dst_scores[MAX_SIZE];
    for(int score = 0; score < MAX_SIZE; score++){
        dst_scores[score] = getNumPlayersWithScore(dst, score);
    }

    swapWeights(node, dst);


    Node iter = ((node)->right_son);
    while((iter)->left_son && iter != dst)
    {
        handleMiddlePlayerWeight(iter, dst_players);
        handleMiddlePlayerSumWeight(iter, dst_player_sum);
        handleMiddlePlayerScores(iter, dst_scores);
        iter = ((iter)->left_son);
    }


}

void AVLRanktree::handleMiddlePlayerWeight( Node iter,  int dst_players)
{

    iter->player_weight -=dst_players;
}

void AVLRanktree::handleMiddlePlayerSumWeight( Node iter,  int dst_players)
{

    iter->weighted_sum -=dst_players;
}

void AVLRanktree::handleMiddlePlayerScores( Node iter, int *dst_scores)
{

    for(int score = 0; score < MAX_SIZE; score++){
        iter->scores[score]-= dst_scores[score];
    }


}



 void AVLRanktree::swapPlayerWeight(AVLnode* root, AVLnode* new_root)
{
    new_root->player_weight = root->player_weight;
//
//    int temp_root_players = getNumPlayers(root);
//    int temp_new_root_players = getNumPlayers(new_root);
//    int root_weight = root->player_weight;
//
//    root->player_weight = new_root->player_weight+temp_root_players-temp_new_root_players;
//    new_root->player_weight = root_weight+temp_new_root_players-temp_root_players;

}
void AVLRanktree::swapPlayerSumWeight(AVLnode* root, AVLnode* new_root)
{
    new_root->weighted_sum = root->weighted_sum;

}

void AVLRanktree::swapPlayerScores(AVLnode* root, AVLnode* new_root)
{
    for(int score = 0; score < MAX_SIZE; score++){
        new_root->scores[score] = root->scores[score];
    }
//
//    int temp_root_scores[MAX_SIZE];
//    for(int score = 0; score < MAX_SIZE; score++){
//        temp_root_scores[score] = getNumPlayersWithScore(root, score);
//    }
//
//    int temp_new_root_scores[MAX_SIZE];
//    for(int score = 0; score < MAX_SIZE; score++){
//        temp_new_root_scores[score] = getNumPlayersWithScore(new_root, score);
//    }
//
//    int temp_scores[MAX_SIZE];
//    for(int score = 0; score < MAX_SIZE; score++){
//        temp_new_root_scores[score] = root->scores[score];
//    }
//
//    for(int score = 0; score < MAX_SIZE; score++){
//        root->scores[score] = new_root->scores[score]+temp_root_scores[score]-temp_new_root_scores[score];
//    }
//
//    for(int score = 0; score < MAX_SIZE; score++){
//        new_root->scores[score] = temp_scores[score]-temp_root_scores[score]+temp_new_root_scores[score];
//    }
//

}

void AVLRanktree::swapWeights(AVLnode* root, AVLnode* new_root)
{
    swapPlayerSumWeight(root, new_root);
    swapPlayerScores(root, new_root);
    swapPlayerWeight(root, new_root);


}





AVLnode* AVLRanktree::handleRoll(AVLnode* node)
{
    int balance_factor = node->getBalanceFactor();
    if (balance_factor == 2)
    {
        int son_balance_factor = node->left_son->getBalanceFactor();
        if(son_balance_factor == -1)
        {
            return LRrotate(node);
        }
        else
        {
            return LLrotate(node);
        }
    }
    else if (balance_factor == -2)
    {
        int son_balance_factor = node->right_son->getBalanceFactor();
        if(son_balance_factor == 1)
        {
            return RLrotate(node);
        }
        else
        {
            return RRrotate(node);
        }
    }
    else
    {
        int max_son_height = (getHeight(node->right_son) > getHeight(node->left_son))?getHeight(node->right_son):getHeight(node->left_son);
        node->height = max_son_height + 1;
        return node;
    }




}






void AVLRanktree::preOrder(Node node)
{
    if (node == nullptr) return;

// doSomthing(param);
    postOrder(node->left_son);
    postOrder(node->right_son);
}


void AVLRanktree::inOrder(Node node)
{
    if (node == nullptr) return;

    postOrder(node->left_son);
// doSomthing();
    postOrder(node->right_son);
}


void AVLRanktree::postOrder(Node node)
{
    if (node == nullptr) return;

    postOrder(node->left_son);
    postOrder(node->right_son);
    deleteLeaf(node);
}






void AVLRanktree::deleteLeaf(Node node)
{
    handleMax(node);
    if(node == root)
    {
        root = nullptr;
        delete node;
        return;
    }
    bool is_right_Son = node->father->right_son == node;
    if(is_right_Son)
    {
        node->father->right_son = nullptr;
        delete node;
    }
    else{
        node->father->left_son = nullptr;
        delete node;
    }
}




void AVLRanktree::handleMax(Node to_remove)
{
    if (max != to_remove) return;

    if (to_remove->left_son)
    {
        Node iter = to_remove->left_son;
        while(iter->right_son)
        {
            iter = iter->right_son;
        }
        max = iter;
    } else{
        max= to_remove->father;
    }
}




void AVLRanktree::clear()
{
    postOrder(root);
    size = 0;
}



int AVLRanktree::numOfPlayesWithScoreAndLowerLevel(int level, int score)
{
    if(level < 0) return 0;
    int r = 0;
    Node iter = root;
    while(iter != nullptr && iter->key != level)
    {
        bool isBigger = iter->key < level;
        if(isBigger)
        {
            r+=getScorePlayersWeight(iter->left_son, score)+getNumPlayersWithScore(iter, score);
            iter = iter->right_son;
        }
        else{
            iter = iter->left_son;
        }
    }
    if(iter != nullptr)
    {
        r+=getScorePlayersWeight(iter->left_son, score)+getNumPlayersWithScore(iter, score);
    }
    r+=zero_level_scores[score];
    return r;
}



int AVLRanktree::numOfPlayersWithLowerLevel(int level)
{
    if(level < 0) return 0;
    int r = 0;
    Node iter = root;
    while(iter != nullptr && iter->key != level)
    {
        bool isBigger = iter->key < level;
        if(isBigger)
        {
            r+=getAllPlayersWeight(iter->left_son)+ getNumPlayers(iter);
            iter = iter->right_son;
        }
        else{
            iter = iter->left_son;
        }
    }
    if(iter != nullptr)
    {
        r+=getAllPlayersWeight(iter->left_son)+getNumPlayers(iter);
    }
    r+=num_of_zero_level_players;
    return r;
}


int AVLRanktree::sumOfPlayersWithLowerLevel(int level, int* tot_players)
{
    if(level < 0) return 0;
    int r = 0;
    Node iter = root;
    while(iter != nullptr && iter->key != level)
    {
        bool isBigger = iter->key < level;
        if(isBigger)
        {
            r+=getWeightedSum(iter->left_son)+ getSumOfLevel(iter);
            iter = iter->right_son;
        }
        else{
            *tot_players += getNumPlayers(iter)+ getAllPlayersWeight(iter->right_son);

            iter = iter->left_son;
        }
    }
    if(iter != nullptr)
    {
        r+=getWeightedSum(iter->left_son)+getSumOfLevel(iter);
        *tot_players += getAllPlayersWeight(iter->right_son);
    }
    return r;
}




void AVLRanktree::addPlayer(int level, int score)
{
    if (level == 0)
    {
        num_of_zero_level_players++;
        zero_level_scores[score]++;
    }
    else {
        Node thisLevel = doesKeyExists(level);
        if(thisLevel)
        {
            updateWeightsForSinglePlayer(level, score, true);
        }
        else
        {
            insert(level);
            updateWeightsForSinglePlayer(level, score, true);
        }
    }
}


void AVLRanktree::removePlayer(int level, int score)
{
    if (level == 0)
    {
        num_of_zero_level_players--;
        zero_level_scores[score]--;
    }
    else {

        Node thisLevel = search(root, level, false);
        int num_of_players_in_level = getAllPlayersWeight(thisLevel) - 
            getAllPlayersWeight(thisLevel->right_son) - getAllPlayersWeight(thisLevel->left_son);
        if(num_of_players_in_level == 1)
        {
            updateWeightsForSinglePlayer(level, score, false);
            remove(level);
        }

        else
        {
            updateWeightsForSinglePlayer(level, score, false);
        }
    }
}





void AVLRanktree::updateWeightsForSinglePlayer(int level, int score, bool insert)
{
    Node iter = root;
    while(iter != nullptr && iter->key != level)
    {
        bool isBigger = iter->key < level;
        if(insert)
        {
            iter->scores[score]++;
            iter->player_weight++;
            iter->weighted_sum += level;
        }
        else
        {
            iter->scores[score]--;
            iter->player_weight--;
            iter->weighted_sum -= level;
        }
        if(isBigger)
        {
            iter = iter->right_son;
        }
        else{
            iter = iter->left_son;
        }
    }
    if(iter != nullptr)
    {
        if(insert)
        {
            iter->scores[score]++;
            iter->player_weight++;
            iter->weighted_sum += level;
        }
        else
        {
            iter->scores[score]--;
            iter->player_weight--;
            iter->weighted_sum -= level;
        }
    }

}


int AVLRanktree::getNumOfPlayersInTree()
{
    return getAllPlayersWeight(root)+num_of_zero_level_players;
}




int AVLRanktree::getLevelOfPlayerM(int m) 
{
    if(m > getNumOfPlayersInTree())
    {
        return -1;
    }
    int helper = m;
    Node iter = root;
    while(iter)
    {
        if(helper <= getAllPlayersWeight(iter->right_son))
        {
            iter = iter->right_son;
        }
        else if(helper <= getAllPlayersWeight(iter->right_son) + getNumPlayers(iter))
        {
            return iter->key;
        }
        else{
            helper -= (getAllPlayersWeight(iter->right_son)+ getNumPlayers(iter));
            iter = iter->left_son;
        }
    }
    return 0;
    
}


int AVLRanktree::getNumOfPlayersInRange(int level1, int level2)
{
    /*if(level1 == 0 && level2 == 0){
        return num_of_zero_level_players;
    }*/
    int sum = numOfPlayersWithLowerLevel(level2) - numOfPlayersWithLowerLevel(level1);
    Node level1_node = doesKeyExists(level1);
    if(level1_node)
    {
        sum += getNumPlayers(level1_node);
    }
    else if(level1 == 0){
        sum += num_of_zero_level_players;
    }
    return sum;
}

int AVLRanktree::getNumOfPlayersWithScoreInRange(int level1, int level2, int score)
{
    /*if(level1 == 0 && level2 == 0){
        return zero_level_scores[score];
    }*/
    int sum = numOfPlayesWithScoreAndLowerLevel(level2, score) - numOfPlayesWithScoreAndLowerLevel(level1, score);
    Node level1_node = doesKeyExists(level1);
    if(level1_node)
    {
        sum += getNumPlayersWithScore(level1_node, score);
    }
    else if(level1 == 0){
        sum += zero_level_scores[score];
    }
    return sum;
}