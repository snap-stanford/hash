#include "../../snap/snap-core/Snap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <iostream>

#include <sys/resource.h>
#include <sys/time.h>
#include <sys/times.h>

int repeat_times = 3;

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

void perform_malloc(long long size) {
  float total_malloc = 0;
  float total_free = 0;
  timespec start_time;
  timespec end_time;
  for (int i=0; i<repeat_times; i++) {
    clock_gettime(CLOCK_REALTIME, &start_time);
    char* buf = new char[size];
    clock_gettime(CLOCK_REALTIME, &end_time);
    total_malloc += gettimediff(start_time, end_time);
    buf[0] = 0;
    clock_gettime(CLOCK_REALTIME, &start_time);
    delete[] buf;
    clock_gettime(CLOCK_REALTIME, &end_time);
    total_malloc += gettimediff(start_time, end_time);
  }
  std::cout << "malloc: " << total_malloc/repeat_times << " free: " << total_free/repeat_times << std::endl;
}

void perform_tvec(long long size) {
  float total_alloc = 0;
  float total_destroy = 0;
  timespec start_time;
  timespec end_time;
  size_t elem_size = sizeof(TInt);
  long long elems = size/elem_size;
  for (int i=0; i<repeat_times; i++) {
    clock_gettime(CLOCK_REALTIME, &start_time);
    TVec<TInt, int64> v(elems);
    clock_gettime(CLOCK_REALTIME, &end_time);
    total_alloc += gettimediff(start_time, end_time);
    v[0] = 0;
    clock_gettime(CLOCK_REALTIME, &start_time);
    ~v;
    clock_gettime(CLOCK_REALTIME, &end_time);
    total_destroy += gettimediff(start_time, end_time);
  }
  std::cout << "alloc: " << total_alloc/repeat_times << " destroy: " << total_destroy/repeat_times << std::endl;
}

int main( int argc, char* argv[] ){
  if (argc != 2) {
    std::cout<<"invalid num args"<<std::endl;
    exit(0);
  }
  long long max_size;
  sscanf(argv[1], "%lld", &max_size);
  for (long long i=10000; i<= max_size; i=i*10) {
    std::cout<<i<<std::endl;
    perform_malloc(i);
    perform_tvec(i);
  }
 return 0;
}