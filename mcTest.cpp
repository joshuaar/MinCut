#include <stdio.h>
#include <algorithm>
#include <time.h>
#include <math.h>
#include <omp.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
//#include "advisor-annotate.h"

//Define map iterators
typedef std::map< int, std::set<int> >::iterator node_it;
typedef std::map< int, std::map<int,int> >::iterator edge_it1;
typedef std::map< int,int >::iterator edge_it2;
typedef std::set<int>::iterator set_it;
typedef std::map< int,std::map<int,int> > edge_map;//Map for all the edges of the graph

class Graph
{
	std::map< int,std::set<int> > nodeMap;//Map for all the nodes in the graph
	edge_map edgeMap;
	int nNodes;
	int nEdges;
	int nReps;//Defines # nodes, edges and monte carlo repititions
public:	
	Graph(){
		std::cin >> nReps;
		std::cin >> nNodes;
		std::cin >> nEdges;
		int curEdge;
		for(int i = 0;i < nNodes; i++)
			addNode(i);
		for(int i = 0; i< nNodes; i++){
			for(int j = 0; j< nNodes; j++){
				std::cin >> curEdge;
				if(curEdge > 0)
					addEdge(i,j,curEdge);
			}
				
		}
		
	};
	int ** edgeArray;
	void addNode(int nodeID){
		if(nodeMap.find(nodeID) != nodeMap.end())//if node does not exist, insert it
			nodeMap[nodeID] = std::set<int>();
	};
	void addEdge(int nodeA, int nodeB, int count){//For adding an edge
		nodeMap[nodeA].insert(nodeB);
		nodeMap[nodeB].insert(nodeA);
		edgeMap[std::min(nodeA,nodeB)][std::max(nodeA,nodeB)]=count;
	};

	void getAllEdges(){//Gets a 2-d array of all edges. Meant to be called once by the constructor
		int** out = new int*[nEdges];	
		int ix = 0;
		for(edge_it1 it = edgeMap.begin(); it != edgeMap.end(); it++){
			int nodeA = it->first;		
			for(edge_it2 jt = it->second.begin(); jt != it->second.end(); jt++){
				int nodeB = jt->first;
				int count = jt->second;
				out[ix] = new int[3];
				out[ix][0] = nodeA;
				out[ix][1] = nodeB;
				out[ix][2] = count;
				ix+=1;		
			};
		};
		edgeArray = out;
	};	

	int getEdgeCount(int i, int j){
		return edgeMap[std::min(i,j)][std::max(i,j)];
	}
	void printGraph(){
		for(node_it it = nodeMap.begin(); it != nodeMap.end(); it++){
			int node = it->first;		
			std::cout << node << ": ";
			for(set_it jt = it->second.begin(); jt != it->second.end(); jt++){
				std::cout << *jt <<";"<< getEdgeCount(node,*jt) << " ";
			};
			std::cout << std::endl;
		};
	};
};

int main(int argc, char **argv)
{
	Graph x = Graph();	
	x.printGraph();
	x.getAllEdges();
	std::cout << x.edgeArray[0][1];
};


