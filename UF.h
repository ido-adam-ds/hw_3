//
// Created by Iddo Petrank on 09/01/2022.
//

#ifndef WET2_UF_H
#define WET2_UF_H

#include "Group.h"


namespace DS {



    template<int K, int SCALE>
    class UF {
        shared_ptr<Group<SCALE>> groups[K];

        UF()
        {
            for (int i = 0; i<K; i++)
            {
                groups[i] = new Group<SCALE>(i);
            }
        }

        std::shared_ptr<Group<SCALE>> Find(int Group_id);
        void Union(int id1, int id2);
    };

    template<int K, int SCALE>
    std::shared_ptr<Group<SCALE>> UF<K, SCALE>::Find(int Group_id)
    {
        std::shared_ptr<Group<SCALE>> iter = groups[Group_id];
        while(iter->father)
        {
            iter = iter->father;
        }
        return iter;
    }

    template<int K, int SCALE>
    void UF<K, SCALE>::Union(int id1, int id2)
    {
        std::shared_ptr<Group<SCALE>> group1 = Find(id1);
        std::shared_ptr<Group<SCALE>> group2 = Find(id2);
        //merge trees
        if(group1->group_size < group2->group_size)
        {
            group1->father = group2;
            //insert merged tree
        } else{
            group2->father = group1;
            //insert merged tree
        }
    }



}


#endif //WET2_UF_H
