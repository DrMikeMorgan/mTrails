#include "mTrails.h"
#include <algorithm>

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
		return true;
	}
	//find the right end
	int front = trails[trail].front();
	int back = trails[trail].back();

	//new edge must be adjacent to an end
	//search to see if either front or back is an adjacent edge

	edgeIterator e;
	for(e = (*g)[ed].begin(); e != (*g)[ed].end(); ++e)
	{
		if(e->dest == front)
		{
			if(trails[trail].size()>1)	//check for double back
			{
				edgeIterator e2; 
				list<int>::iterator second = trails[trail].begin();
				++second;
				cout << "scndend: " << *second << "\n";
				for(e2 = (*g)[front].begin(); e2 != (*g)[front].end(); ++e2)
					if(e2->dest == *second && e->id == e2->id)
						return false;
			}

			trails[trail].push_front(ed);
			edgeData[trail][ed] = true;
			return true;
		}
		if(e->dest == back)
		{
			if(trails[trail].size()>1)	//check for double back
			{
				edgeIterator e2;
				list<int>::iterator second = trails[trail].end();
				--second;
				--second;
				cout << "scndend: " << *second << "\n";
				for(e2 = (*g)[back].begin(); e2 != (*g)[back].end(); ++e2)
					if(e2->dest == *second && e->id == e2->id)
						return false;
			}

			edgeData[trail][ed] = true;
			trails[trail].push_back(ed);
			return true;
		} 
	}

	//still to do - deal with cycles (although I think this works in the simple case)

	return false;	//edge wasn't adjacent to either end
}

bool mTrailSet::Check()
{
	
}

/*void mTrailSet::CrapHeuristic()
{
	//work out max trails
	int minTrails = 0;
	for(;pow(2,minTrails) < g-<size(); ++minTrails);

	
}*/

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
	for(int i=0; i<m.trails.size(); ++i)
		if(m.trails[i].size() > 0)
		{
			o << "Trail " << i << ": ";
			for(list<int>::iterator j=m.trails[i].begin(); j!=m.trails[i].end();++j)
				o << *j << ", ";
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
