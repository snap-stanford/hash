#include "../../snap/snap-core/Snap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <iostream>

#include <sys/resource.h>
#include <sys/time.h>
#include <sys/times.h>
// Loads a 10bn element array and tests multithreaded
// sequential access (with the given number of ops
// and gap between them)
// Vector is 10000000000 elements long
TVec<TInt, int64> v;
long long n;

void getstime(struct timeval *tval, char *s)
{
  int msec;
  char *s1;

  s1 = strncpy(s, ctime(&tval->tv_sec)+11, 8);
  s1[8] = '\0';
  msec = (int) (tval->tv_usec / 1000);
  sprintf(s, "%s.%03d", s1, msec);
}


float gettimediff(timespec start, timespec end) {
  timespec temp; 
  if ((end.tv_nsec-start.tv_nsec)<0) {
    temp.tv_sec = end.tv_sec-start.tv_sec-1;
    temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
  } else {
    temp.tv_sec = end.tv_sec-start.tv_sec;
    temp.tv_nsec = end.tv_nsec-start.tv_nsec;
  }
  return (float)temp.tv_sec + ((float)temp.tv_nsec) / 1000000000;
}

void SequentialAccessWorker(long long start, long long ops) {
  long long s = 0;
  for(long long i=0; i<ops; i++) {
    s+= v[start+i];
  }
  char tmp[100];
  sprintf(tmp, "%lld\n", s);
}

void SequentialAccessTest(long long gap, long long ops, int thread_count) {
  if ((gap+ops)*thread_count > n) {
    std::cout<<"args overfill vector"<<std::endl;
    return;
  }
  timespec start_time;
  clock_gettime(CLOCK_REALTIME, &start_time);

  #pragma omp parallel num_threads(thread_count)
  {
    #pragma omp for
    for(int i=0; i<thread_count; i++) {
      SequentialAccessWorker(i*(gap+ops), ops);
    }
  }
  struct rusage rusage;
  getrusage(RUSAGE_SELF, &rusage);

  timespec end_time;
  clock_gettime(CLOCK_REALTIME, &end_time);
  float wtdiff = gettimediff(start_time, end_time);

  struct timeval tval;
  char tstr[64];
  gettimeofday(&tval, 0);
  getstime(&tval, tstr);

  float tall = 
    ((float) (rusage.ru_utime.tv_usec + rusage.ru_stime.tv_usec) / 1000000) +
    ((float) (rusage.ru_utime.tv_sec + rusage.ru_stime.tv_sec));
  float nps = ops / wtdiff;

  printf("__time_sequential_access__,gap, ops, n, npst, cpu(s), wtime_d(s)\n");
  printf("%s, %lld, %lld, %lld, %.0f, %.3f, %.3f\n",
    tstr, gap, ops, ops * thread_count, nps, tall, wtdiff);
}


int main( int argc, char* argv[] ){
  if (argc != 3) {
    std::cout<<"invalid num args"<<std::endl;
    exit(0);
  }
  long long ops;
  int thread_count;
  sscanf(argv[1], "%lld", &ops);
  sscanf(argv[2], "%d", &thread_count);
  n=10000000000;
  long long gap = 10000000;
  TVec<TInt, int64> v(n);
  std::cout<<"vector loaded"<<std::endl;
  SequentialAccessTest(gap, ops, thread_count);
 return 0;
}
