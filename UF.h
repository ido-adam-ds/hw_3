#ifndef UF_H
#define UF_H

#include "Group.h"
#include <memory>
using std::shared_ptr;

namespace DS{

    class UF{
    private:
        int k;
        shared_ptr<Group>* groups;
    public:
        UF(int k): k(k), groups(new shared_ptr<Group>[k]){
            for(int i = 1; i < k+1; i++){
                shared_ptr<Group> group_to_insert(new Group(i));
                groups[i - 1] = group_to_insert;
            }
        }
        ~UF(){
            delete[] groups;
        }
        Group* Find(int GroupID) const;
        void Union(int id1, int id2);
    };


}

#endif