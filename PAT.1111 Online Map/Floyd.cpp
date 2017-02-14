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

//#define DBG
#ifdef DBG
	stringstream CIN(
"10 15 "
"0 1 0 1 1 "
"8 0 0 1 1 "
"4 8 1 1 1 "
"3 4 0 3 2 "
"3 9 1 4 1 "
"0 6 0 1 1 "
"7 5 1 2 1 "
"8 5 1 2 1 "
"2 3 0 2 2 "
"2 1 1 1 1 "
"1 3 0 3 1 "
"1 4 0 1 1 "
"9 7 1 3 1 "
"5 1 0 5 2 "
"6 5 1 1 2 "
"3 5 "
	);
#else 
	#define CIN cin
#endif

struct Info{
	Info()
	:length(0)
	,time(0)
	,count(0)
	,valid(false)
	{
		
	}
	int length;
	int time;
	int count;
	bool valid;
};

typedef vector<  vector<Info>  > RoadMap;
typedef vector<  vector<int>  > PathMap;

int N;
int M;
int FROM;
int TO;

RoadMap roadmaplength;
RoadMap roadmaptime;
PathMap pathlength;
PathMap pathtime;



void readdata(){
	CIN >> N >> M;
	{
		Info info;
		vector<Info> initvalue(N,info);
		RoadMap initmap(N,initvalue);
		roadmaplength = initmap;
		roadmaptime = roadmaplength;
	}
	{
		vector<int> initvalue(N,-1);
		PathMap initpath(N,initvalue);
		pathlength = initpath;
		pathtime = pathlength;		
	}	

	
	for(int i=0;i<M;i++){
		int a,b,c,d,e;
		CIN >>a >>b >>c >>d >>e;		
		
		roadmaplength[a][b].valid = true;
		roadmaplength[a][b].length=d;
		roadmaplength[a][b].time=e;
		roadmaplength[a][b].count = 0;

		roadmaptime[a][b].valid = true;
		roadmaptime[a][b].length=d;
		roadmaptime[a][b].time=e;
		roadmaptime[a][b].count = 0;				
		
		if(0 == c){
			roadmaplength[b][a].valid = true;
			roadmaplength[b][a].length=d;
			roadmaplength[b][a].time=e;
			roadmaplength[b][a].count = 0;
	
			roadmaptime[b][a].valid = true;
			roadmaptime[b][a].length=d;
			roadmaptime[b][a].time=e;
			roadmaptime[b][a].count = 0;
		}		
	}
	
	CIN >> FROM >> TO;
	
}

vector<int> answerlength;
vector<int> answertime;
void findlengthpath(int from, int to){
	int mid = pathlength[from][to];
	if(mid == -1){
		//answerlength.push_back(from);
		//answerlength.push_back(to);
	}else{
		findlengthpath(from,mid);
		answerlength.push_back(mid);
		findlengthpath(mid,to);		
	}
}
void findtimepath(int from, int to){
	int mid = pathtime[from][to];
	if(mid == -1){
		//answertime.push_back(from);
		//answertime.push_back(to);
	}else{
		findtimepath(from,mid);
		answertime.push_back(mid);
		findtimepath(mid,to);		
	}
}

// floyd
void work(){
	for(int n=0;n<N;n++){
		for(int i=0;i<N;i++){
			for(int j=0;j<N;j++){
				if(	roadmaplength[i][n].valid && roadmaplength[n][j].valid){
					if( (roadmaplength[i][j].valid == false)
						|| (roadmaplength[i][n].length + roadmaplength[n][j].length < roadmaplength[i][j].length)
						|| ( 	(roadmaplength[i][n].length + roadmaplength[n][j].length == roadmaplength[i][j].length)
							&& 	(roadmaplength[i][n].time + roadmaplength[n][j].time < roadmaplength[i][j].time)
							)
					){
						roadmaplength[i][j].valid = true;
						roadmaplength[i][j].length = roadmaplength[i][n].length + roadmaplength[n][j].length;
						roadmaplength[i][j].time = 	roadmaplength[i][n].time + roadmaplength[n][j].time;
						roadmaplength[i][j].count = 	roadmaplength[i][n].count + roadmaplength[n][j].count + 1;
						pathlength[i][j] = n;	
					}

				}
				//
				if(	roadmaptime[i][n].valid && roadmaptime[n][j].valid){
					if( (roadmaptime[i][j].valid== false)
						|| (roadmaptime[i][n].time + roadmaptime[n][j].time < roadmaptime[i][j].time)
						|| ( 	(roadmaptime[i][n].time + roadmaptime[n][j].time == roadmaptime[i][j].time)
							&& 	(roadmaptime[i][n].count + roadmaptime[n][j].count < roadmaptime[i][j].count)
							)
					){
						roadmaptime[i][j].valid = true;
						roadmaptime[i][j].length = roadmaptime[i][n].length + roadmaptime[n][j].length;
						roadmaptime[i][j].time = 	roadmaptime[i][n].time + roadmaptime[n][j].time;
						roadmaptime[i][j].count = 	roadmaptime[i][n].count + roadmaptime[n][j].count + 1;
						pathtime[i][j] = n;	
					}

				}				
			
			}
		}
	}
	answerlength.clear();
	answerlength.push_back(FROM);
	findlengthpath(FROM,TO);
	answerlength.push_back(TO);
	
	answertime.clear();
	answertime.push_back(FROM);
	findtimepath(FROM,TO);
	answertime.push_back(TO);
	
}


void printAnswer(){
	if(answerlength == answertime){
		printf("Distance = %d; Time = %d: %d",roadmaplength[FROM][TO].length,roadmaplength[FROM][TO].time,answerlength[0]);
		for(int i=1;i<answerlength.size();i++){
			printf(" -> %d",answerlength[i]);
		}
		printf("\n");
		return;
	}
	//
	printf("Distance = %d: %d",roadmaplength[FROM][TO].length,answerlength[0]);
	for(int i=1;i<answerlength.size();i++){
		printf(" -> %d",answerlength[i]);
	}
	printf("\n");
	//
	printf("Time = %d: %d",roadmaptime[FROM][TO].time, answertime[0]);
	for(int i=1;i<answertime.size();i++){
		printf(" -> %d",answertime[i]);
	}
	printf("\n");
	
}

int main(int argc, char** argv) {
	readdata();
	work();
	
	printAnswer();
	
}