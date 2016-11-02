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


void* perform_malloc(long long size) {
  timespec start_time;
  clock_gettime(CLOCK_REALTIME, &start_time);
  void* buf = malloc(size);
  timespec end_time;
  clock_gettime(CLOCK_REALTIME, &end_time);
  float wtdiff = gettimediff(start_time, end_time);
  std::cout<<"malloc: " << wtdiff <<std::endl;
  return buf;
}

void perform_free(void* buf {
  timespec start_time;
  clock_gettime(CLOCK_REALTIME, &start_time);
  free(buf);
  timespec end_time;
  clock_gettime(CLOCK_REALTIME, &end_time);
  float wtdiff = gettimediff(start_time, end_time);
  std::cout<<"free: " << wtdiff <<std::endl;
}

int main( int argc, char* argv[] ){
  if (argc != 2) {
    std::cout<<"invalid num args"<<std::endl;
    exit(0);
  }
  long long size;
  sscanf(argv[1], "%lld", &size);
  void* buf = perform_malloc(size);
  perform_free(buf);
 return 0;
}
