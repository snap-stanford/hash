CXXFLAGS += -fopenmp -std=c++98 -g -Wall
LDFLAGS += -lrt

SNAP = ../snap/snap-core
GLIB = ../snap/glib-core

all: vec-test-01 vec-test-02-openmp vec-test-03-openmp hash-test-01 hash-test-02-openmp
opt: CXXFLAGS += -O3
opt: LDFLAGS += -O3
opt: vec-test-01 vec-test-02-openmp vec-test-03-openmp hash-test-01 hash-test-02-openmp

vec-test-01: vec-test-01 Snap.o
	g++ $(CXXFLAGS) -o vec-test-01 vec-test-01.cc Snap.o -I$(GLIB) -I$(SNAP) $(LDFLAGS)
vec-test-02-openmp: vec-test-02-openmp Snap.o
	g++ $(CXXFLAGS) -o vec-test-02-openmp vec-test-02-openmp.cc Snap.o -I$(GLIB) -I$(SNAP) $(LDFLAGS)
vec-test-03-openmp: vec-test-03-openmp Snap.o
	g++ $(CXXFLAGS) -o vec-test-03-openmp vec-test-03-openmp.cc Snap.o -I$(GLIB) -I$(SNAP) $(LDFLAGS)
hash-test-01: hash-test-01 Snap.o
	g++ $(CXXFLAGS) -o hash-test-01 hash-test-01.cc Snap.o -I$(GLIB) -I$(SNAP) $(LDFLAGS)
hash-test-02-openmp: hash-test-02-openmp Snap.o
	g++ $(CXXFLAGS) -o hash-test-02-openmp hash-test-02-openmp.cc Snap.o -I$(GLIB) -I$(SNAP) $(LDFLAGS)
Snap.o: 
	g++ -c $(CXXFLAGS) $(SNAP)/Snap.cpp -I$(GLIB) -I$(SNAP)
clean:	
	rm -f *.o *.png *.plt *.tab $(NAME) $(NAME).exe
