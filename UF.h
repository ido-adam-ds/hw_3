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
        UF(int k): groups(new shared_ptr<Group>[k]){
            for(int i = 0; i < k; i++){
                shared_ptr<Group> group_to_insert(new Group(i));
                groups[i] = group_to_insert;
            }
        }
        Group* Find(int GroupID) const;
        void Union(int id1, int id2);
    };

    Group* UF::Find(int GroupID) const{
        Group* iter = groups[GroupID-1].get();
        while(iter->getFather())
        {
            iter = iter->getFather();
        }

        //Shrinking paths
        Group* root = iter;
        iter = groups[GroupID-1].get();
        Group* next_group;
        while(iter->getFather()){
            next_group = iter->getFather();
            iter->setFather(root);
            iter = next_group;
        }

        return iter;
    }

    void UF::Union(int id1, int id2)
    {
        Group* group1 = Find(id1);
        Group* group2 = Find(id2);
        if(group1->getSize() < group2->getSize())
        {
            group1->setFather(group2);
            group2->mergeLevelsTreeWithAnotherGroup(group1);
            group1->setLevelsTree(nullptr);
        } else{
            group2->setFather(group1);
            group1->mergeLevelsTreeWithAnotherGroup(group2);
            group2->setLevelsTree(nullptr);
        }
    }

}

#endif