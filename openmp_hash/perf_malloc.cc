#include "../../snap/snap-core/Snap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <iostream>

#include <sys/resource.h>
#include <sys/time.h>
#include <sys/times.h>

int REPEAT_TIMES = 3;

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

void 
time_operation(float &a_result, float &d_result, void* (*allocFn)(), void (*destFn)(void* buf)) {
  a_result = 0;
  d_result = 0;
  timespec start_time;
  timespec end_time;
  for (int i=0; i<REPEAT_TIMES; i++) {
    clock_gettime(CLOCK_REALTIME, &start_time);
    void* buf = allocFn();
    clock_gettime(CLOCK_REALTIME, &end_time);
    a_result += gettimediff(start_time, end_time);
    clock_gettime(CLOCK_REALTIME, &start_time);
    destFn(buf);
    clock_gettime(CLOCK_REALTIME, &end_time);
    d_result += gettimediff(start_time, end_time);
  }
  a_result = a_result/REPEAT_TIMES;
  d_result = d_result/REPEAT_TIMES;
}

void perform_malloc(long long size, int static_opt) {
  float alloc_result;
  float free_result;
  if (static_opt == 1) {
    auto allocFn = [size] () {return malloc(size);};
    auto destrFn = [] (void* buf) {free(buf);};
    time_operation(alloc_result, free_result, allocFn, destrFn);
  } else if (static_opt == 2) {
    auto allocFn = [size] () {return calloc(size, 1);};
    auto destrFn = [] (void* buf) {free(buf);};
    time_operation(alloc_result, free_result, allocFn, destrFn);
  } else {
    auto allocFn = [size] () {return new char[size];};
    auto destrFn = [] (void* buf) {
      char* arr = (char*) buf;
      delete[] arr;
    };
    time_operation(alloc_result, free_result, allocFn, destrFn);
  }
  printf("Size: %lld, Allocation: %f, Destruction: %f\n", size, alloc_result, free_result);
}

void perform_tvec(long long size) {
  float alloc_result;
  float free_result;
  size_t elem_size = sizeof(TInt);
  long long elems = size/elem_size;
  auto allocFn = [size] () {
    return new TVec<TInt, int64>(elems);
  };
  auto destrFn = [] (void* buf) {
    TVec<TInt, int64>*v = (TVec<TInt, int64>*) buf;
    delete v;
  };
  time_operation(alloc_result, free_result, allocFn, destrFn);
  printf("Num Elems: %lld, TVec Allocation: %f, TVec Destruction: %f\n", elems, alloc_result, free_result);
}

int main( int argc, char* argv[] ){
  if (argc != 3) {
    std::cout<<"invalid num args"<<std::endl;
    exit(0);
  }
  long long max_size;
  int static_opt;
  sscanf(argv[1], "%lld", &max_size);
  sscanf(argv[1], "%d", &static_opt);
  // 1 for malloc, 2 for calloc, 3 for new
  if (static_opt < 1 || static_opt > 3) {
    std::cout<<"invalid num args"<<std::endl;
    exit(0);
  }
  for (long long i=10000; i<= max_size; i=i*10) {
    std::cout<<i<<std::endl;
    perform_malloc(i, static_opt);
  }
 return 0;
}