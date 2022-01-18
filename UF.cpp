#include "UF.h"

using namespace DS;

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
    if(id1 == 7 || id2 == 7)
    {
        printf("");
    }
    Group* group1 = Find(id1);
    Group* group2 = Find(id2);
    if(group1 == group2){
        return;
    }
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