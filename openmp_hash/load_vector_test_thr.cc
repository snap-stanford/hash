
#include "../../snap/snap-core/Snap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sys/resource.h>
#include <sys/time.h>
#include <sys/times.h>

TVec<TInt, int64> v;
long long n;
long long num_operation = 10000000;
float random_time_per_elem;

void getstime(struct timeval *tval, char *s)
{
  int msec;
  char *s1;

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

void RandomAccessWorker() {
  struct drand48_data buffer;
  srand48_r(time(NULL), &buffer);

  long long s = 0;
  long int r = 0;
  for (long long i = 0; i < num_operation; i++) {
    lrand48_r(&buffer, &r);
    s += v[r % n];
  }

  char tmp[100];
  sprintf(tmp, "%lld\n", s);
}

void RandomAccessTest(int thread_count) {
  struct drand48_data buffer;
  srand48_r(time(NULL), &buffer);

  timespec start_time;
  clock_gettime(CLOCK_REALTIME, &start_time);
  float fs = getcputime();

  if (thread_count == 1) {
    long int r;
    long long s = 0;
    for (long long i = 0; i < num_operation; i++) {
      lrand48_r(&buffer, &r);
      s += v[r % n];
    }
    char tmp[100];
    sprintf(tmp, "%lld\n", s);
  } else {
    #pragma omp parallel num_threads(thread_count)
    RandomAccessWorker();
  }

  struct rusage rusage;
  getrusage(RUSAGE_SELF, &rusage);

  timespec end_time;
  clock_gettime(CLOCK_REALTIME, &end_time);
  float fe = getcputime();
  float tdiff = fe - fs - random_time_per_elem * num_operation;
  float wtdiff = gettimediff(start_time, end_time) - random_time_per_elem * num_operation;

  struct timeval tval;
  char tstr[64];
  gettimeofday(&tval, 0);
  getstime(&tval, tstr);

  float tall = 
    ((float) (rusage.ru_utime.tv_usec + rusage.ru_stime.tv_usec) / 1000000) +
    ((float) (rusage.ru_utime.tv_sec + rusage.ru_stime.tv_sec));
  float nps = num_operation / wtdiff;

  printf("%s\t__time_random_access__\tn %lld\tnpst %.0f\tcpu(s) %.3f\twtime_d(s) %.3f\n",
    tstr, num_operation * thread_count, nps, tall, wtdiff);
}

void CalcRandomTime() {
  struct drand48_data buffer;
  srand48_r(time(NULL), &buffer);

  float fs = getcputime();

  long long N = 1000000000;
  long long s = 0;
  long int r;
  for (long long i = 0; i < N; i++) {
    lrand48_r(&buffer, &r);
    s += r;
  }

  float fe = getcputime();
  float tdiff = fe - fs;

  random_time_per_elem = tdiff / N;
}

int main( int argc, char* argv[] ){
  if (argc != 3) {
    exit(0);
  }
  int num_threads;
  sscanf(argv[1], "%d", &num_threads);
  TStr filename(argv[2]);
  TFIn instream(filename);
  v.Load(instream);
  n = v.Len();
  CalcRandomTime();
  RandomAccessTest(num_threads);

 return 0;
}
