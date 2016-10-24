#include "../../snap/snap-core/Snap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sys/resource.h>
#include <sys/time.h>
#include <sys/times.h>

long long n;

void fillVector(TVec<TInt, int64> & v) {
  #pragma omp parallel for
  for (long long i = 0; i < n; i++) {
    if (i % (n/10) == 0) {
      printf("still updating...\n");
    }
    v[i] = lrand48();
  }
}

int main( int argc, char* argv[] ){
  if (argc != 3) {
    exit(0);
  }
  sscanf(argv[1], "%lld", &n);

  TVec<TInt, int64> v = TVec<TInt, int64>(n);

  fillVector(v);
  TStr filename(argv[2]);
  TFOut outstream(filename);
  v.Save(outstream);
  return 0;
}