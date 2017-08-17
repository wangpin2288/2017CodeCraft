#include "own.h"
#include "deploy.h"

//extern const int N = 1010;  //const默认问文件独有，为了多文件共享，必须在定义的时候加上extern
vector<int> ran(N, 0);

Edge EG[M];
int V, X, Y;
vector<int> yVec;
int need[N];
int id[N];
vector<vector<int>> result;
int servicePrice;
int pred[N];
int dist[N];
bool marked[N];
int first[N];
int cnt=0;
int f0;
int v0;
int TIME;
bool flag = false;

milliseconds start = duration_cast<milliseconds>(
		system_clock::now().time_since_epoch()
);


void addE(int a, int b, int c, int p){
    EG[cnt].setEG(a,b,c,p );
    EG[cnt].next = first[a]; first[a] = cnt++;

    EG[cnt].setEG(b, a, 0,-p );
    EG[cnt].next = first[b]; first[b] = cnt++;
}


int  getRandomNum(int n)
{
	while (true){
		random_shuffle(ran.begin(), ran.end());
		if (*ran.begin()<n) return *ran.begin();
	}
}

bool spfa(){
    for(int i=0;i<V;i++){
		pred[i]=-1;
		dist[i]=MAXINT;
		marked[i]=false;
	}
    deque<int> dq;
    dist[X] = 0; 
    dq.push_front(X); 
    marked[X] = true;
    
    while(!dq.empty()){
        int u = dq.front(); dq.pop_front(); marked[u] = false;
        for(int e = first[u]; e != -1; e = EG[e].next){
            int  v = EG[e].b;
            if( EG[e].c_f>0 && dist[v] > dist[u] + EG[e].p ){
                dist[v] = dist[u] + EG[e].p;
                pred[v] = e;
                if( !marked[v] ){
                	if(!dq.empty() && dist[v]<=dist[dq.front()])
                		dq.push_front(v);
                    else 
                    	dq.push_back(v);
                    marked[v] = true;
                }
            }
        }
    }
    return pred[Y] != -1;
}

void minCostFlow(vector<int>& xVec,int& minCost,bool& success,bool flag = true){
	static const int tmp = cnt;
	for(int i=0;i<xVec.size();i++){
		addE(X,xVec[i],v0,0);
	}
	
	int totalFlow = 0;
	minCost = xVec.size()*servicePrice;
	success = false;
    while(spfa()){
        int delta = MAXINT;
        for(int e = pred[Y]; e != -1; e = pred[EG[e].a])
        {
            if( delta > EG[e].c_f ) delta = EG[e].c_f;
        }
        for(int e = pred[Y]; e != -1; e = pred[EG[e].a])
        {
            	EG[e].c_f -= delta;
           	 	EG[e^1].c_f += delta;
        }
        minCost += dist[Y]*delta;
        totalFlow += delta;
    }
    
    if(totalFlow == v0)
    {
        success = true;
    }else{
    	minCost = MAXINT;
    }
    
    if(flag){
    	while(cnt != tmp){
    		cnt--;   
    		first[EG[cnt].a] = EG[cnt].next;
   	 	}
		for(int i=0;i<cnt;i++){
			EG[i].c_f = EG[i].c;
		}
	}
	
}

