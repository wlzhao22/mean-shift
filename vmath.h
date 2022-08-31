#ifndef VMATH_H
#define VMATH_H

#include <set>
#include <map>
#include <math.h>

#define LG(a,b) (log(a)/log(b))

using namespace std;

class VMath {

public:
    static float selfEntropy(map<int,set<int>* > &cluster,const int num);
    static float selfEntropy(map<int,set<int>*> &cluster,const int num,int &cmpct_size,const int thres);
    static int interSection(const set<int>* seta,const set<int>* setb);
    static float lgx(const float a,const float b)
    {   return log(a)/log(b);}
    static int Sign(float val)
    {
        if(val >= 0)
        {
            return 1;
        }else{
            return -1;
        }
    }
    static float absx(const float val);
    static float interEntropy(map<int,set<int>* > &main_cluster,const int num, map<int,set<int>* > &minner_cluster,float &max_share);
    static float BiEntropy(map<int,set<int>* > &main_cluster,const int num, map<int,set<int>* > &miner_cluster,float &max_share);
    static void convert_hex(int number, int* hex_array);
    static void test();
};
#endif
