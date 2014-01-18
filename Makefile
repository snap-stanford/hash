CXXFLAGS += -std=c++98 -g -Wall
LDFLAGS += -lrt

SNAP = ../snap/snap-core
GLIB = ../snap/glib-core
HASHPARLIB = hashpar

all: rand-test vec-test-01 vec-test-02 vec-test-03 vec-test-04 string-hash-test-01 string-hash-test-02 string-hash-test-03 hash-test-01 hash-test-02 hash-test-03 hash-test-04 hash-test-05 two-vec-test-01 hashpar-test-01 hashpar-test-02 hashpar-test-03
opt: CXXFLAGS += -O3
opt: LDFLAGS += -O3
opt: rand-test vec-test-01 vec-test-02 vec-test-03 vec-test-04 string-hash-test-01 string-hash-test-02 string-hash-test-03 hash-test-01 hash-test-02 hash-test-03 hash-test-04 hash-test-05 two-vec-test-01 hashpar-test-01 hashpar-test-02 hashpar-test-03

rand-test: rand-test.cc Snap.o
	g++ -o rand-test rand-test.cc Snap.o -I$(GLIB) -I$(SNAP) $(LDFLAGS)
vec-test-01: vec-test-01.cc Snap.o
	g++ -o vec-test-01 vec-test-01.cc Snap.o -I$(GLIB) -I$(SNAP) $(LDFLAGS)
vec-test-02: vec-test-02.cc Snap.o
	g++ -o vec-test-02 vec-test-02.cc Snap.o -I$(GLIB) -I$(SNAP) $(LDFLAGS)
vec-test-03: vec-test-03.cc Snap.o
	g++ -o vec-test-03 vec-test-03.cc Snap.o -I$(GLIB) -I$(SNAP) $(LDFLAGS)
vec-test-04: vec-test-04.cc Snap.o
	g++ -o vec-test-04 vec-test-04.cc Snap.o -I$(GLIB) -I$(SNAP) $(LDFLAGS)
string-hash-test-01: string-hash-test-01.cc Snap.o
	g++ -o string-hash-test-01 string-hash-test-01.cc Snap.o -I$(GLIB) -I$(SNAP) $(LDFLAGS)
string-hash-test-02: string-hash-test-02.cc Snap.o
	g++ -o string-hash-test-02 string-hash-test-02.cc Snap.o -I$(GLIB) -I$(SNAP) $(LDFLAGS)
string-hash-test-03: string-hash-test-03.cc Snap.o
	g++ -o string-hash-test-03 string-hash-test-03.cc Snap.o -I$(GLIB) -I$(SNAP) $(LDFLAGS)
hash-test-01: hash-test-01.cc Snap.o
	g++ -o hash-test-01 hash-test-01.cc Snap.o -I$(GLIB) -I$(SNAP) $(LDFLAGS)
hash-test-02: hash-test-02.cc Snap.o
	g++ -o hash-test-02 hash-test-02.cc Snap.o -I$(GLIB) -I$(SNAP) $(LDFLAGS)
hash-test-03: hash-test-03.cc Snap.o
	g++ -o hash-test-03 hash-test-03.cc Snap.o -I$(GLIB) -I$(SNAP) $(LDFLAGS)
hash-test-04: hash-test-04.cc Snap.o
	g++ -o hash-test-04 hash-test-04.cc Snap.o -I$(GLIB) -I$(SNAP) $(LDFLAGS)
hash-test-05: hash-test-05.cc Snap.o
	g++ -o hash-test-05 hash-test-05.cc Snap.o -I$(GLIB) -I$(SNAP) $(LDFLAGS)
two-vec-test-01: two-vec-test-01.cc Snap.o
	g++ -o two-vec-test-01 two-vec-test-01.cc Snap.o -I$(GLIB) -I$(SNAP) $(LDFLAGS)
hashpar-test-01: hashpar-test-01.cc Snap.o hashpar.o
	g++ -o hashpar-test-01 hashpar-test-01.cc Snap.o hashpar.o -I$(GLIB) -I$(SNAP) -I$(HASHPARLIB) $(LDFLAGS)
hashpar-test-02: hashpar-test-02.cc Snap.o hashpar.o
	g++ -o hashpar-test-02 hashpar-test-02.cc Snap.o hashpar.o -I$(GLIB) -I$(SNAP) -I$(HASHPARLIB) $(LDFLAGS)
hashpar-test-03: hashpar-test-03.cc Snap.o hashpar.o
	g++ -o hashpar-test-03 hashpar-test-03.cc Snap.o hashpar.o -I$(GLIB) -I$(SNAP) -I$(HASHPARLIB) $(LDFLAGS)
Snap.o: 
	g++ -c $(CXXFLAGS) $(SNAP)/Snap.cpp -I$(GLIB) -I$(SNAP)
hashpar.o:
	g++ -c $(CXXFLAGS) $(HASHPARLIB)/hashpar.cc -I$(GLIB) -I$(SNAP)
clean:	
	rm -f *.o *.png *.plt *.tab $(NAME) $(NAME).exe