void deleteServer(vector<int>& xVec)
{
    priority_queue<Node,vector<Node>,less<Node>> pq;
	for(int i=0;i<yVec.size();i++){
		pq.push(Node(yVec[i],need[i]));
	}

	vector<int> Vec;
	while(!pq.empty()){
		Vec.push_back(pq.top().nodeId);
		pq.pop();
	}

	vector<int> v;
	int ct = 0;
	bool suc1,suc2;
	int cost1,cost2;
	for(int i=0;i<yVec.size();i++)
	{
		v = Vec;
		v.erase(v.begin()+ct);
		minCostFlow(v,cost1,suc1);
		minCostFlow(Vec,cost2,suc2);
		if(cost1 < cost2){
			Vec = v;
		}else{
			ct++;
		}

		milliseconds end = duration_cast<milliseconds>(
			system_clock::now().time_since_epoch()
		);
		if(end.count()-start.count() > 88000){
			xVec = Vec;return;
		}
	}

	int visited[V-2];
	memset(visited,0,sizeof(visited));
	for(int i=0;i<Vec.size();i++) visited[Vec[i]]=1;

	for(int i=0;i<Vec.size();i++){
		for(int j=0;j<V-2;j++){
			if(visited[j]==0){
				v = Vec;
				v[i] = j;
				minCostFlow(v,cost1,suc1);
				minCostFlow(Vec,cost2,suc2);
				if(cost1 < cost2){
					Vec = v;
					visited[Vec[i]] = 0;
					visited[j] = 1;
				}
			}

			milliseconds end = duration_cast<milliseconds>(
				system_clock::now().time_since_epoch()
			);
			if(end.count()-start.count() > 88000){
				xVec = Vec;return;
			}
		}
	}

	
	ct = 0;
	int sz = Vec.size();
	for(int i=0;i<sz;i++)
	{
		v = Vec;
		v.erase(v.begin()+ct);
		minCostFlow(v,cost1,suc1);
		minCostFlow(Vec,cost2,suc2);
		if(cost1 < cost2){
			Vec = v;
		}else{
			ct++;
		}

		milliseconds end = duration_cast<milliseconds>(
			system_clock::now().time_since_epoch()
		);
		if(end.count()-start.count() > 88000){
			xVec = Vec;return;
		}
	}

	sz = Vec.size();
	vector<int> xvec = Vec;
	for(int i=0;i<V-2;i++){
		if(visited[i]) continue;
		v=Vec;
		v.push_back(i);
		minCostFlow(v,cost1,suc1);
		minCostFlow(Vec,cost2,suc2);
		if(cost1<cost2){
			Vec = v;
		}
		milliseconds end = duration_cast<milliseconds>(
			system_clock::now().time_since_epoch()
		);
		if(end.count()-start.count() > 88000){
			xVec = Vec;return;
		}
	}
	
	ct = 0;
	sz = Vec.size();
	for(int i=0;i<sz;i++)
	{
		v = Vec;
		v.erase(v.begin()+ct);
		minCostFlow(v,cost1,suc1);
		minCostFlow(Vec,cost2,suc2);
		if(cost1 < cost2){
			Vec = v;
		}else{
			ct++;
		}
		milliseconds end = duration_cast<milliseconds>(
			system_clock::now().time_since_epoch()
		);
		if(end.count()-start.count() > 88000){
			xVec = Vec;return;
		}
	}

	xVec = Vec;
}


void MCF(bool& success,int& minCost)
{
	int totalFlow = 0;
	minCost = 0;
	success = false;
    while(spfa()){
        int delta = MAXINT;
        for(int e = pred[Y]; e != -1;e = pred[EG[e].a])
        {
            if( delta > EG[e].c_f ) delta = EG[e].c_f;
        }
		int price=0;
        for(int e = pred[Y]; e != -1; e = pred[EG[e].a])
        {
				price = EG[e].p;
            	EG[e].c_f -= delta;
           	 	EG[e^1].c_f += delta;
        }
        minCost += (dist[Y]-price)*delta;
        totalFlow += delta;
    }   

    if(totalFlow == v0)
    {
        success = true;
    }else{
    	minCost = MAXINT;
    }

}

