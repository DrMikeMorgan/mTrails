#ifndef MTRAILS_H
#define MTRAILS_H


#include <iostream>
#include <cstdlib>
#include <vector>
#include <list>



struct triple
{
	triple(int a, int b, int c):i(a),j(b),k(c){}
	int i,j,k;
};

struct edge
{
	edge(){}
	edge(int i, int j):dest(i),id(j){}
	int dest;
	int id;
};

typedef std::vector<std::list<edge> > graph;
typedef std::list<edge>::iterator edgeIterator;

class mTrailSet
{
	//int nonempty;
	graph* g; //edge graph?
	std::vector<std::list<int> > trails;
	std::vector<std::vector<bool> > edgeData;  
public:
	mTrailSet(){}
	mTrailSet(graph* g);
	void crapHeuristic(int add_aggression);
	bool addEdgeToTrail(int edge, int trail);
	bool removeEdgeFromTrail(int edge, int trail);
	int newTrailAtEnd(int edge, int trail); //returns index of new trail (-1 otherwise)
	int newTrailAt(int edge);	//returns index of new trail (-1 otherwise)
	bool removeTrail(int trail);
	int removeAllTrails();
	bool feasibleGrow(int trail);
	bool Check();
	double evaluate();
	friend std::ostream& operator << (std::ostream& o, mTrailSet& m);
};


std::ostream& operator << (std::ostream& o, graph& g);
std::ostream& operator << (std::ostream& o, std::list<triple>& t);

graph getEdgeGraph(graph& g);
std::list<triple> getTriples(graph& g);
graph crapGenerator(int n, float dmax, float prob, std::vector<std::vector<float> >& coords);

#endif
