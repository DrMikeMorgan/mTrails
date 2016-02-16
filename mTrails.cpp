#include "mTrails.h"
#include <algorithm>
#include <iterator>

using namespace std;

mTrailSet::mTrailSet(graph* gr)
{
	g = gr;

	trails.resize(g->size());
	edgeData.resize(g->size());
	for(size_t i=0; i<edgeData.size(); ++i)
		edgeData[i].resize(edgeData.size(),0);
}

bool mTrailSet::addEdgeToTrail(int ed, int trail)
{
	if(trails[trail].size() == 0)
	{
		trails[trail].push_back(ed);
		edgeData[trail][ed] = true;
		//cout << "New Trail\n";
		return true;
	}

	//search for duplicate edges!!!!
	for(list<int>::iterator i = trails[trail].begin(); i!= trails[trail].end(); ++i)
		if(*i == ed)
			return false;


	//find the right end
	int front = trails[trail].front();
	int back = trails[trail].back();

	//new edge must be adjacent to an end
	//search to see if either front or back is an adjacent edge

	edgeIterator e;
	for(e = (*g)[ed].begin(); e != (*g)[ed].end(); ++e)
	{
		if(e->dest == back)
		{
			bool doubleback = false;
			if(trails[trail].size()>1)	//check for double back
			{
				edgeIterator e2;
				list<int>::iterator second = trails[trail].end();
				--second;
				--second;
				for(e2 = (*g)[back].begin(); e2 != (*g)[back].end(); ++e2)
					if(e2->dest == *second && e->id == e2->id)
					{
						doubleback = true;
					}
			}

			if(!doubleback)
			{
				edgeData[trail][ed] = true;
				trails[trail].push_back(ed);
				return true;
			} 
		}	
		if(e->dest == front)
		{
			bool doubleback = false;
			if(trails[trail].size()>1)	//check for double back
			{
				edgeIterator e2; 
				list<int>::iterator second = trails[trail].begin();
				++second;
				for(e2 = (*g)[front].begin(); e2 != (*g)[front].end(); ++e2)
					if(e2->dest == *second && e->id == e2->id)
					{
						doubleback = true;
					}
			}

			if(!doubleback)
			{
				trails[trail].push_front(ed);
				edgeData[trail][ed] = true;
				return true;
			}
		}
	}

	//still to do - deal with cycles (although I think this works in the simple case)

	return false;	//edge wasn't adjacent to either end
}

bool mTrailSet::removeEdgeFromTrail(int edge, int trail)
{
	if(trails[trail].front() != edge && trails[trail].back() != edge)
		return false;
	trails[trail].remove(edge);
	edgeData[trail][edge] = false;
	return true;
}

int mTrailSet::newTrailAtEnd(int edge, int trail)
{
	int newTrail = -1;
	for(size_t i=0; i<trails.size(); ++i)
		if(trails[i].size() == 0)
			newTrail = i;

	if(newTrail == -1)
		return -1;

	if(edge == trails[trail].front())	
	{	
		trails[newTrail].push_front(edge);
		edgeData[newTrail][edge] = true;
	}
	else if(trails[trail].back())
	{
		trails[newTrail].push_back(edge);
		edgeData[newTrail][edge] = true;
	}
	else return -1;

	return newTrail;
}

int mTrailSet::newTrailAt(int edge)
{
	int newTrail = -1;
	for(size_t i=0; i<trails.size(); ++i)
		if(trails[i].size() == 0)
			newTrail = i;

	if(newTrail == -1)
		return -1;

	trails[newTrail].push_back(edge);
		edgeData[newTrail][edge] = true;
	return newTrail;
}

bool mTrailSet::feasibleGrow(int trail)
{
	
}

bool mTrailSet::removeTrail(int trail)
{
	double eval1 = evaluate();
	list<int> backUp;
	for(list<int>::iterator it = trails[trail].begin(); it != trails[trail].end(); ++it)
	{
		backUp.push_back(*it);
		edgeData[trail][*it] = false;
	}
	
	trails[trail].clear();
	
	if(evaluate() >= eval1)
	{
		for(list<int>::iterator it = backUp.begin(); it != backUp.end(); ++it)
		{
			trails[trail].push_back(*it);
			edgeData[trail][*it] = true;
		}
		return false;	
	}
	return true;
}

