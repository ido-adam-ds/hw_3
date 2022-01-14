//
// Created by Iddo Petrank on 09/01/2022.
//

#ifndef WET2_UF_H
#define WET2_UF_H

#include "Group.h"


namespace DS {



    
    class UF {
        Group** groups;
        int num_groups;

    public:
        UF(int k): num_groups(k)
        {
            groups = (Group**)malloc(sizeof(Group*)*k);
            for (int i = 0; i<k; i++)
            {
                Group* new_group = new Group(i+1);
                groups[i] = new_group;
            }
        }

        ~UF()
        {
            for (int i = 0; i<num_groups; i++)
            {
                delete groups[i];
            }
            free(groups);
        }

        Group* Find(int Group_id);
        void Union(int id1, int id2);
    };

    
    Group* UF::Find(int Group_id)
    {
        Group* iter = groups[Group_id];
        Group* root;
        while(iter->getFather())
        {
            iter = iter->getFather();
        }
        root = iter;
        iter = groups[Group_id-1];
        while(iter->getFather() != root)
        {
            iter->setFather(root);
        }
        return iter;
    }

    
    void UF::Union(int id1, int id2)
    {
        Group* group1 = Find(id1);
        Group* group2 = Find(id2);
        //merge trees
        if(group1->getGroupSize() < group2->getGroupSize())
        {
            group1->setFather(group2);
            //insert merged tree
        } else{
            group2->setFather(group1);
            //insert merged tree
        }
    }



}


#endif //WET2_UF_H