void slope(vector<int>& vec)
{
	static const int tmp = cnt;
	for(int i=0;i<cnt;i++) EG[i].p *= 1000;
	int visited[V];
	memset(visited,0,sizeof(visited));
	for(int i=0;i<yVec.size();i++) visited[yVec[i]]=1;

	int Last[V-2];
	int Now[V-2];
	for(int i=0;i<V-2;i++){
		if(!visited[i]){
			addE(X,i,v0,MAXINT);
			Last[i]=Now[i]=0;
		}
	}
	for(int i=0;i<yVec.size();i++){
		addE(X,yVec[i],v0,1000*servicePrice/need[i]);
		Last[yVec[i]]=Now[yVec[i]]=need[i];
	}

	bool success = true;
	int minCost;
	while(true){	
		MCF(success,minCost);
		vec.clear();
		for(int e=first[X]; e!=-1; e=EG[e].next){
			int f = EG[e].c-EG[e].c_f;
			int j = EG[e].b;
			Now[j] = f;
			if(f>0){
				minCost += servicePrice;
				vec.push_back(EG[e].b);
				EG[e].p = 1000*servicePrice/f;
				EG[e^1].p = -1000*servicePrice/f;
			}else{
				EG[e].p = MAXINT;
				EG[e^1].p = -MAXINT;
			}
		}

		for(int i=0;i<cnt;i++){
			EG[i].c_f = EG[i].c;
		}

		bool flag = true;
		for(int i=0;i<V-2;i++){
			if(Now[i]!=Last[i]) flag = false;
		}
		if(flag) break;
		else{
			for(int i=0;i<V-2;i++) Last[i]=Now[i];
		}

		milliseconds end = duration_cast<milliseconds>(

				system_clock::now().time_since_epoch()
		);
		if(end.count()-start.count() > 88000){
				return;
		}
	}
	
	while(cnt != tmp){
    	cnt--;   
    	first[EG[cnt].a] = EG[cnt].next;
   	}
   	for(int i=0;i<cnt;i++) EG[i].p = EG[i].p/1000;

	priority_queue<Node,vector<Node>,less<Node>> pq;
	for(int i=0;i<vec.size();i++){
		pq.push(Node(vec[i],need[id[vec[i]]]));
	}

	vector<int> v;
	vec.clear();
	while(!pq.empty()){
		vec.push_back(pq.top().nodeId);
		pq.pop();
	}
	
	int ct = 0;
	int sz = vec.size();
	int cost1,cost2;
	bool suc1,suc2;
	for(int i=0;i<sz;i++)
	{
		v = vec;
		v.erase(v.begin()+ct);
		minCostFlow(v,cost1,suc1);
		minCostFlow(vec,cost2,suc2);
		if(cost1 < cost2){
			vec = v;
		}else{
			ct++;
		}

		milliseconds end = duration_cast<milliseconds>(
				system_clock::now().time_since_epoch()
		);
		if(end.count()-start.count() > 88000){
				return;
		}
	}

	memset(visited,0,sizeof(visited));
	for(int i=0;i<vec.size();i++) visited[vec[i]]=1;

	sz = vec.size();
	vector<int> xvec = vec;
	for(int k=0;k<yVec.size();k++){
		int i=yVec[k];
		if(visited[i]) continue;
		v=vec;
		v.push_back(i);
		minCostFlow(v,cost1,suc1);
		minCostFlow(vec,cost2,suc2);
		if(cost1<cost2){
			vec = v;
		}
		milliseconds end = duration_cast<milliseconds>(
				system_clock::now().time_since_epoch()
		);
		if(end.count()-start.count() > 88000){
				return;
		}
	}
	
	ct = 0;
	sz = vec.size();
	for(int i=0;i<sz;i++)
	{
		v = vec;
		v.erase(v.begin()+ct);
		minCostFlow(v,cost1,suc1);
		minCostFlow(vec,cost2,suc2);
		if(cost1 < cost2){
			vec = v;
		}else{
			ct++;
		}
		milliseconds end = duration_cast<milliseconds>(
				system_clock::now().time_since_epoch()
		);
		if(end.count()-start.count() > 88000){
				return;
		}
	}

}


