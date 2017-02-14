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


struct Street{
	int from;
	int to;
	int time;
	int length;
	bool operator==(const Street& street) const {
		return from == street.from
			&& to == street.to
			&& time == street.time
			&& length == street.length;
	}
};

struct Answer{
	int time;
	int length;
	vector<Street> streets;
	bool operator==(const Answer& answer) const{
		return time == answer.time
			&& length == answer.length
			&& streets == answer.streets;
	}
};

typedef vector<Street> Node;
typedef vector<Node> RoadMap;

int N;
int M;
int FROM;
int TO;

RoadMap roadmap;



void readdata(){
	CIN >> N >> M;
	roadmap.resize(N);
	for(int i=0;i<M;i++){
		int a,b,c,d,e;
		CIN >>a >>b >>c >>d >>e;
		
		Street street;
		street.length = d;
		street.time = e;
		street.from = a;
		street.to = b;
		roadmap[street.from].push_back(street);
		
		if(0 == c){
			street.from = b;
			street.to = a;
			roadmap[street.from].push_back(street);
		}
		
	}
	
	CIN >> FROM >> TO;
	
}

map<int , bool> isvisited;
Answer minTimeWay;
Answer minLengthWay;

void work(int curr, Answer &answer){
	if(isvisited[curr]){
		return;
	}
	
	if(curr == TO){
		if(answer.time<minTimeWay.time){
			minTimeWay = answer;
		}else if(answer.time==minTimeWay.time){
			if(answer.streets.size() < minTimeWay.streets.size()){
				minTimeWay = answer;
			} 
		}
		
		if(answer.length<minLengthWay.length){
			minLengthWay = answer;
		}else if(answer.length==minLengthWay.length){
			if(answer.time<minLengthWay.time){
				minLengthWay = answer;
			}
		}
		return;
	}
	
	if(answer.time > minTimeWay.time && answer.length > minLengthWay.length)
		return;
	

	isvisited[curr] = true;

	for(const auto& street : roadmap[curr]){
		answer.streets.push_back(street);
		answer.time+=street.time;
		answer.length+=street.length;
		work(street.to,answer);
		answer.time-=street.time;
		answer.length-=street.length;
		answer.streets.pop_back();	
	}	
	isvisited[curr] = false;
	
}


void printAnswer(){
	if(minTimeWay == minLengthWay){
		printf("Distance = %d; Time = %d: %d",minTimeWay.length,minTimeWay.time,minTimeWay.streets[0].from);
		for(int i=0;i<minTimeWay.streets.size();i++){
			printf(" -> %d",minTimeWay.streets[i].to);
		}
		printf("\n");
		return;
	}
	//
	printf("Distance = %d: %d",minLengthWay.length,minLengthWay.streets[0].from);
	for(int i=0;i<minLengthWay.streets.size();i++){
		printf(" -> %d",minLengthWay.streets[i].to);
	}
	printf("\n");
	//
	printf("Time = %d: %d",minTimeWay.time,minTimeWay.streets[0].from);
	for(int i=0;i<minTimeWay.streets.size();i++){
		printf(" -> %d",minTimeWay.streets[i].to);
	}
	printf("\n");
	
}

int main(int argc, char** argv) {
	readdata();
	minTimeWay.streets.clear();
	minTimeWay.length = INT_MAX;
	minTimeWay.time = INT_MAX;
	
	minLengthWay.streets.clear();
	minLengthWay.length = INT_MAX;
	minLengthWay.time = INT_MAX;
	
	Answer empty;
	empty.streets.clear();
	empty.length=0;
	empty.time=0;
	
	work(FROM,empty);
	
	printAnswer();
	
}