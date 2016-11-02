CXXFLAGS += -fopenmp -std=c++98 -g -Wall
LDFLAGS += -lrt

SNAP = ../../snap/snap-core
GLIB = ../../snap/glib-core

all: vec-test-01 vec-test-02 vec-test-03 hash-test-01 hash-test-02 vec-test-02-indiv_thr hash-test-02-indiv_thr make_vector load_vector_test_thr vec_seq perf_vec_2
opt: CXXFLAGS += -O3
opt: LDFLAGS += -O3
opt: vec-test-01 vec-test-02 vec-test-03 hash-test-01 hash-test-02 vec-test-02-indiv_thr hash-test-02-indiv_thr make_vector load_vector_test_thr vec_seq perf_vec_2

perf_vec_2: perf_vec_2.cc Snap.o
	g++ $(CXXFLAGS) -o perf_vec_2 perf_vec_2.cc Snap.o -I$(GLIB) -I$(SNAP) $(LDFLAGS)
vec_seq: vec_seq.cc Snap.o
	g++ $(CXXFLAGS) -o vec_seq vec_seq.cc Snap.o -I$(GLIB) -I$(SNAP) $(LDFLAGS)
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
make_vector: make_vector.cc Snap.o
	g++ $(CXXFLAGS) -o make_vector make_vector.cc Snap.o -I$(GLIB) -I$(SNAP) $(LDFLAGS)
load_vector_test_thr: load_vector_test_thr.cc Snap.o
	g++ $(CXXFLAGS) -o load_vector_test_thr load_vector_test_thr.cc Snap.o -I$(GLIB) -I$(SNAP) $(LDFLAGS)
Snap.o: 
	g++ -c $(CXXFLAGS) $(SNAP)/Snap.cpp -I$(GLIB) -I$(SNAP)
clean:	
	rm -f *.o *.png *.plt *.tab $(NAME) $(NAME).exe
