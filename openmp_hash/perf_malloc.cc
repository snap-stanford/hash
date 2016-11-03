/*
 * Perform an allocation of a large amount of memory and measure the
 * the time it takes to allocate and free. The program will perform
 * a static allocation (malloc, calloc, or new) and create a TVec
 * of integers of the equivalent size.
 * The program takes two arguments:
 *  size: the size of the array to be allocated
 *  static_opt: 1 for malloc, 2 for calloc, 3 for new
 */
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
size_t ARRAY_SIZE;

/* Callbacks */
void* malloc_chars() {return malloc(ARRAY_SIZE);}
void* calloc_chars() {return calloc(ARRAY_SIZE, 1);}
void* new_chars() {return new char[ARRAY_SIZE];}
void* alloc_tvec() {
  size_t elem_size = sizeof(TInt);
  size_t elems = ARRAY_SIZE/elem_size;
  return new TVec<TInt, int64>(elems);
}
void new_destr(void* buf) {
  char* arr = (char*) buf;
  delete[] arr;
}
void free_TVec(void* buf) {
  TVec<TInt, int64>* v = (TVec<TInt, int64>*) buf;
  delete v;
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

/* Time the operation of two callbacks: allocFn and destFn */
void
time_operation(void* (*allocFn)(), void (*destFn)(void* buf)) {
  float create = 0;
  float destr = 0;
  timespec start_time;
  timespec end_time;
  for (int i=0; i<REPEAT_TIMES; i++) {
    clock_gettime(CLOCK_REALTIME, &start_time);
    void* buf = allocFn();
    clock_gettime(CLOCK_REALTIME, &end_time);
    create += gettimediff(start_time, end_time);
    clock_gettime(CLOCK_REALTIME, &start_time);
    destFn(buf);
    clock_gettime(CLOCK_REALTIME, &end_time);
    destr += gettimediff(start_time, end_time);
  }
  printf("Allocation: %.3e, Destruction: %.3e\n", create/REPEAT_TIMES, destr/REPEAT_TIMES);
}

void measure_allocations(size_t size, int static_opt) {
  if (static_opt == 1) {
    time_operation(malloc_chars, free);
  } else if (static_opt == 2) {
    time_operation(calloc_chars, free);
  } else {
    time_operation(new_chars, new_destr);
  }
  time_operation(alloc_tvec, free_TVec);
}

int main( int argc, char* argv[] ){
  if (argc != 3) {
    std::cout<<"invalid num args"<<std::endl;
    exit(0);
  }
  size_t size;
  sscanf(argv[1], "%lld", &size);
  ARRAY_SIZE = size;
  int static_opt;
  sscanf(argv[2], "%d", &static_opt);
  if (static_opt < 1 || static_opt > 3) {
    std::cout<<"invalid num args"<<std::endl;
    exit(0);
  }
  printf("Size is %zd \n", size);
  measure_allocations(size, static_opt);
 return 0;
}