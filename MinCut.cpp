#include <stdio.h>
#include <algorithm>
#include <time.h>
#include <math.h>
#include <omp.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
//#include "advisor-annotate.h"

typedef std::tuple<int,int> Edge;
typedef std::tuple<int,int,int> EdgeInfo;

struct Node
{
	Node(int idx):id(idx);
	int id;
	std::set<int> edges;
}

class MinCutGraph
{
	int nNodes,nEdges,nReps;//Defines # nodes, edges and monte carlo repititions
	std::map<int,Edge> edgeMap;//Map for storing edges
	std::map<int,Node> nodeMap;//Map for storing nodes

public:
	MinCutGraph();//reads graph from stdin
	//void printGraph();
	//void contract();
};

MinCutGraph::MinCutGraph()
{
	std::cin >> nReps;
	std::cin >> nNodes;
	std::cin >> nEdges;
	int edgeCount = 0;
	for(int i=0;i<nNodes;i++){
		nodeMap[i] = Node(i)//Create all the nodes
	};
	for(int i=0;i<nNodes;i++){
		for(int j=0;j<nNodes;j++){
			edgeCount << std::cin;//Get the current edge count
			if(edgeCount) > 0
				edgeMap[Edge (std::min(i,j), std::max(i,j))]=EdgeInfo (std::min(i,j), std::max(i,j), edgeCount); //Create an edge
		};
	};
		
};

void MinCutGraph::printGraph()
{
	std::cout << nReps << std::endl;
	std::cout << nNodes << " " << nEdges << std::endl;
	for(int i=0;i<nNodes*nNodes;i+=nNodes){
		for(int j=0;j<nNodes;j++)
			std::cout << g[j+i] << " ";
		std::cout << std::endl;
	}
};

int* MinCutGraph::get(int row, int col)
{
	return g+(nNodes*row)+col;
};


int main(int argc, char **argv)
{
	MinCutGraph x = MinCutGraph();
	std::cout << x.get(1,1);
	x.link_inc(1,2);
};


