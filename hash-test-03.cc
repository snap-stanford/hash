/*
  This program uses a local forked SNAP at https://github.com/chantat/snap. This program
  evaluates the portion of time taken for each step of the THash's AddKey operation for
  n 32-bit integer keys and values. To evaluate the total time of a step, uncomment its
  section in the AddKey function. The program takes in one argument which is n.
*/
#include "../snap/snap-core/Snap.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sys/resource.h>
#include <sys/time.h>
#include <sys/times.h>

THash<TInt, TInt> h;
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

void InsertionTest(bool GetChainTraversalDistribution) {
  // Construct random key sequence
  TIntV v(n);
  for (long long i = 0; i < n; i++) {
    v[i] = i;
  }

  TRnd rnd;
  v.Shuffle(rnd);

  struct drand48_data buffer;
  srand48_r(time(NULL), &buffer);

  // Begin Test
  float fs = getcputime();

  long int r;
  for (long long i = 0; i < n; i++) {
    lrand48_r(&buffer, &r);
    h.AddDat(v[i], r);
  }

  // Calculate Stat
  struct rusage rusage;
  getrusage(RUSAGE_SELF, &rusage);

  float fe = getcputime();
  float tdiff = fe - fs;

  struct timeval tval;
  char tstr[64];
  gettimeofday(&tval, 0);
  getstime(&tval, tstr);

  float tall = 
    ((float) (rusage.ru_utime.tv_usec + rusage.ru_stime.tv_usec) / 1000000) +
    ((float) (rusage.ru_utime.tv_sec + rusage.ru_stime.tv_sec));
  float nps = n/tdiff;

  printf("%s\t__time_insertion__\tn %lld\tnps %.0f\tcpu(s) %.3f\tcpu_d(s) %.3f\n",
    tstr, n, nps, tall, tdiff);
}

int main( int argc, char* argv[] ){
  if (argc != 2) {
    exit(0);
  }
  sscanf(argv[1], "%lld", &n);

  // Insertion Test
  InsertionTest(false);
  return 0;
}