void slope2(vector<int>& vec)
{
	int totalCost[yVec.size()];
	for(int k=0;k<yVec.size();k++){
		int u=yVec[k];
		int totalPrice=0;
		int num=0;
		for(int e=first[u];e!=-1;e=EG[e].next){
			if(EG[e].p>0){
				totalPrice+=EG[e].p;
				num++;
			}
		}
		totalCost[k]=need[k]*(totalPrice/num);
	}
	
	static const int tmp = cnt;
	for(int i=0;i<cnt;i++) EG[i].p *= 1000;
	int visited[V];
	memset(visited,0,sizeof(visited));
	for(int i=0;i<yVec.size();i++) visited[yVec[i]]=1;

	int Last[V-2];
	int Now[V-2];
	for(int i=0;i<V-2;i++){
		if(!visited[i]){
			addE(X,i,v0,MAXINT);
			Last[i]=Now[i]=0;
		}
	}
	for(int i=0;i<yVec.size();i++){
		addE(X,yVec[i],v0,1000*servicePrice/need[i]);
		Last[yVec[i]]=Now[yVec[i]]=need[i];
	}

	bool success = true;
	int minCost;
	while(true){	
		MCF(success,minCost);
		vec.clear();
		for(int e=first[X]; e!=-1; e=EG[e].next){
			int f = EG[e].c-EG[e].c_f;
			int j = EG[e].b;
			Now[j] = f;
			if(f>0){
				minCost += servicePrice;
				vec.push_back(EG[e].b);
				EG[e].p = 1000*servicePrice/f;
				EG[e^1].p = -1000*servicePrice/f;
			}else{
				EG[e].p = MAXINT;
				EG[e^1].p = -MAXINT;
			}
		}

		for(int i=0;i<cnt;i++){
			EG[i].c_f = EG[i].c;
		}

		bool flag = true;
		for(int i=0;i<V-2;i++){
			if(Now[i]!=Last[i]) flag = false;
		}
		if(flag) break;
		else{
			for(int i=0;i<V-2;i++) Last[i]=Now[i];
		}

		milliseconds end = duration_cast<milliseconds>(

				system_clock::now().time_since_epoch()
		);
		if(end.count()-start.count() > 88000){
				return;
		}
	}
	
	while(cnt != tmp){
    	cnt--;   
    	first[EG[cnt].a] = EG[cnt].next;
   	}
   	for(int i=0;i<cnt;i++) EG[i].p = EG[i].p/1000;

	priority_queue<Node,vector<Node>,less<Node>> pq;
	for(int i=0;i<vec.size();i++){
		pq.push(Node(vec[i],totalCost[id[vec[i]]]));
	}

	vector<int> v;
	vec.clear();
	while(!pq.empty()){
		vec.push_back(pq.top().nodeId);
		pq.pop();
	}
	
	int ct = 0;
	int sz = vec.size();
	int cost1,cost2;
	bool suc1,suc2;
	for(int i=0;i<sz;i++)
	{
		v = vec;
		v.erase(v.begin()+ct);
		minCostFlow(v,cost1,suc1);
		minCostFlow(vec,cost2,suc2);
		if(cost1 < cost2){
			vec = v;
		}else{
			ct++;
		}

		milliseconds end = duration_cast<milliseconds>(
				system_clock::now().time_since_epoch()
		);
		if(end.count()-start.count() > 88000){
				return;
		}
	}

	memset(visited,0,sizeof(visited));
	for(int i=0;i<vec.size();i++) visited[vec[i]]=1;

	sz = vec.size();
	vector<int> xvec = vec;
	for(int k=0;k<yVec.size();k++){
		int i=yVec[k];
		if(visited[i]) continue;
		v=vec;
		v.push_back(i);
		minCostFlow(v,cost1,suc1);
		minCostFlow(vec,cost2,suc2);
		if(cost1<cost2){
			vec = v;
		}
		milliseconds end = duration_cast<milliseconds>(
				system_clock::now().time_since_epoch()
		);
		if(end.count()-start.count() > 88000){
				return;
		}
	}
	
	ct = 0;
	sz = vec.size();
	for(int i=0;i<sz;i++)
	{
		v = vec;
		v.erase(v.begin()+ct);
		minCostFlow(v,cost1,suc1);
		minCostFlow(vec,cost2,suc2);
		if(cost1 < cost2){
			vec = v;
		}else{
			ct++;
		}
		milliseconds end = duration_cast<milliseconds>(
				system_clock::now().time_since_epoch()
		);
		if(end.count()-start.count() > 88000){
				return;
		}
	}


}

