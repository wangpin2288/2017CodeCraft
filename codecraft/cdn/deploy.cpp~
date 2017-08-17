#include "deploy.h"
#include <stdio.h>

//你要完成的功能总入口
void deploy_server(char * topo[MAX_EDGE_NUM], int line_num,char * filename)
{
	int node=0;
	int E,consumeNum;
	for(int i=0;i<N;i++) first[i]=-1;
	v0=0;cnt=0;
	stringstream ss(topo[node++]);
	ss>>V>>E>>consumeNum;
	V+=2;
	X=V-2;
	Y=V-1;

	node++;
	ss.clear();
	ss.str(topo[node++]);
	ss>>servicePrice;

	node++;
	for(int i=0;i<E;i++){
		ss.clear();
		ss.str(topo[node++]);
		int a,b,capacity,price;
		ss>>a>>b>>capacity>>price;
		addE(a, b, capacity, price); 
		addE(b, a, capacity, price);
	}

	node++;

	for(int i=0;i<consumeNum;i++){
		ss.clear();
		ss.str(topo[node++]);
		int a,b,capacity;
		ss>>a>>b>>capacity;
		yVec.push_back(b);

		addE(b,Y,capacity,0);
		v0 += capacity;
		need[i]=capacity;
		id[b]=a;
	}

	for(int i=0;i<N;i++) ran[i]=i;
	int minCost;
	bool success;
	vector<int> xVec;
	if(V<200) deleteServer(xVec);
	else if(V<600) slope3(xVec);
	else slope(xVec);

	minCostFlow(xVec,minCost,success,false);

	vector<vector<int>> result;
	writeResult(result);

	ostringstream ostr;
	ostr<<result.size();
	ostr<<"\n\n";
	for(int i=0;i<result.size();i++){
		for(int j=0;j<result[i].size();j++){
			ostr<<result[i][j];
			if(j!=result[i].size()-1) ostr<<" ";
		}
		if(i!=result.size()-1) ostr<<"\n";
	}
	string str = ostr.str();
	const char* topo_file = str.c_str();

	write_result(topo_file, filename);

}


