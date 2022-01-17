#include "library2.h"
#include "PlayersManager.h"
#include "Exceptions.h"

#ifndef __cplusplus
#define __cplusplus
#endif

using namespace DS;

void* Init(int k, int scale){
    if(k <= 0 || scale >= MAX_SIZE || scale <= 0){
        return NULL;
    }
    PlayersManager* DS;
    try{
        DS = new PlayersManager(k, scale);
    }
    catch(std::bad_alloc& ba){
        return NULL;
    }
    return (void*)DS;
}

StatusType MergeGroups(void* DS, int GroupID1, int GroupID2){
    if(DS == NULL || GroupID1 <= 0 || GroupID2 <= 0 ||
         (DS != NULL && ((PlayersManager*)DS)->getNumOfGroups() < GroupID1) ||
            (DS != NULL && ((PlayersManager*)DS)->getNumOfGroups() < GroupID2)){
        return StatusType::INVALID_INPUT;
    }
    try{
        ((PlayersManager*)DS)->MergeGroups(GroupID1, GroupID2);
    }
    catch(std::bad_alloc& ba){
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

StatusType AddPlayer(void* DS, int PlayerID, int GroupID, int score){
    if(DS == NULL || (DS != NULL && ((PlayersManager*)DS)->getNumOfGroups() < GroupID) ||
        GroupID <= 0 || PlayerID <= 0 || score <= 0 ||
            (DS != NULL && ((PlayersManager*)DS)->getScale() < score)){
        return StatusType::INVALID_INPUT;
    }
    try{
        ((PlayersManager*)DS)->AddPlayer(PlayerID, GroupID, score);
    }
    catch(std::bad_alloc& ba){
        return StatusType::ALLOCATION_ERROR;
    }
    catch(Exception& e){
        return e.getError();
    }
    return StatusType::SUCCESS;
}

StatusType RemovePlayer(void* DS, int PlayerID){
    if(DS == NULL || PlayerID <= 0){
        return StatusType::INVALID_INPUT;
    }
    try{
        ((PlayersManager*)DS)->RemovePlayer(PlayerID);
    }
    catch(Exception& e){
        return e.getError();
    }
    return StatusType::SUCCESS;
}

StatusType IncreasePlayerIDLevel(void* DS, int PlayerID, int LevelIncrease){
    if(DS == NULL || PlayerID <= 0 || LevelIncrease <= 0){
        return StatusType::INVALID_INPUT;
    }
    try{
        ((PlayersManager*)DS)->IncreasePlayerIDLevel(PlayerID, LevelIncrease);
    }
    catch(std::bad_alloc& ba){
        return StatusType::ALLOCATION_ERROR;
    }
    catch(Exception& e){
        return e.getError();
    }
    return StatusType::SUCCESS;
}

StatusType ChangePlayerIDScore(void* DS, int PlayerID, int NewScore){
    if(DS == NULL || PlayerID <= 0 || NewScore <= 0 ||
     (DS != NULL && ((PlayersManager*)DS)->getScale() < NewScore)){
        return StatusType::INVALID_INPUT;
    }
    try{
        ((PlayersManager*)DS)->ChangePlayerIDScore(PlayerID, NewScore);
    }
    catch(std::bad_alloc& ba){
        return StatusType::ALLOCATION_ERROR;
    }
    catch(Exception& e){
        return e.getError();
    }
    return StatusType::SUCCESS;
}

StatusType GetPercentOfPlayersWithScoreInBounds(void* DS, int GroupID, int score,
     int LowerLevel, int higherLevel, double* players){
    if(DS == NULL || players == NULL || GroupID < 0 ||
        (DS != NULL && ((PlayersManager*)DS)->getNumOfGroups() < GroupID)){
        return StatusType::INVALID_INPUT;
    }
    try{
        ((PlayersManager*)DS)->GetPercentOfPlayersWithScoreInBounds(GroupID,
                                         score, LowerLevel, higherLevel, players);
    }
    catch(std::bad_alloc& ba){
        return StatusType::ALLOCATION_ERROR;
    }
    catch(Exception& e){
        return e.getError();
    }
    return StatusType::SUCCESS;
}

StatusType AverageHighestPlayerLevelByGroup(void* DS, int GroupID, int m, double* avgLevel){
    if(DS == NULL || avgLevel == NULL || m <= 0 || GroupID < 0 ||
        (DS != NULL && ((PlayersManager*)DS)->getNumOfGroups() < GroupID)){
        return StatusType::INVALID_INPUT;
    }
    try{
        ((PlayersManager*)DS)->AverageHighestPlayerLevelByGroup(GroupID, m, avgLevel);
    }
    catch(std::bad_alloc& ba){
        return StatusType::ALLOCATION_ERROR;
    }
    catch(Exception& e){
        return e.getError();
    }
    return StatusType::SUCCESS;
}

StatusType GetPlayersBound(void* DS, int GroupID, int score, int m,
                     int* LowerBoundPlayers, int* HigherBoundPlayers){
    if(DS == NULL || LowerBoundPlayers == NULL || HigherBoundPlayers == NULL ||
        m < 0 || GroupID < 0 || (DS != NULL && ((PlayersManager*)DS)->getNumOfGroups() < GroupID) ||
            score <= 0 || (DS != NULL && ((PlayersManager*)DS)->getScale() < score)){
        return StatusType::INVALID_INPUT;
    }
    try{
        ((PlayersManager*)DS)->GetPlayersBound(GroupID, score, m, LowerBoundPlayers, HigherBoundPlayers);
    }
    catch(std::bad_alloc& ba){
        return StatusType::ALLOCATION_ERROR;
    }
    catch(Exception& e){
        return e.getError();
    }
    return StatusType::SUCCESS;
}

void Quit(void** DS){
    delete (PlayersManager*)*DS;
    *DS = NULL;
}