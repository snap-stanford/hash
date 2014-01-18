/* 
   This program uses a local forked SNAP at https://github.com/chantat/snap.
   This program computes the distribution of the number of hash chain
   traversals for THash's AddKey for n 32-bit integer keys and values
   (i.e. the distribution of the length of the bucket's linked list when a key
   that will be inserted is hashed into it). This program takes in one argument
   which is n.
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

void InsertionChainTest(bool GetChainTraversalDistribution) {
  // Construct random key sequence
  TIntV v(n);
  for (long long i = 0; i < n; i++) {
    v[i] = i;
  }

  TRnd rnd;
  v.Shuffle(rnd);

  struct drand48_data buffer;
  srand48_r(time(NULL), &buffer);

  THash<TInt, TInt> stat;
  long int r;
  for (long long i = 0; i < n; i++) {
    lrand48_r(&buffer, &r);
    int ret =  h.AddDat2(v[i], r);
    TInt freq;
    if (!stat.IsKeyGetDat(ret, freq)) {
      freq = 0;
    }
    stat.AddDat(ret, freq + 1);
  }

  printf("Len\tFreq\n");
  TIntPrV result;
  stat.GetKeyDatPrV(result);
  for (int i = 0; i < result.Len(); i++) {
    printf("%d\t%d\n", result[i].GetVal1().Val, result[i].GetVal2().Val);
  }
}

int main( int argc, char* argv[] ){
  if (argc != 2) {
    exit(0);
  }
  sscanf(argv[1], "%lld", &n);

  // Insertion Test
  InsertionChainTest(false);
  
  return 0;
}
