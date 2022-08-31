
#include <math.h>
#include <iostream>
#include <stdio.h>

#include "vmath.h"

using namespace std;

float VMath::absx(const float val)
{
    float tmpval = val<0?(-1*val):val;
    return tmpval;
}

float VMath::selfEntropy( map<int,set<int>* > &cluster,const int num)
{
    float entropy = 0;
    float size,divid;
    map<int, set<int>* >::iterator it;
    for(it = cluster.begin(); it != cluster.end(); it++)
    {
        size = (*it->second).size();
        divid = size/num;
        //entropy += divid*LG(divid,2.0); //old version
        entropy += divid*LG(size,2.0); //new version
    }

    /** old version*************
    entropy = entropy/LG(num,2.0);
    entropy = entropy*(-1.0);
    /************************/

    entropy = 1.0 - entropy/LG(num,2.0); //new version

    return entropy;
}

float VMath::selfEntropy(map<int,set<int>*> &cluster,const int num,int &cmpct_size,const int thres)
{
    float entropy = 0;
    float size,divid;
    bool SPARSE = true;
    map<int, set<int>* >::iterator it;
    float nwnum = 0;
    cmpct_size = 0;
    //cout<<"bug 1\n";
    for(it = cluster.begin(); it != cluster.end(); it++)
    {
        size = (*it->second).size();
        if(size > thres)
        {
            nwnum = nwnum + size;
            SPARSE = false;
            cmpct_size++;
        }

        divid = size/num; //prepare for sparse case
        //entropy += divid*LG(divid,2.0); //prepare for sparse case
        entropy += divid*LG(size,2.0);

    }

    if(SPARSE)
    {
        //entropy = entropy/LG(num,2.0);  old version
        //entropy = entropy*(-1.0);             old version
        entropy = entropy/LG(nwnum,2.0);  //new version
        entropy = 1 - entropy;                      //new version
        cmpct_size = cluster.size();
        return entropy;
    }

    entropy = 0;
    for(it = cluster.begin(); it != cluster.end(); it++)
    {
        size = (*it->second).size();
        if(size > thres)
        {
            divid = size/nwnum;
            //entropy += divid*LG(divid,2.0);  //old version
            entropy += divid*LG(size,2.0);
            //new version
        }
    }
    //cout<<"hellow"<<endl;
    entropy = entropy/LG(nwnum,2.0);

    //entropy =  entropy*(-1.0);   //old version
    entropy = 1 - entropy;

    return entropy;
}

int VMath::interSection(const set<int>* seta,const set<int>* setb)
{
    int i = 0;
    set<int>::iterator it;
    for(it = seta->begin(); it != seta->end(); it++)
   {
       if(setb->find(*it) != setb->end())
       {
           i++;
       }
    }
    return i;
}

/***********************************************************************
I in fact ignore this implementation, I used to  use it. However, I found it is
 unreasonable in some cases and its performace is slightly worse than above
 impletation. So I reserve it here for comparison, but not employ it in current
 program
/************************************************************************/

float VMath::BiEntropy(map<int,set<int>* > &main_cluster,const int num, map<int,set<int>* > &miner_cluster,float &max_share)
{
    float entropy = 0,divid;
    float inter_share;
    map<int,set<int>* >::iterator main_it, minn_it;
    set<int>* main_set;
    set<int>* minn_set;
    max_share = 0;
    int num_grp, max_grp,divider;

   if(main_cluster.size() < miner_cluster.size())
   {
       num_grp = main_cluster.size();
       max_grp = miner_cluster.size();
   }else
   {
       num_grp = miner_cluster.size();
       max_grp = main_cluster.size();
   }

    if(num_grp == 0)
    return 1.0;

     entropy = 0.0;
     max_share = 0;
     float bigone;
     for(minn_it = miner_cluster.begin(); minn_it != miner_cluster.end(); minn_it++)
     {
           minn_set = minn_it->second;
           //max_share = 0;
           bigone = 0;
           for(main_it = main_cluster.begin(); main_it != main_cluster.end(); main_it++)
           {
               main_set = main_it->second;
               inter_share = VMath::interSection(main_set,minn_set);

               if(max_share < inter_share)
                {
                    max_share = inter_share;
                }

                if(bigone <inter_share)
                {
                    bigone = inter_share;
                    divider = main_set->size();
                }
        }//end inner for

         if(bigone >0)
        {
            //cout<<"\t"<<max_divid<<endl;
            divid = bigone/divider;
            entropy += divid*lgx(divid,2.0);
        }

    }//end outer for

    //max_share = biggest_card;

    if(main_cluster.size() > 1)
    {
        entropy = -entropy/(num*lgx(main_cluster.size(),2.0));
    }else
    {
        entropy = -entropy/num;
    }
    //entropy = entropy/;
    //printf("\t%5.3f\n",entropy);
    //cout<<"\t"<<entropy<<endl;
   return entropy;
}

float VMath::interEntropy(map<int,set<int>* > &main_cluster,const int num, map<int,set<int>* > &miner_cluster,float &max_share)
{
    float entropy = 0,divid,sgl_entropy;
    float inter_share;
    map<int,set<int>* >::iterator main_it, minn_it;
    set<int>* main_set;
    set<int>* minn_set;
    max_share = 0;
    int counter = 0;
    float minner,sum = 0;
    float divider;
    int num_grp, max_grp;

   if(main_cluster.size() < miner_cluster.size())
   {
       num_grp = main_cluster.size();
       max_grp = miner_cluster.size();
   }else
   {
       num_grp = miner_cluster.size();
       max_grp = main_cluster.size();
   }

    if(num_grp == 0)
    return 1.0;

    entropy = 0;
    max_share = 0;
    for(minn_it = miner_cluster.begin(); minn_it != miner_cluster.end(); minn_it++)
    {
         minn_set = minn_it->second;
         divider = minn_set->size();
         sgl_entropy = 0;

         for(main_it = main_cluster.begin(); main_it != main_cluster.end(); main_it++)
         {
               main_set = main_it->second;
               inter_share = VMath::interSection(main_set,minn_set);
               if(inter_share > 0)
               {
                   divid = inter_share/divider;
                   //cout<<inter_share<<"::";
                   sgl_entropy += divid*VMath::lgx(divid,2.0);
               }
               if(inter_share >= 3)
               {
                    //if(max_share < inter_share)
                   max_share = inter_share + max_share;
               }
        }//end inner for
        //cout<<endl;
        entropy += (-1)*(divider/num)*sgl_entropy;

    }//end outer for

   return entropy;
}

void VMath::convert_hex(int number, int* hex_array)
{
      for(int i=0; i<4; i++)
      {
         hex_array[i] = number%256;
         number = number/256;
      }
}

void VMath::test()
{
    int hi = 8;
    set<int> seta,setb;
    set<int>::iterator it;
    int i,j = 0;
    for(i = 0; i <10;i += 2,j++)
    {
        seta.insert(i);
        setb.insert(j);
    }

    setb.insert(8);
    int inter = VMath::interSection(&seta,&setb);
    for(it = seta.begin(); it != seta.end(); it++)
    printf("%d ",*it);
    printf("\n");

    for(it = setb.begin(); it != setb.end(); it++)
    printf("%d ",*it);
    printf("\n");

    printf("inter: %d",inter);

    return ;
}
