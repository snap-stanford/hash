CXXFLAGS += -std=c++98 -g -Wall
LDFLAGS += -lrt

SNAP = ../snap/snap-core
GLIB = ../snap/glib-core

all: rand-test vec-test hash-test
opt: CXXFLAGS += -O3
opt: LDFLAGS += -O3
opt: rand-test vec-test hash-test

rand-test: rand-test.cc Snap.o
	g++ -o rand-test rand-test.cc Snap.o -I$(GLIB) -I$(SNAP) $(LDFLAGS)
vec-test: vec-test.cc Snap.o
	g++ -o vec-test vec-test.cc Snap.o -I$(GLIB) -I$(SNAP) $(LDFLAGS)
hash-test: hash-test.cc Snap.o
	g++ -o hash-test hash-test.cc Snap.o -I$(GLIB) -I$(SNAP) $(LDFLAGS)
Snap.o: 
	g++ -c $(CXXFLAGS) $(SNAP)/Snap.cpp -I$(GLIB) -I$(SNAP)
clean:	
	rm -f *.o *.png *.plt *.tab $(NAME) $(NAME).exe
