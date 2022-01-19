#include "PlayersManager.h"

using namespace DS;

void PlayersManager::MergeGroups(int GroupID1, int GroupID2){
    groups.Union(GroupID1, GroupID2);
}

void PlayersManager::AddPlayer(int PlayerID, int GroupID, int score){
    shared_ptr<Player> player_to_find = players.find(PlayerID);
    if(player_to_find != nullptr){
        throw Failure();
    }
    if(score == 64)
    {
        printf("");

    }
    shared_ptr<Player> player_to_insert(new Player(PlayerID, GroupID, score));
    players.insert(player_to_insert);
    Group* group_to_find = groups.Find(GroupID);
    group_to_find->insertPlayer(score);
    all_players_levels_tree.addPlayer(0, score);
}

void PlayersManager::RemovePlayer(int PlayerID){
    shared_ptr<Player> player_to_remove = players.find(PlayerID);
    if(player_to_remove == nullptr){
        throw Failure();
    }
    Group* group_to_find = groups.Find(player_to_remove->getGroupID());
    if(player_to_remove->getPlayerLevel()==64)
        printf("");
    group_to_find->removePlayer(player_to_remove->getPlayerLevel(),
         player_to_remove->getPlayerScore());
    all_players_levels_tree.removePlayer(player_to_remove->getPlayerLevel(),
         player_to_remove->getPlayerScore());
    players.remove(PlayerID);
}

void PlayersManager::IncreasePlayerIDLevel(int PlayerID, int LevelIncrease){
    shared_ptr<Player> player_to_increase = players.find(PlayerID);
    if(player_to_increase == nullptr){
        throw Failure();
    }
    if(player_to_increase->getPlayerScore() == 64)
        printf("");
    int new_level = player_to_increase->getPlayerLevel() + LevelIncrease;
    Group* group_to_find = groups.Find(player_to_increase->getGroupID());
    group_to_find->increaseLevel(player_to_increase->getPlayerLevel(),
        new_level, player_to_increase->getPlayerScore());
    all_players_levels_tree.removePlayer(player_to_increase->getPlayerLevel(),
        player_to_increase->getPlayerScore());
    all_players_levels_tree.addPlayer(new_level,
        player_to_increase->getPlayerScore());
    player_to_increase->setPlayerLevel(new_level);
}

void PlayersManager::ChangePlayerIDScore(int PlayerID, int NewScore){
    shared_ptr<Player> player_to_change = players.find(PlayerID);
    if(player_to_change == nullptr){
        throw Failure();
    }
    if(NewScore == 64){
        printf("");
    }
    if(player_to_change->getPlayerScore() == 64){
        printf("");
    }
    Group* group_to_find = groups.Find(player_to_change->getGroupID());

    shared_ptr<AVLRanktree> tree_of_group = group_to_find->getLevelsTree();
    tree_of_group->removePlayer(player_to_change->getPlayerLevel(),
         player_to_change->getPlayerScore());
    tree_of_group->addPlayer(player_to_change->getPlayerLevel(), NewScore);

    all_players_levels_tree.removePlayer(player_to_change->getPlayerLevel(),
        player_to_change->getPlayerScore());
    all_players_levels_tree.addPlayer(player_to_change->getPlayerLevel(), NewScore);
    player_to_change->setPlayerScore(NewScore);
}

void PlayersManager::GetPercentOfPlayersWithScoreInBounds(int GroupID, int score,
                     int lowerLevel, int higherLevel, double* players){
    if(higherLevel < lowerLevel){
        throw Failure();
    }
    if(GroupID == 0){
        int total_players_in_range =
             all_players_levels_tree.getNumOfPlayersInRange(lowerLevel, higherLevel);
        if(total_players_in_range == 0){
            throw Failure();
        }
        int num_of_players_with_score_in_range = 
            all_players_levels_tree.getNumOfPlayersWithScoreInRange(lowerLevel, higherLevel, score);
        *players = (double)(((double)num_of_players_with_score_in_range/
            (double)total_players_in_range)*100.0);
    }
    else{
        Group* group_to_find = groups.Find(GroupID);
        int players_of_group_in_range = 
            group_to_find->getLevelsTree()->getNumOfPlayersInRange(lowerLevel, higherLevel);
        if(players_of_group_in_range == 0){
            throw Failure();
        }
        int num_of_players_in_group_with_score_in_range = 
            group_to_find->getLevelsTree()->getNumOfPlayersWithScoreInRange(lowerLevel, higherLevel, score);
        *players = (double)(((double)num_of_players_in_group_with_score_in_range/
            (double)players_of_group_in_range)*100.0);
    }
}