int mTrailSet::removeAllTrails()
{
	int count;
	for(int i=0; i<trails.size(); ++i)
	{
		if(trails[i].size() && removeTrail(i) )
			++count;
	}
	return count;	
}

bool mTrailSet::Check()
{
	for(size_t i=0; i<trails.size(); ++i)
		for(list<int>::iterator j = trails[i].begin(); j!= trails[i].end(); ++j)
			if(!edgeData[i][*j])
				return false;

	for(size_t i=0; i<edgeData.size(); ++i)
		for(size_t j=0; j<edgeData.size(); ++j)
			if(edgeData[i][j] && find(trails[i].begin(),trails[i].end(),j) == trails[i].end())
				return false;

	return true;
}

double mTrailSet::evaluate()
{
	double result = 0;

	for(size_t i=0; i<trails.size(); ++i)
		if(trails[i].size() > 0)
			result += 1.0;				//1 per trail 

	//cout << "stage 1: " << result;
	//some code to count edges with no trail here...

	std::vector<bool> isInTrail(edgeData.size(), false);

	for(size_t i=0; i< edgeData.size(); ++i) 
	{
		for(size_t j=0; j< edgeData.size(); ++j) 
		{
			if(edgeData[j][i])
				isInTrail[i] = true;
		}
		if(!isInTrail[i])
		{
			result += 3*edgeData.size();
		}
	}
	//cout << "  stage 2: " << result;

	//check for duplicate alarm codes - maybe keep record of unused edges to spped things up
	for(size_t i=0; i< edgeData.size(); ++i) //first edge
	{	
		//if(!isInTrail[i])
		//	continue;
		bool matchedOnce = false;
		for(size_t j=i+1; j<edgeData.size(); ++j) //second edge
		{
			//if(!isInTrail[j])
			//	continue;
			bool match = true;
			for(size_t k=0; k<edgeData.size(); ++k) //trail
			{
				//if(trails[k].size() == 0)
				//	continue;
				if(edgeData[k][i] != edgeData[k][j])
				{
					match = false;
					break;
				}
			}	
			if(match)
			{
				if(matchedOnce)
					result += 0.0;
				else
					result += edgeData.size();	//add n*num of matches
				matchedOnce = true;
			}	
		}	
	}
	return result;
}

void mTrailSet::crapHeuristic(int add_aggression)
{
	double Eval = 10000000000;
	double newEval = evaluate();
	//cout << "Initial evaluation: " << newEval << "\n";
	int numTrails = 1;
	std::vector<std::list<int> > bestTrails(trails.size());
	std::vector<std::vector<bool> > bestEdgeData(trails.size(),std::vector<bool>(trails.size()));  
	while (newEval < Eval)
	{
		int thisEval;
		Eval = newEval;
		for(int i=0; i<250; ++i)
		{
			bool success = false;
			for (int i=0;!success && i<120; ++i)
			{
				int which = (rand()%200 < 100 + add_aggression)? 1:0;
				int edg;
				int trail = rand() %(numTrails+1);
				
				if(which && trails[trail].size() != 0)
				{
					//select a valid, adjacent edge
						
					int front = trails[trail].front();
					int back = trails[trail].back();
					size_t number = 0;
					number += (*g)[front].size();
					number += (*g)[back].size();

					edg = rand()%number;
					if(edg < (*g)[front].size())
					{
						std::list<edge>::iterator it = (*g)[front].begin();
						for(size_t ctr = 0; ctr < edg; ++it, ++ctr);
						edg = it->dest;							
					}
					else
					{
						std::list<edge>::iterator it = (*g)[back].begin();
						for(size_t ctr = (*g)[front].size(); ctr < edg; ++it, ++ctr);
						edg = it->dest;							
					}
				}
				else if(!which && trails[trail].size() != 0)
				{
					int what = rand()%2; //front or back
					edg = what ? trails[trail].front() : trails[trail].back();
				}
				else edg = rand()%edgeData.size();
				
				if(which)
				{
					success = addEdgeToTrail(edg,trail);
					if(trail == numTrails && numTrails < trails.size()-1) ++ numTrails;
				}
				else
					success = removeEdgeFromTrail(edg,trail);
			}
			//cout << "Before remove: " << evaluate();
			int rem = removeAllTrails();
			thisEval = evaluate();
			if(thisEval < Eval)
			{
				newEval = thisEval;
				bestTrails = this->trails;
				bestEdgeData = this->edgeData;
			}
			//if(rem)
				//cout << "removed trail\n";
			//cout << *this << ", " << newEval << ":" << Eval << "\n";
		}
		
	}

	this->trails = bestTrails;
	this->edgeData = bestEdgeData;
	
}

