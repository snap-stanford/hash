#include "../../snap/snap-core/Snap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sys/resource.h>
#include <sys/time.h>
#include <sys/times.h>
// Loads a 10bn element array and tests multithreaded
// sequential access (with the given number of ops
// and gap between them)
TVec<TInt, int64> v;
long long n;
int num_threads = 144;

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
  timespec start_time;
  clock_gettime(CLOCK_REALTIME, &start_time);

  #pragma omp parallel num_threads(num_threads)
  {
    #pragma omp for
    for(int i=0; i<num_threads; i++) {
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

  printf("__time_sequential_access__,n, npst, cpu(s), wtime_d(s)\n");
  printf("%s, %lld, %.0f, %.3f, %.3f\n",
    tstr, ops * thread_count, nps, tall, wtdiff);
}


int main( int argc, char* argv[] ){
  if (argc != 5) {
    exit(0);
  }
  TStr filename(argv[4]);
  TFIn instream(filename);
  v.Load(instream);
  n = v.Len();

  long long gap; //elements between threads
  sscanf(argv[1], "%lld", &gap);
  long long ops; //operations per thread
  sscanf(argv[2], "%lld", &ops);
  int num_threads; //number of threads
  sscanf(argv[3], "%d", &num_threads);
  if ((gap+ops)*num_threads > n) {
    exit(0);
  }
  SequentialAccessTest(gap, ops, num_threads);

 return 0;
}