/*static int findWeightOfMFirstPlayers(AVLnode* node,
                int num_of_players_in_m_th_player_level, int level_of_m_th_player){
    int sum = 0;
    if(node == nullptr){
        return 0;
    }
    sum += findWeightOfMFirstPlayers(node->right_son, 
        num_of_players_in_m_th_player_level, level_of_m_th_player);
    if(node->key == level_of_m_th_player){
        sum += num_of_players_in_m_th_player_level*level_of_m_th_player;
    }
    else if(node->key > level_of_m_th_player){
        sum += getNumPlayers(node)*(node->key);
    }
    sum += findWeightOfMFirstPlayers(node->left_son, 
        num_of_players_in_m_th_player_level, level_of_m_th_player);
    return sum;
}*/

static int findWeightOfMFirstPlayers(AVLRanktree* tree, AVLnode* node,
                int m, int level_of_m_th_player){
    int result = getWeightedSum(tree->getRoot());
    if(level_of_m_th_player == 0)
        return result;
    int diff = 0;
    result -= tree->sumOfPlayersWithLowerLevel(level_of_m_th_player);
     diff = getAllPlayersWeight(tree->getRoot())+tree->getNumOfZeroLevelPlayers() - tree->numOfPlayersWithLowerLevel(level_of_m_th_player);

    diff = m-diff;

    result += node->key*diff;
    return result;
//


}

void PlayersManager::AverageHighestPlayerLevelByGroup(int GroupID, int m, double* level){
    if(GroupID == 0){
        int level_of_m_th_player = all_players_levels_tree.getLevelOfPlayerM(m);
        if(level_of_m_th_player == -1){
            throw Failure();
        }
        double weighted_sum_of_first_m_players = 
            (double)findWeightOfMFirstPlayers(&all_players_levels_tree,
             all_players_levels_tree.doesKeyExists(level_of_m_th_player), m, level_of_m_th_player);
        *level = weighted_sum_of_first_m_players/((double)m);
    }
    else{
        Group* group_to_find = groups.Find(GroupID);
        int level_of_m_th_player_in_group = group_to_find->getLevelsTree()->getLevelOfPlayerM(m);
        if(level_of_m_th_player_in_group == -1){
            throw Failure();
        }
        double weighted_sum_of_first_m_players_in_group = 
            findWeightOfMFirstPlayers(group_to_find->getLevelsTree().get(),
        group_to_find->getLevelsTree()->doesKeyExists(level_of_m_th_player_in_group), m, level_of_m_th_player_in_group);
        *level = weighted_sum_of_first_m_players_in_group/((double)m);
    }
}

static void setLowerAndUpperBoundOfMFirstPlayersWithScore(AVLnode* node, int score, 
            int m, int level_of_m_th_player, int* LowerBoundPlayers, int* HigherBoundPlayers){
    AVLnode* iter = node;
    int current_player_weight_with_score = 0;
    if(iter != nullptr){
        current_player_weight_with_score = iter->scores[score];
    }
    while(iter){
        if(iter->key < level_of_m_th_player){
            current_player_weight_with_score = iter->right_son->scores[score];
            iter = iter->right_son;
        }
        else if(iter->key > level_of_m_th_player){
            iter = iter->left_son;
        }
        else{
            if(iter->left_son){
                current_player_weight_with_score -= iter->left_son->scores[score];
            }
            int num_of_players_in_m_th_player_level = getNumPlayers(iter);
            int num_of_players_in_m_th_player_level_with_score = getNumPlayersWithScore(iter, score);
            int num_of_players_with_score_without_m_th_player_level =
                 current_player_weight_with_score - num_of_players_in_m_th_player_level_with_score;
            if(current_player_weight_with_score <= m){
                *HigherBoundPlayers = current_player_weight_with_score;
            }
            else{
                *HigherBoundPlayers = m;
            }
            if(m - num_of_players_with_score_without_m_th_player_level <=
                 num_of_players_in_m_th_player_level - num_of_players_in_m_th_player_level_with_score){
                *LowerBoundPlayers = num_of_players_with_score_without_m_th_player_level;
            }
            else{
                *LowerBoundPlayers = m - 
                    (num_of_players_in_m_th_player_level - num_of_players_in_m_th_player_level_with_score);
            }
        }
    }
}

void PlayersManager::GetPlayersBound(int GroupID, int score, int m,
                                        int * LowerBoundPlayers, int * HigherBoundPlayers){
    if(GroupID == 0){
        int level_of_m_th_player = all_players_levels_tree.getLevelOfPlayerM(m);
        if(level_of_m_th_player == -1){
            throw Failure();
        }
        setLowerAndUpperBoundOfMFirstPlayersWithScore(all_players_levels_tree.getRoot(),
            score, m, level_of_m_th_player, LowerBoundPlayers, HigherBoundPlayers);
    }
    else{
        Group* group_to_find = groups.Find(GroupID);
        int level_of_m_th_player_in_group = group_to_find->getLevelsTree()->getLevelOfPlayerM(m);
        if(level_of_m_th_player_in_group == -1){
            throw Failure();
        }
        setLowerAndUpperBoundOfMFirstPlayersWithScore(group_to_find->getLevelsTree()->getRoot(),
            score, m, level_of_m_th_player_in_group, LowerBoundPlayers, HigherBoundPlayers);
    }
}


