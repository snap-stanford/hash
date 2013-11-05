#include "../snap/snap-core/Snap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sys/resource.h>
#include <sys/time.h>
#include <sys/times.h>

TVec<TInt, int64> v1, v2;
long long n;
long long num_operation = 10000000;
float random_time_per_elem;

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

void Append() {
  struct drand48_data buffer;
  srand48_r(time(NULL), &buffer);

  v1.Reserve(n, n);
  for (long long i = 0; i < n; i++) {
    v1[i] = i;
  }

  TRnd rnd;
  v1.Shuffle(rnd);

  long int r;
  for (long long i = 0; i < n; i++) {
    lrand48_r(&buffer, &r);
    v2.Add(r);
  }
}

void* RandomAccessWorker(void* args) {
  struct drand48_data buffer;
  srand48_r(time(NULL), &buffer);

  long long s = 0;
  long int r = 0;
  for (long long i = 0; i < num_operation; i++) {
    lrand48_r(&buffer, &r);
    long long ind = v1[r % n];
    s += v2[ind];
  }

  char tmp[100];
  sprintf(tmp, "%lld\n", s);
}

void RandomAccessTest(int num_threads) {
  struct drand48_data buffer;
  srand48_r(time(NULL), &buffer);

  timespec start_time;
  clock_gettime(CLOCK_REALTIME, &start_time);
  float fs = getcputime();

  if (num_threads == 1) {
    long int r;
    long long s = 0;
    for (long long i = 0; i < num_operation; i++) {
      lrand48_r(&buffer, &r);
      long long ind = v1[r % n];
      s += v2[ind];
    }
    char tmp[100];
    sprintf(tmp, "%lld\n", s);
  } else {
    pthread_t threads[num_threads];
    for (int i = 0; i < num_threads; i++) {
      pthread_create(threads + i, NULL, RandomAccessWorker, NULL);
    }
    
    for (int i = 0; i < num_threads; i++) {
      pthread_join(threads[i], NULL);
    }
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
  float nps = 2 * num_operation / wtdiff;

  printf("%s\t__time_random_access__\tn %lld\tnpst %.0f\tcpu(s) %.3f\twtime_d(s) %.3f\n",
    tstr, 2 * num_operation * num_threads, nps, tall, wtdiff);
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
  if (argc != 2) {
    exit(0);
  }
  sscanf(argv[1], "%lld", &n);

  CalcRandomTime();

  Append();

  int a[] = {1,2,4,10,20,40,80,160};
  for (int i = 0; i < 8; i++) {
    RandomAccessTest(a[i]);
  }

  return 0;
}