void slope3(vector<int>& vec)
{
	static const int tmp = cnt;
	for(int i=0;i<cnt;i++) EG[i].p *= 1000;
	int visited[V];
	memset(visited,0,sizeof(visited));
	for(int i=0;i<yVec.size();i++) visited[yVec[i]]=1;

	int Last[V-2];
	int Now[V-2];
	for(int i=0;i<V-2;i++){
		if(!visited[i]){
			addE(X,i,v0,MAXINT);
			Last[i]=Now[i]=0;
		}
	}
	for(int i=0;i<yVec.size();i++){
		addE(X,yVec[i],v0,1000*servicePrice/need[i]);
		Last[yVec[i]]=Now[yVec[i]]=need[i];
	}

	bool success = true;
	int minCost;
	while(true){	
		MCF(success,minCost);
		vec.clear();
		for(int e=first[X]; e!=-1; e=EG[e].next){
			int f = EG[e].c-EG[e].c_f;
			int j = EG[e].b;
			Now[j] = f;
			if(f>0){
				minCost += servicePrice;
				vec.push_back(EG[e].b);
				EG[e].p = 1000*servicePrice/f;
				EG[e^1].p = -1000*servicePrice/f;
			}else{
				EG[e].p = MAXINT;
				EG[e^1].p = -MAXINT;
			}
		}

		for(int i=0;i<cnt;i++){
			EG[i].c_f = EG[i].c;
		}

		bool flag = true;
		for(int i=0;i<V-2;i++){
			if(Now[i]!=Last[i]) flag = false;
		}
		if(flag) break;
		else{
			for(int i=0;i<V-2;i++) Last[i]=Now[i];
		}

		milliseconds end = duration_cast<milliseconds>(

				system_clock::now().time_since_epoch()
		);
		if(end.count()-start.count() > 88000){
				return;
		}
	}
	
	while(cnt != tmp){
    	cnt--;   
    	first[EG[cnt].a] = EG[cnt].next;
   	}
   	for(int i=0;i<cnt;i++) EG[i].p = EG[i].p/1000;

	priority_queue<Node,vector<Node>,less<Node>> pq;
	for(int i=0;i<vec.size();i++){
		pq.push(Node(vec[i],need[id[vec[i]]]));
	}

	vector<int> v;
	vec.clear();
	while(!pq.empty()){
		vec.push_back(pq.top().nodeId);
		pq.pop();
	}
	
	int ct = 0;
	int sz = vec.size();
	int cost1,cost2;
	bool suc1,suc2;
	for(int i=0;i<sz;i++)
	{
		v = vec;
		v.erase(v.begin()+ct);
		minCostFlow(v,cost1,suc1);
		minCostFlow(vec,cost2,suc2);
		if(cost1 < cost2){
			vec = v;
		}else{
			ct++;
		}

		milliseconds end = duration_cast<milliseconds>(
				system_clock::now().time_since_epoch()
		);
		if(end.count()-start.count() > 88000){
				return;
		}
	}

	memset(visited,0,sizeof(visited));
	for(int i=0;i<vec.size();i++) visited[vec[i]]=1;

	sz = vec.size();
	vector<int> xvec = vec;
	for(int i=0;i<V-2;i++){
		if(visited[i]) continue;
		v=vec;
		v.push_back(i);
		minCostFlow(v,cost1,suc1);
		minCostFlow(vec,cost2,suc2);
		if(cost1<cost2){
			vec = v;
		}
		milliseconds end = duration_cast<milliseconds>(
				system_clock::now().time_since_epoch()
		);
		if(end.count()-start.count() > 88000){
				return;
		}
	}
	
	ct = 0;
	sz = vec.size();
	for(int i=0;i<sz;i++)
	{
		v = vec;
		v.erase(v.begin()+ct);
		minCostFlow(v,cost1,suc1);
		minCostFlow(vec,cost2,suc2);
		if(cost1 < cost2){
			vec = v;
		}else{
			ct++;
		}
		milliseconds end = duration_cast<milliseconds>(
				system_clock::now().time_since_epoch()
		);
		if(end.count()-start.count() > 88000){
				return;
		}
	}

}



int push_flow(int u, int leaveFlow){
    if( leaveFlow == 0) return 0;
    if( u == Y ){
    	stack<int> s;
    	int x;
    	int y;
    	for(int e = pred[Y];e!=-1;e = pred[x]){
    		x = EG[e].a;
    		y = EG[e].b;
    		s.push(x);
    	}
    	s.pop();
    	vector<int> vec;
    	while(!s.empty()){
    		vec.push_back(s.top());
    		s.pop();
    	}
    	for(int i=0;i<yVec.size();i++){
    		if(yVec[i]==vec[vec.size()-1]){
    			vec.push_back(i);
    			break;	
    		}
    	}
    	vec.push_back(leaveFlow);
    	
        result.push_back(vec);
        return leaveFlow;
    }

    int pushFlow = 0;
    for(int e = first[u]; e != -1; e = EG[e].next){
        int v = EG[e].b;
        if(EG[e].c-EG[e].c_f > 0){
            int f = leaveFlow-pushFlow;
            if(EG[e].c-EG[e].c_f < f) f = EG[e].c-EG[e].c_f;
            pred[v] = e;
            f = push_flow(v,f);
            pushFlow += f;
            EG[e].c_f += f;
            EG[e^1].c_f -= f;
        }
    }
    return pushFlow;
}

void writeResult(vector<vector<int>>& res)
{
	for(int i=0;i<V;i++){
		pred[i]=-1;
	}	
	push_flow(X,MAXINT);
	res = result;
}
