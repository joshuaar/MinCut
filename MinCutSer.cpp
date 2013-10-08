#include <stdio.h>
#include <algorithm>
#include <time.h>
#include <math.h>
#include <omp.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <map>
#include <set>
#include <ctime>
//#include "advisor-annotate.h"

//Define map iterators

struct edge_record {
	int a,b;
	int count;
	edge_record(int nda, int ndb, int c){
		a = nda;
		b = ndb;
		count = c;
	};
	void print(){
		std::cout << "(" << a << ", " << b << ")";
	}
};

typedef std::map< int, std::set<int> >::iterator node_it;
typedef std::map< int, std::map<int,std::vector<edge_record> > >::iterator edge_it1;
typedef std::map< int,std::vector<edge_record> >::iterator edge_it2;
typedef std::vector<edge_record>::iterator edge_record_it;
typedef std::vector<int>::iterator node_vec_it;
typedef std::set<int>::iterator set_it;
typedef std::map< int,std::map< int,std::vector<edge_record> > > edge_map;//Map for all the edges of the graph
class Graph
{
	std::map< int,std::set<int> > nodeMap;//Map for all the nodes in the graph
	edge_map edgeMap;
	int nEdges;
	int nReps;//Defines # nodes, edges and monte carlo repititions
public:	
	int nNodes;
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
	void addNode(int nodeID){
		if(nodeMap.find(nodeID) != nodeMap.end())//if node does not exist, insert it
			nodeMap[nodeID] = std::set<int>();
	};

	bool edgeExists(int i, int j){
		if(edgeMap.find(std::min(i,j)) != edgeMap.end() && edgeMap[std::min(i,j)].find(std::max(i,j)) != edgeMap[std::min(i,j)].end())
			return true;
		return false;
	};
	bool nodeExists(int i){
		if(nodeMap.find(i) != nodeMap.end()){
			return true;
		}
		return false;
	};
	void addEdge(int nodeA, int nodeB, int count){//only adds if it doesnt exist
		if(edgeExists(nodeA,nodeB))
			return;	
		nodeMap[nodeA].insert(nodeB);
		nodeMap[nodeB].insert(nodeA);
		edgeMap[std::min(nodeA,nodeB)][std::max(nodeA,nodeB)].push_back(edge_record(nodeA,nodeB,count));
	};
	void addEdge(int nodeA, int nodeB, std::vector<edge_record>k){//used in the contract function.
		nodeMap[nodeA].insert(nodeB);
		nodeMap[nodeB].insert(nodeA);
		edgeMap[std::min(nodeA,nodeB)][std::max(nodeA,nodeB)]=k;//Retain the original edge record, do not create a new one
		//std::cout << "---Edge Added--- " << nodeA << " , " << nodeB << " , " << k.size() << std::endl;
	}
	std::vector<edge_record> getEdge(int i, int j){
		return edgeMap[std::min(i,j)][std::max(i,j)];
	}
	std::vector<int> getAllNodes(bool shuffle){
		std::vector<int> out;
		for(node_it it = nodeMap.begin(); it!=nodeMap.end();it++){
			out.push_back(it->first);
		};
		if(!shuffle)
			return out;
		std::random_shuffle(out.begin(),out.end());
		return out;
	};
	std::vector<edge_record> getAllEdges(){//Gets a vector of all edges. Meant to be called once at beginning of min-cut algorithm
		std::vector<edge_record> out;	
		for(edge_it1 it = edgeMap.begin(); it != edgeMap.end(); it++){
			int nodeA = it->first;		
			for(edge_it2 jt = it->second.begin(); jt != it->second.end(); jt++){
				std::vector<edge_record> record = jt->second;
				for(int k=0; k < record.size(); k++){
					out.push_back(record[k]);
				};
			};
		};
		return out;
	};	
	std::vector<edge_record> findCut(){//gets a cut in g. Not the monte carlo min-cut algorithmi. returns null if the graph is not connected at start (i.e. there is already a cut in the graph)
		//std::cout<<"cutFinding"<<std::endl;
		std::vector<int> nodes = getAllNodes(true);
		//std::cout<<"num nodes"<< nodes.size()<<std::endl;
		for(node_vec_it it=nodes.begin(); it!=nodes.end();it++){//Go through the random ordering of nodes
			if(nNodes <= 2){ //if we have less than 2 nodes, we're done
				//printGraph();
				return getAllEdges();
			}
			//std::cout<<"stillGoing"<< nNodes <<std::endl;
			std::vector<int> v(nodeMap[*it].begin(),nodeMap[*it].end());
			std::random_shuffle(v.begin(),v.end());//Randomly shuffle the edges
			std::vector<int>::iterator jt=v.begin();//now we grab the first one
			if(jt!=v.end()){//If this node is at least degree 1		
			//	std::cout << *jt <<std::endl;
				//std::cout<<"Contracting: "<< *jt << " , " << *it << std::endl;
				contract(*jt,*it);//contract in the right order
				nNodes -= 1;
			}
		}
		return getAllEdges();		
	};
	

	float failProb(){
		float n = (float)nNodes;
		
		return pow((1  - (2.0/(n*(n-1)))),nReps);

		
	}

