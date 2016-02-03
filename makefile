FLTK = $(shell fltk-config --cxx)
CXX = g++
CXXFLAGS = -Wall
FLTKFLAGS = $(shell fltk-config --cxxflags)
LDFLAGS = $(shell fltk-config --ldstaticflags --use-gl)

OBJ = mTrails.o main.o

all: exe

mTrails.o: mTrails.cpp
	$(CXX) -c $< $(CXXFLAGS)

main.o: main.cpp mTrails.o
	$(FLTK) -c $< $(FLTKFLAGS)

exe: $(OBJ)
	$(FLTK) -o $@ $(OBJ) $(LDFLAGS)
