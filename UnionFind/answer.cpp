/****
对于不记录路径的点之间的关系可以用并查集，记录路径就用DFS。


关键函数：
findfather
union
init
其中find中增加路径压缩可以优化复杂度


***/

#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <utility>
#include <set>

using namespace std;


//#define DBG
#ifdef DBG
    stringstream CIN(
"4 "
"3 10 1 2 "
"2 3 4 "
"4 1 5 7 8 "
"3 9 6 4 "
"2 "
"10 5 "
"3 7 "
    );
#else
    #define CIN cin
#endif

#define XXMAX 10000

int N,Q;

set<int> isroot;
vector<int> father;


void init(){
    father.resize(XXMAX+1);
    for(int i=1;i<=XXMAX;i++){
        father[i] = i;
    }
}

int findfather(int a){
    int tmp = a;
    while(father[a] != a){
        a = father[a];
    }

    // compress path
    while(father[tmp] != tmp){
        int tmptmp = tmp;
        tmp = father[tmp];
        father[tmptmp] = a;
    }

    return a;

}

void union_(int a,int b){
    int fa = findfather(a);
    int fb = findfather(b);
    if(fa != fb){
        father[fa] = fb;
    }
}


vector<string> answers;
int maxv = 0;
vector< pair<int,int> > querys;
void readandwork(){
    CIN >> N;

    for(int i=0;i<N;i++){
        int cnt,first;
        CIN >> cnt >> first;
        maxv = max(maxv,first);
        for(int j=1;j<cnt;j++){
            int second;
            CIN >> second;
            maxv = max(maxv,second);
            union_(first,second);
        }

    }
    CIN >> Q;
    for(int i=0;i<Q;i++){
        int a,b;
        CIN >>a >>b;
        querys.push_back(pair<int,int>(a,b));
    }

    for(int i=1;i<=maxv;i++){
        isroot.insert(findfather(i));
    }

    for(int i=0;i<Q;i++){
        int a=querys[i].first;
        int b=querys[i].second;
        if(findfather(a) == findfather(b)){
            answers.push_back("Yes");
        }else{
            answers.push_back("No");
        }
    }


}

void printanswer(){
    cout << isroot.size() << " " << maxv ;

    for(int i=0;i<answers.size();i++){
        cout << endl << answers[i] ;
    }

}

int main()
{
    init();
    readandwork();
    printanswer();
    return 0;
}

