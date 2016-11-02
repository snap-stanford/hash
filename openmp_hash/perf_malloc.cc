#include "../../snap/snap-core/Snap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <iostream>

#include <sys/resource.h>
#include <sys/time.h>
#include <sys/times.h>

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


char* perform_malloc(long long size) {
  timespec start_time;
  clock_gettime(CLOCK_REALTIME, &start_time);
  char* buf = new char[size];
  timespec end_time;
  clock_gettime(CLOCK_REALTIME, &end_time);
  float wtdiff = gettimediff(start_time, end_time);
  std::cout<<"malloc: " << wtdiff <<std::endl;
  return buf;
}

void perform_free(char* buf) {
  timespec start_time;
  clock_gettime(CLOCK_REALTIME, &start_time);
  delete[] buf;
  timespec end_time;
  clock_gettime(CLOCK_REALTIME, &end_time);
  float wtdiff = gettimediff(start_time, end_time);
  std::cout<<"free: " << wtdiff <<std::endl;
}

void perform_tvec(long long size) {
  timespec start_time;
  clock_gettime(CLOCK_REALTIME, &start_time);
  TVec<TInt, int64> v(size);
  timespec end_time;
  clock_gettime(CLOCK_REALTIME, &end_time);
  float wtdiff = gettimediff(start_time, end_time);
  v[0] = 0;
  timespec start_time;
  clock_gettime(CLOCK_REALTIME, &start_time);
  ~v;
  timespec end_time;
  clock_gettime(CLOCK_REALTIME, &end_time);
  float wtdiff = gettimediff(start_time, end_time);
}

int main( int argc, char* argv[] ){
  if (argc != 2) {
    std::cout<<"invalid num args"<<std::endl;
    exit(0);
  }
  long long max_size;
  sscanf(argv[1], "%lld", &max_size);
  size_t elem_size = sizeof(TInt);
  for (long long i=10000; i<= max_size; i=i*10) {
    std::cout<<i<<std::endl;
    long long elems = i/elem_size;
    for (int j = 0; j<5; j++) {
      char* buf = perform_malloc(i);
      char[0] = 0;
      perform_free(buf);
      perform_tvc(elems);
    }
  }
 return 0;
}