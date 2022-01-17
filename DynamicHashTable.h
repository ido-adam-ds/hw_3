#ifndef DYNAMIC_HASH_TABLE
#define DYNAMIC_HASH_TABLE

#include "Player.h"
#include "LinkedList.h"
#include <memory>

#define INITIAL_SIZE 4

using std::shared_ptr;

namespace DS{

    class DynamicHashTable{

    private:

        LinkedList<shared_ptr<Player>>* array;

        const int expand_rate = 2;
        const int shrink_rate = 2;

        int total_size;
        int occupied;

        void moveToNewArray(int new_size);

    public:

        DynamicHashTable(): 
            array(new LinkedList<shared_ptr<Player>>[INITIAL_SIZE]),
                total_size(INITIAL_SIZE), occupied(0){}
        ~DynamicHashTable(){
            delete[] array;
        }

        DynamicHashTable(const DynamicHashTable& other) = delete;
        DynamicHashTable& operator=(const DynamicHashTable& other) = delete;

        shared_ptr<Player> find(int PlayerID) const;

        void insert(shared_ptr<Player> player_to_insert);

        void remove(int PlayerID);

        int HashFunction(int PlayerID) const;

        int getSize() const{
            return total_size;
        }

        /*int getOccupied() const{
            return occupied;
        }

        LinkedList<shared_ptr<Player>>* getArray() const{
            return array;
        }*/

    };

}

#endif