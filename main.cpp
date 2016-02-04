#include <iostream>
#include <cstdlib>
#include <vector>
#include <list>
#include "mTrails.h"

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>

#include <GL/gl.h>

using namespace std;

class MyGlWindow : public Fl_Gl_Window 
{	
	graph* g; 
	GLfloat * vertices;
	GLuint * edges;
	int mby2;
    void draw(); 
public:
    MyGlWindow(int X,int Y,int W,int H,const char*L=0) : g(0), vertices(0), edges(0), Fl_Gl_Window(X,Y,W,H,L) {}
	~MyGlWindow(){if(vertices) delete [] vertices;}
	void setGraph(graph* gr, vector<vector<float> > * c);
};

void MyGlWindow::setGraph(graph* gr, vector<vector<float> > * c)
{
	g = gr;
	vertices = new GLfloat(g->size()*3);
	for(int i=0; i<c->size(); ++i)
	{
		vertices[i*3] = (*c)[i][0] * 400 - 200;
		vertices[i*3+1] = (*c)[i][1] * 400 - 200;
		vertices[i*3+2] = 0;
	}

	mby2 = 0;
	for(size_t i=0; i<gr->size(); ++i)
		for(edgeIterator j = (*g)[i].begin(); j!= (*g)[i].end(); ++j)
			mby2++;

	/*edges = new GLuint(2*mby2);

	size_t idx = 0;	
	for(int i=0; i<g->size(); ++i)
		for(edgeIterator j = (*g)[i].begin(); j!= (*g)[i].end(); ++j)
		{
			edges[idx++] = i;
			edges[idx++] = j->dest;
		}*/
}

void MyGlWindow::draw()
{
        if (!valid()) 
		{
            glLoadIdentity();
            glViewport(0,0,w(),h());
            glOrtho(-w(),w(),-h(),h(),-1,1);
        }
        
        if(g)
		{
			glEnable(GL_POINT_SMOOTH);
		    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		    glEnable(GL_LINE_SMOOTH);
		    glEnable (GL_BLEND);
		    glShadeModel (GL_SMOOTH);
		    glPointSize(10.0);
		    glLineWidth(0.3);

			glEnableClientState(GL_VERTEX_ARRAY);
		    glVertexPointer(3,GL_FLOAT,0,vertices);

			glDrawArrays(GL_POINTS, 0, g->size());
			//glDrawElements(GL_LINES, 12, GL_UNSIGNED_INT, edges);

			glDisableClientState(GL_VERTEX_ARRAY);			
		}
}




int main() {
    //Fl_Window win(500, 300, "OpenGL X");
    //MyGlWindow mygl(10, 10, win.w()-20, win.h()-20);
    

	const int n = 10;
	const float dmax = 0.5;
	graph g;
	vector<vector<float> > coords;
	
	g = crapGenerator(n,dmax,0.7,coords);
	//mygl.setGraph(&g,&coords);

	cout << g << "\n\n";
	graph eg = getEdgeGraph(g);
	cout << eg;

	mTrailSet mt(&eg);

	int addseq[] = {6,8,11,5,7};

	for(int i=0; i<sizeof(addseq)/sizeof(int); ++i)
		cout << "\nAdding " << addseq[i] << "... " << (mt.addEdgeToTrail(addseq[i],0) ? "succeeded ":"failed ") << "\n" << mt;
	

    //win.resizable(mygl);

    //win.show();
    return 0; //(Fl::run());
}


