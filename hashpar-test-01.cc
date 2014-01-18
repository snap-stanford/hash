/*
  This program evaluates the throughput of multi-threaded insert of THashPar
  for 32-bit integer keys and values. The code tests multi-threaded insert of
  1,2,4,10,20,40,80 and 160 threads. The program takes in one argument which is
  the number of elements.
*/
#include "../snap/snap-core/Snap.h"
#include "hashpar/hashpar.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sys/resource.h>
#include <sys/time.h>
#include <sys/times.h>

int work_size;
TIntPrV v;
THashPar<TInt, TInt> h;
long long n;
long long num_operation = 100000000;

void getstime(struct timeval *tval, char *s)
{
  int msec;
  char *s1;

  // printf("2. %d %d\n", tval->tv_sec, tval->tv_usec);
  s1 = strncpy(s, ctime(&tval->tv_sec)+11, 8);
  s1[8] = '\0';
  msec = (int) (tval->tv_usec / 1000);
  sprintf(s, "%s.%03d", s1, msec);
}

float getcputime() {
  struct rusage rusage;
  float result;

  getrusage(RUSAGE_SELF, &rusage);

  result =
    ((float) (rusage.ru_utime.tv_usec + rusage.ru_stime.tv_usec) / 1000000) +
    ((float) (rusage.ru_utime.tv_sec + rusage.ru_stime.tv_sec));
  return result;
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

void* InsertWorker(void* args) {
  int id = *(int *)args;
  
  int start = id * work_size;
  int end = (id + 1) * work_size;
  for (long long i = start; i < end && i < n; i++) {
    h.ThreadSafeAddDat(v[i].Val1, v[i].Val2, false);
  }
}

void InsertionTest(int num_threads) {
  printf("InsertionTest(%d threads)\n", num_threads);

  work_size = n / num_threads;
  
  // Begin Test
  timespec start_time;
  clock_gettime(CLOCK_REALTIME, &start_time);
  float fs = getcputime();

  h.Gen(n);
  if (num_threads == 1) {
    for (long long i = 0; i < n; i++) {
      h.ThreadSafeAddDat(v[i].Val1, v[i].Val2, false);
    }
  } else {
    int id[num_threads];
    pthread_t threads[num_threads];
    for (int i = 0; i < num_threads; i++) {
      id[i] = i;
      pthread_create(threads + i, NULL, InsertWorker, &id[i]);
    }
    
    for (int i = 0; i < num_threads; i++) {
      pthread_join(threads[i], NULL);
    }
  }

  // Calculate Stat
  struct rusage rusage;
  getrusage(RUSAGE_SELF, &rusage);

  timespec end_time;
  clock_gettime(CLOCK_REALTIME, &end_time);
  float fe = getcputime();
  float tdiff = fe - fs;
  float wtdiff = gettimediff(start_time, end_time);

  struct timeval tval;
  char tstr[64];
  gettimeofday(&tval, 0);
  getstime(&tval, tstr);

  float tall = 
    ((float) (rusage.ru_utime.tv_usec + rusage.ru_stime.tv_usec) / 1000000) +
    ((float) (rusage.ru_utime.tv_sec + rusage.ru_stime.tv_sec));
  float nps = n/wtdiff;

  printf("%s\t__time_insertion__\tn %lld\tnps %.0f\tcpu(s) %.3f\tcpu_d(s) %.3f\tnum_thread(s) %d\n",
	 tstr, n, nps, tall, wtdiff, num_threads);
}

void Verify() {
  for (int i = 0; i < n; i++) {
    if (v[i].Val2 != h.GetDat(v[i].Val1)) {
      printf("Error\n");
      exit(0);
    }
  }
}

void GenerateTest() {
  struct drand48_data buffer;
  srand48_r(time(NULL), &buffer);

  v.Gen(n);
  
  // Construct random key sequence
  long int r;
  
  for (long long i = 0; i < n; i++) {
    lrand48_r(&buffer, &r);
    v[i].Val1 = i;
    v[i].Val2 = r;
  }

  TRnd rnd;
  v.Shuffle(rnd);
}

int main( int argc, char* argv[] ){
  if (argc != 2) {
    exit(0);
  }
  sscanf(argv[1], "%lld", &n);

  GenerateTest();

  // Insertion Test
  int a[] = {1,2,4,10,20,40,80,160};
  for (int i = 0; i < 8; i++) {
    h.Clr();
    InsertionTest(a[i]);
  }
  h.ComputeLen();

  // Verification
  Verify();
  return 0;
}
