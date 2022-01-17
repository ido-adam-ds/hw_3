#include "DynamicHashTable.h"

using namespace DS;

int DynamicHashTable::HashFunction(int PlayerID) const{
    return PlayerID % total_size;
}

shared_ptr<Player> DynamicHashTable::find(int PlayerID) const{
    int index = HashFunction(PlayerID);
    LinkedListNode<shared_ptr<Player>>* node_to_find = array[index].getHead();
    while(node_to_find != nullptr){
        if(node_to_find->data->getPlayerID() == PlayerID){
            break;
        }
        node_to_find = node_to_find->next;
    }
    if(node_to_find == nullptr){
        return nullptr;
    }
    return node_to_find->data;
}

void DynamicHashTable::moveToNewArray(int new_size){
    int old_size = total_size;
    total_size = new_size;
    LinkedList<shared_ptr<Player>>* new_array = new LinkedList<shared_ptr<Player>>[new_size];
    LinkedListNode<shared_ptr<Player>>* ptr;
    for(int i = 0; i < old_size; i++){
        ptr = array[i].getHead();
        while(ptr != nullptr){
            new_array[HashFunction(ptr->data->getPlayerID())].insertNewNode(ptr->data);
            ptr = ptr->next;
        }
    }
    delete[] array;
    array = new_array;
}

void DynamicHashTable::insert(shared_ptr<Player> player_to_insert){
    shared_ptr<Player> player_to_find = find(player_to_insert->getPlayerID());
    if(player_to_find != nullptr){
        throw Failure();
    }
    if(occupied == total_size){ 
        moveToNewArray(total_size*expand_rate);
    }
    array[HashFunction(player_to_insert->getPlayerID())].insertNewNode(player_to_insert);
    occupied++;
}

void DynamicHashTable::remove(int PlayerID){
    shared_ptr<Player> player_to_find = find(PlayerID);
    if(player_to_find == nullptr){
        throw Failure();
    }
    if(occupied == total_size/(shrink_rate*shrink_rate) && total_size > INITIAL_SIZE){
        moveToNewArray(total_size/shrink_rate);
    }
    LinkedListNode<shared_ptr<Player>>* ptr = array[HashFunction(PlayerID)].getHead();
    while(ptr != nullptr){
        if(ptr->data->getPlayerID() == PlayerID){
            array[HashFunction(PlayerID)].deleteNode(ptr);
            break;
        }
        ptr = ptr -> next;
    }
    occupied--;
}