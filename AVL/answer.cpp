#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <cmath>
#include <vector>
#include <map>
#include <array>
#include <sstream>
#include <limits.h>
#include <queue>


using namespace std;

#define DBG
#ifdef DBG
	stringstream CIN(
"8 88 70 61 96 120 90 65 68"
	);
#else 
	#define CIN cin
#endif

struct Node{
	Node()
	:left(-1)
	,right(-1)
	,value(-1)
	{}
	Node(const Node& node)
	:left(node.left)
	,right(node.right)
	,value(node.value)
	{}
	int left;
	int right;
	int value;
};

int N;
vector<Node> tree;
int root;

void readdata(){
	CIN >> N;
	tree.resize(N);
	for(int i=0;i<N;i++){
		CIN >> tree[i].value;
	}

}

int getheight(int node){
	if(node == -1){
		return 0;
	}
	int lf = getheight(tree[node].left);
	int rt = getheight(tree[node].right);
	return max(lf,rt) + 1;	
}

/*
    0
   /
  0
 /
0

*/
void leftleft(int node){
	//std::cout << "LL " << node << endl;
	Node node_high = tree[node];
	Node node_mid = tree[node_high.left];
	
	tree[node].value = node_mid.value;
	tree[node].left = node_mid.left;
	tree[node].right = node_high.left;
	
	tree[node_high.left].value = node_high.value;
	tree[node_high.left].left = node_mid.right;
	tree[node_high.left].right = node_high.right;
	
}

/*
0
 \
  0
   \
    0

*/
void rightright(int node){
	//std::cout << "RR " << node << endl;
	Node node_high = tree[node];
	Node node_mid = tree[node_high.right];
	
	tree[node].value = node_mid.value;
	tree[node].left = node_high.right;
	tree[node].right = node_mid.right;
	
	tree[node_high.right].value = node_high.value;
	tree[node_high.right].left = node_high.left;
	tree[node_high.right].right = node_mid.left;
}

/*
    0
   /
  0
   \
    0

*/
void leftright(int node){
	//std::cout << "LR " << node << endl;
	rightright(tree[node].left);
	leftleft(node);
}

/*
0
 \
  0
 /
0

*/
void rightleft(int node){
	//std::cout << "RL " << node << endl;
	leftleft(tree[node].right);
	rightright(node);
}

void insert(int parent,int index){
	int val = tree[index].value;
	
	if(tree[index].value < tree[parent].value){
		if(tree[parent].left == -1){
			tree[parent].left = index;
		}else{
			insert(tree[parent].left, index);
		}
		int left = tree[parent].left;
		int right = tree[parent].right;
		int leftheight = getheight(left);
		int rightheight = getheight(right);
		
		if(leftheight - rightheight >= 2){
			if(val < tree[left].value){
				leftleft(parent);
			}else{
				leftright(parent);
			}
		}
		
		
	}else{
		if(tree[parent].right == -1){
			tree[parent].right = index;
		}else{
			insert(tree[parent].right,index);
		}
		int left = tree[parent].left;
		int right = tree[parent].right;
		int leftheight = getheight(left);
		int rightheight = getheight(right);
		
		if(rightheight - leftheight >= 2){
			if(val < tree[right].value){
				rightleft(parent);
			}else{
				rightright(parent);
			}
		}
	
	}
	
	
}

bool isComplete = true;
vector<int> levelseq;
void walklevel(){
	//cout << "walklevel" << endl;
	levelseq.clear();
	queue<int> que;
	que.push(root);
	bool ismeetend=false;
	while(!que.empty()){
		int node = que.front();
		levelseq.push_back(node);
		if(isComplete){
			int left = tree[node].left;
			int right =  tree[node].right;
			
			if(left == -1 && right ==-1){
				if(ismeetend){
				
				}else{
					ismeetend = true;
				}
			}else if(left != -1 && right != -1){
				if(ismeetend){
					isComplete = false;
				}else{
					
				}
			}else if(left != -1 && right == -1){
				if(ismeetend){
					isComplete = false;
				}else{
					ismeetend = true;
				}
			}else if(left == -1 && right != -1){
				if(ismeetend){
					isComplete = false;
				}else{
					isComplete = false;
					ismeetend = true;
				}
			}

		}
		
		if(tree[node].left != -1){
			que.push(tree[node].left);
		}
		if(tree[node].right != -1){
			que.push(tree[node].right);
		}
		
		que.pop();		

	}	
}





void printAnswer(){
	std::cout << tree[levelseq[0]].value;
	for(int i=1;i<levelseq.size();i++){
		std::cout << " " << tree[levelseq[i]].value;
	}
	cout << endl;
	
	if(isComplete){
		cout << "YES" << endl;
	}else{
		cout << "NO" << endl;
	}

}

void work(){
	root = 0;
	for(int i=1;i<N;i++){
		insert(root,i);
	}	
	
}


int main(int argc, char** argv) {
	readdata();
	work();
	walklevel();
	printAnswer();
	
}

