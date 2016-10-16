CXXFLAGS += -fopenmp -std=c++98 -g -Wall
LDFLAGS += -lrt

SNAP = ../../snap/snap-core
GLIB = ../../snap/glib-core

all: vec-test-01 vec-test-02 vec-test-03 hash-test-01 hash-test-02 vec-test-02-indiv_thr hash-test-02-indiv_thr
opt: CXXFLAGS += -O3
opt: LDFLAGS += -O3
opt: vec-test-01 vec-test-02 vec-test-03 hash-test-01 hash-test-02 vec-test-02-indiv_thr hash-test-02-indiv_thr

vec-test-01: vec-test-01.cc Snap.o
	g++ $(CXXFLAGS) -o vec-test-01 vec-test-01.cc Snap.o -I$(GLIB) -I$(SNAP) $(LDFLAGS)
vec-test-02: vec-test-02.cc Snap.o
	g++ $(CXXFLAGS) -o vec-test-02 vec-test-02.cc Snap.o -I$(GLIB) -I$(SNAP) $(LDFLAGS)
vec-test-03: vec-test-03.cc Snap.o
	g++ $(CXXFLAGS) -o vec-test-03 vec-test-03.cc Snap.o -I$(GLIB) -I$(SNAP) $(LDFLAGS)
hash-test-01: hash-test-01.cc Snap.o
	g++ $(CXXFLAGS) -o hash-test-01 hash-test-01.cc Snap.o -I$(GLIB) -I$(SNAP) $(LDFLAGS)
hash-test-02: hash-test-02.cc Snap.o
	g++ $(CXXFLAGS) -o hash-test-02 hash-test-02.cc Snap.o -I$(GLIB) -I$(SNAP) $(LDFLAGS)
vec-test-02-indiv_thr: vec-test-02-indiv_thr.cc Snap.o
	g++ $(CXXFLAGS) -o vec-test-02-indiv_thr vec-test-02-indiv_thr.cc Snap.o -I$(GLIB) -I$(SNAP) $(LDFLAGS)
hash-test-02-indiv_thr: hash-test-02-indiv_thr.cc Snap.o
	g++ $(CXXFLAGS) -o hash-test-02-indiv_thr hash-test-02-indiv_thr.cc Snap.o -I$(GLIB) -I$(SNAP) $(LDFLAGS)
Snap.o: 
	g++ -c $(CXXFLAGS) $(SNAP)/Snap.cpp -I$(GLIB) -I$(SNAP)
clean:	
	rm -f *.o *.png *.plt *.tab $(NAME) $(NAME).exe
