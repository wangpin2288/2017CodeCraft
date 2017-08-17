#ifndef _OWN_H_
#define _OWN_H_

#include "deploy.h"
#include <vector>
#include <string>
#include <chrono>
using namespace std;
using namespace std::chrono;


#define MAXINT 0x3f3f3f3f
#define N 1010
#define M 40400

struct Edge{
    int a, b, c=0, p, c_f=0;
    int next=-1;
    void setEG(int _a, int _b, int _c, int _p ){
        a = _a;b = _b; c = _c; p = _p;
        c_f = _c;
    }
    
    Edge& operator=(const Edge& EG){
		a = EG.a;
		b = EG.b;
		c_f = c = EG.c;
		p = EG.p;
		return *this;
	}
    
};

struct Node
{
    int nodeId;
    int cost;
    Node(int id,int cost):nodeId(id),cost(cost){}
    friend bool operator <(const Node& t,const Node& other)
    {
        return t.cost>other.cost;
    }
};

struct Indivi{
	string Gene;
	double fit=0.0;
	Indivi& operator=(const Indivi& ind){
		Gene = ind.Gene;
		fit = ind.fit;
		return *this;
	}
};

extern vector<int> ran;
extern Edge EG[];
extern int first[];
extern int V, X, Y;
extern vector<int> yVec;
extern int need[];
extern int id[];
extern vector<vector<int>> result;
extern int servicePrice;
extern int pred[];
extern int dist[];
extern bool marked[];
extern int cnt;
extern int f0;
extern int v0;
extern int TIME;
extern bool flag;

extern void addE(int a, int b, int c, int p);
extern int  getRandomNum(int n);
extern bool spfa();
extern void minCostFlow(vector<int>& xVec,int& minCost,bool& success,bool flag);
extern void getRandomVec(int n, vector<int>& xVec);
extern void MCF(bool& success,int& minCost);
extern void deleteServer(vector<int>& xVec);
extern void slope(vector<int>& vec);
extern void slope2(vector<int>& vec);
extern void slope3(vector<int>& vec);
extern int push_flow(int u, int leaveFlow);
extern void writeResult(vector<vector<int>>& res);













#endif