ostream& operator << (ostream& o, graph& g)
{
	for(size_t i=0; i<g.size(); ++i)
	{
		for(edgeIterator j = g[i].begin(); j != g[i].end(); ++j)
			o << "(" << j->id << ", " << j->dest << ")";
		o << "\n";
	}
	return o;
} 

ostream& operator << (ostream& o, list<triple>& t)
{
	for(list<triple>::iterator i=t.begin(); i!=t.end(); ++i)
		o << "(" << i->i << "," << i->j << "," << i->k << ") ";
	return o;
}

std::ostream& operator << (std::ostream& o, mTrailSet& m)
{
	for(size_t i=0; i<m.trails.size(); ++i)
		if(m.trails[i].size() > 0)
		{
			o << "Trail " << i << ": ";
			for(list<int>::iterator j=m.trails[i].begin(); j!=m.trails[i].end();++j)
				o << *j << ", ";
			o << "\n";
		}

	for(size_t i=0; i<m.trails.size(); ++i)
	{
		o << "Binary: ";
		for(int j=0; j<m.trails.size(); ++j)
			o << (int) m.edgeData[i][j];
		o << "\n";
	}
	return o;
}

graph getEdgeGraph(graph& g)
{
	graph result;
	int lastEdge = 0;
	for(size_t i=0; i<g.size(); ++i)
	{
		for(edgeIterator j=g[i].begin(); j!= g[i].end(); ++j)
		{
			if(j->id < lastEdge)
				continue;
			list<edge> edgeAdjacencies;
			for(edgeIterator k=g[i].begin(); k!= g[i].end(); ++k)
				if(k->id != j->id)
					edgeAdjacencies.push_back(edge(k->id, i));
			for(edgeIterator k=g[j->dest].begin(); k!= g[j->dest].end(); ++k)
				if(k->id != j->id)
					edgeAdjacencies.push_back(edge(k->id, j->dest));
			result.push_back(edgeAdjacencies);
			++lastEdge;
		}
	}
	return result;
}

list<triple> getTriples(graph& g)
{
	list<triple> result;
	for(size_t i=0; i<g.size(); ++i) //for testing - should be i<g.size()
	{
		if(g[i].size() < 3)
			continue;
		vector<edge> edges(g[i].size());
		copy(g[i].begin(),g[i].end(),edges.begin()); //make 'hard' copy of edge list


		for(size_t j = 0; j < edges.size() - 2; ++j)
			for(size_t k= j+1; k < edges.size() - 1; ++k)
				for(size_t l = k+1; l < edges.size(); ++l)
					result.push_back( triple(edges[j].id, edges[k].id, edges[l].id) );

	}
	return result;
}

graph crapGenerator(int n, float dmax, float prob, std::vector<std::vector<float> >& coords)
{
	graph g(n);
	coords.resize(n);
	for(int i=0; i<n; ++i)
	{
		coords[i].resize(2);
		coords[i][0] = rand()%1000/1000.f;
		coords[i][1] = rand()%1000/1000.f;
	} 
	
	int m=0;	//replace this section with pavan algorithm eventually
	for(int i=0; i<n; ++i)
		for(int j=i+1; j<n; ++j)
			if( (coords[i][0]-coords[j][0])*(coords[i][0]-coords[j][0]) + (coords[i][1]-coords[j][1])*(coords[i][1]-coords[j][1]) < dmax*dmax && rand()%100<70 )
			{
				g[i].push_back(edge(j,m));
				g[j].push_back(edge(i,m++));	
			}

	return g;
}
