#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <cmath>
#include <vector>
#include <map>
#include <array>
#include <sstream>
#include <limits.h>


using namespace std;

#define DBG
#ifdef DBG
	stringstream CIN(
"7 9 "
"0 4 1 1 1 "
"1 6 1 1 3 "
"2 6 1 1 1 "
"2 5 1 2 2 "
"3 0 0 1 1 "
"3 1 1 1 3 "
"3 2 1 1 2 "
"4 5 0 2 2 "
"6 5 1 1 2 "
"3 5 "
	);
#else 
	#define CIN cin
#endif





int N;
int M;
int FROM;
int TO;

// Road
struct Road{
	int from;
	int to;
	int length;
	int time;
};
// All Node
vector< vector<Road> > nodes;


// properties
struct Prop{
	Prop()
	:length(0)
	,time(0)
	,step(0)
	,pre(0)
	,iswaiting(true)
	,valid(false)
	{}
	int length;
	int time;
	int step;
	int pre;
	bool iswaiting;
	bool valid;
};
// properties for process length and time
vector<Prop> waiting;
vector<Prop> waitingTM;

// read data
void readdata(){
	CIN >> N >> M;
	nodes.resize(N);
	waiting.resize(N);
	waitingTM.resize(N);
	
	for(int i=0;i<M;i++){
		int a,b,c,d,e;
		CIN >>a >>b >>c >>d >>e;
		Road road;
		road.length = d;
		road.time = e;
		road.from = a;
		road.to = b;
		nodes[a].push_back(road);
		
		if(c==0){
			road.from=b;
			road.to=a;
			nodes[b].push_back(road);
		}		
	}
	
	
	CIN >> FROM >> TO;
	
}


// if need update length for node
bool isNeedUpdateLength(const Prop& mid, const Prop& target, const Road& road){
		if(target.valid == false
			|| target.length > mid.length + road.length
			|| (target.length == mid.length + road.length && target.time > mid.time+road.time)
		){
			return true;
		}	
		return false;
}

// if need update time for node
bool isNeedUpdateTime(const Prop& mid, const Prop& target, const Road& road){
		if(target.valid == false
			|| target.time > mid.time + road.time
			|| (target.time == mid.time + road.time && target.step > mid.step+1)
		){
			return true;
		}	
		return false;
}
typedef bool(*FCHECK)(const Prop& mid, const Prop& target, const Road& road);

// update value after pick a new node
void updateValue(int node,vector<Prop>& WAITINGS, FCHECK fcheck){
	Prop& mid = WAITINGS[node];
	for(int i=0; i<nodes[node].size(); i++){
		Road& road = nodes[node][i];
		int to=road.to;
		Prop& prop = WAITINGS[to];
		
		if(prop.iswaiting == false){
			continue;
		}
		if(fcheck(mid,prop,road)){
			prop.length = mid.length + road.length;
			prop.time = mid.time + road.time;
			prop.valid = true;
			prop.pre = node;
			prop.step = mid.step+1;
			//cout << node << "<-" << to << endl;
		}
				
	}	
}

int valueToCmpLength(const Prop& prop){
	return prop.length;
}

int valueToCmpTime(const Prop& prop){
	return prop.time;
}
typedef int(*PPICK)(const Prop& prop);

// pick a new node
int selectNext(vector<Prop>& WAITINGS,PPICK ppick){
	int len = INT_MAX;
	int node;
	for(int i=0;i<N;i++){
		Prop& prop = WAITINGS[i];
		if(prop.valid==false){
			continue;
		}
		if(prop.iswaiting == false){
			continue;
		}
		if(ppick(prop) < len){// no matter time
			len = ppick(prop);
			node = i;
		}		
	}
	
	return node;
	
}




void Dijkstra(){
	{
		
		int node = FROM;
		waiting[node].step=0;
		waiting[node].length = 0;
		waiting[node].time = 0;
		waiting[node].valid = true;
		waiting[node].iswaiting = false;
		waiting[node].pre = FROM;
		
		//cout << node << endl;
		for(int i=1;i<N;i++){			
			updateValue(node,waiting,isNeedUpdateLength);
			node = selectNext(waiting,valueToCmpLength);
			waiting[node].iswaiting = false;
			//cout << node << endl;
		}
	}

	{
		int node = FROM;
		waitingTM[node].step = 0;
		waitingTM[node].length = 0;
		waitingTM[node].time = 0;
		waitingTM[node].valid = true;
		waitingTM[node].iswaiting = false;
		waitingTM[node].pre = FROM;

		//cout << node << endl;
		for(int i=1;i<N;i++){
			updateValue(node,waitingTM,isNeedUpdateTime);
			node = selectNext(waitingTM,valueToCmpTime);
			waitingTM[node].iswaiting = false;
			//cout << node << endl;
		}		
	}


}

vector<int> answerlength;
vector<int> answertime;
void findLenAns(int node){
	if(node == FROM){
		answerlength.push_back(node);
	}else{
		int pre=waiting[node].pre;
		findLenAns(pre);
		answerlength.push_back(node);
	}
	
}

void findTimeAns(int node){
	if(node == FROM){
		answertime.push_back(node);
	}else{
		int pre=waitingTM[node].pre;
		findTimeAns(pre);
		answertime.push_back(node);
	}
	
}


void printAnswer(){
	if(answerlength == answertime){
		printf("Distance = %d; Time = %d: %d",waiting[TO].length,waiting[TO].time,answerlength[0]);
		for(int i=1;i<answerlength.size();i++){
			printf(" -> %d",answerlength[i]);
		}
		printf("\n");
		return;
	}
	//
	printf("Distance = %d: %d",waiting[TO].length,answerlength[0]);
	for(int i=1;i<answerlength.size();i++){
		printf(" -> %d",answerlength[i]);
	}
	printf("\n");
	//
	printf("Time = %d: %d",waitingTM[TO].time, answertime[0]);
	for(int i=1;i<answertime.size();i++){
		printf(" -> %d",answertime[i]);
	}
	printf("\n");
	
}

int main(int argc, char** argv) {
	readdata();
	Dijkstra();
	findLenAns(TO);
	findTimeAns(TO);
	printAnswer();
	
}