	void minCutSer(){//Here is the min cut algorithm;
		Graph x = *this;
		int n = x.nNodes;
		int e = x.nEdges;
		std::vector<edge_record> cut_candidate = x.findCut();
		double before, after;	
		before = omp_get_wtime();
		for(int i=0; i<nReps;i++){
			x = *this;
			std::vector<edge_record> cut_candidate_new = x.findCut(); 
			if(cut_candidate_new.size()<cut_candidate.size())
				cut_candidate=cut_candidate_new;
		}
		after = omp_get_wtime();
		std::cout << "Num Iterations: " << nReps << std::endl;
		std::cout << "Nodes: " << n << " Edges: " << e << std::endl;
		std::cout << "Failure Probability: "<< failProb() <<std::endl;
		std::cout << "Execution Time: "<< (float)(after-before) << " s" << std::endl;
		std::cout << "Min Cut Edges: ";
		for(int i=0; i<cut_candidate.size();i++)
			cut_candidate[i].print();
			std::cout << std::endl;
	};


	void minCut(){//Here is the min cut algorithm;
		Graph x = *this;
		int n = x.nNodes;
		int e = x.nEdges;
		std::vector<edge_record> cut_candidate = x.findCut();
		double before, after;	
		before = omp_get_wtime();
#pragma omp parallel for firstprivate(x)
		for(int i=0; i<nReps;i++){
			//std::cout << omp_get_thread_num();
			x = *this;
			std::vector<edge_record> cut_candidate_new = x.findCut(); 
			if(cut_candidate_new.size()<cut_candidate.size())
				cut_candidate=cut_candidate_new;
		}
		after = omp_get_wtime();
		std::cout << "Num Iterations: " << nReps << std::endl;
		std::cout << "Nodes: " << n << " Edges: " << e << std::endl;
		std::cout << "Failure Probability: "<< failProb() <<std::endl;
		std::cout << "Execution Time: "<< (float)(after-before) << " s" << std::endl;
		std::cout << "Min Cut Edges: ";
		for(int i=0; i<cut_candidate.size();i++)
			cut_candidate[i].print();
			std::cout << std::endl;
	};

	std::vector<edge_record> getEdgeCount(int i, int j){
		return edgeMap[std::min(i,j)][std::max(i,j)];
	}
	void edgeCountInc(int i, int j, std::vector<edge_record> k){
		for(int l = 0; l< k.size(); l++){
			edgeMap[std::min(i,j)][std::max(i,j)].push_back(k[l]);
		}
	}
	void deleteEdge(int i, int j, bool complete){
		if(edgeMap.find(std::min(i,j)) != edgeMap.end() && edgeMap[std::min(i,j)].find(std::max(i,j)) != edgeMap[std::min(i,j)].end()){//If edge exists
			edgeMap[std::min(i,j)].erase(std::max(i,j));
			if(complete){
				nodeMap[i].erase(j);
				nodeMap[j].erase(i);
			}
		}	
	}
	void deleteNode(int i){
		for(set_it it = nodeMap[i].begin(); it != nodeMap[i].end(); it++){
			deleteEdge(i,*it,false);
			nodeMap[*it].erase(i);
		}
		nodeMap[i].erase(nodeMap[i].begin(),nodeMap[i].end());//Erase record of edges in the nodemap
		nodeMap.erase(i);//Erase the node entry from the nodemap
		
	};

	void printGraph(){
		for(node_it it = nodeMap.begin(); it != nodeMap.end(); it++){
			int node = it->first;		
			std::cout << node << ": ";
			for(set_it jt = it->second.begin(); jt != it->second.end(); jt++){
				std::vector<edge_record> records = getEdgeCount(node,*jt);
				std::cout << *jt << "; ";
				for(int k = 0; k<records.size();k++)
					std::cout << records[k].a << "." << records[k].b << "." << records[k].count << "-"; 
				std::cout << "  ";
			};
			std::cout << std::endl;
		};
		std::cout << std::endl;
	};

	void contract(int i, int j){//contract nodes i and j, removing any self edges
		if(nodeMap.find(i) == nodeMap.end() || nodeMap.find(j) == nodeMap.end())
			return; //Don't contract if one or both of the nodes does not exist
		std::set<int> i_edges = nodeMap[i];//get the edges for ith node
		std::set<int> j_edges = nodeMap[j];//get the edges for jth node
		for(set_it it = j_edges.begin(); it != j_edges.end(); it++){//iterate through node j's edges 
			if( i_edges.find(*it)!=i_edges.end() ){//an edge from j is also an edge in i
				edgeCountInc(i,*it,getEdge(j,*it));//Then incriment the edge count on i's edge
			}
			else if(*it != i){//If we're here, it means that the edge (i,*it) does not exist, so add it if its not a self edge
				//std::cout << "At edge adder " << i << " , " << *it << ", " << j << std::endl;
				addEdge(i,*it,getEdge(j,*it));//Create an edge with the current edge record
			}
			//else, do nothing because the current edge is a self edge, so we ignore it	
		}
		deleteNode(j);//Delete the jth node
	}
};

int main(int argc, char **argv)
{

	std::srand(std::time(0));//set a random seed
	Graph x = Graph();	
	x.minCutSer();
};


