CXXFLAGS += -std=c++98 -g -Wall
LDFLAGS += -lrt

SNAP = ../snap/snap-core
GLIB = ../snap/glib-core
PHASHLIB = phash

all: rand-test vec-test-01 vec-test-02 vec-test-03 string-hash-test-01 string-hash-test-02 string-hash-test-03 hash-test-01 hash-test-02 hash-test-03 hash-test-04 hash-test-05 two-vec-test-01 phash-test-01 phash-test-02
opt: CXXFLAGS += -O3
opt: LDFLAGS += -O3
opt: rand-test vec-test-01 vec-test-02 vec-test-03 string-hash-test-01 string-hash-test-02 string-hash-test-03 hash-test-01 hash-test-02 hash-test-03 hash-test-04 hash-test-05 two-vec-test-01 phash-test-01 phash-test-02

rand-test: rand-test.cc Snap.o
	g++ -o rand-test rand-test.cc Snap.o -I$(GLIB) -I$(SNAP) $(LDFLAGS)
vec-test-01: vec-test-01.cc Snap.o
	g++ -o vec-test-01 vec-test-01.cc Snap.o -I$(GLIB) -I$(SNAP) $(LDFLAGS)
vec-test-02: vec-test-02.cc Snap.o
	g++ -o vec-test-02 vec-test-02.cc Snap.o -I$(GLIB) -I$(SNAP) $(LDFLAGS)
vec-test-03: vec-test-03.cc Snap.o
	g++ -o vec-test-03 vec-test-03.cc Snap.o -I$(GLIB) -I$(SNAP) $(LDFLAGS)
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
phash-test-01: phash-test-01.cc Snap.o phash.o
	g++ -o phash-test-01 phash-test-01.cc Snap.o phash.o -I$(GLIB) -I$(SNAP) -I$(PHASHLIB) $(LDFLAGS)
phash-test-02: phash-test-02.cc Snap.o phash.o
	g++ -o phash-test-02 phash-test-02.cc Snap.o phash.o -I$(GLIB) -I$(SNAP) -I$(PHASHLIB) $(LDFLAGS)
Snap.o: 
	g++ -c $(CXXFLAGS) $(SNAP)/Snap.cpp -I$(GLIB) -I$(SNAP)
phash.o:
	g++ -c $(CXXFLAGS) $(PHASHLIB)/phash.cc -I$(GLIB) -I$(SNAP)
clean:	
	rm -f *.o *.png *.plt *.tab $(NAME) $(NAME).exe
