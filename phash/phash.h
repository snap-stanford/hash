#ifndef phash_h
#define phash_h

#include "../../snap/snap-core/Snap.h"

class TPHashKeyDat {
public:
  TInt HashCd;
  TInt Key;
  TInt Dat;
public:
  TPHashKeyDat():
  HashCd(-1), Key(-1), Dat(-1){}
};

class TPHash {
public:
  enum {HashPrimes=32};
  static const unsigned int HashPrimeT[HashPrimes];
private:
  typedef TPHashKeyDat TPHKeyDat;
  TVec<TPHKeyDat> Table;
  uint GetNextPrime(const uint& Val) const;
  void Resize();
public:
  void Gen(const int &ExpectVals);
  TInt& AddDat(const TInt& Key, const TInt& Dat);
  TInt& GetDat(const TInt& Key);
};

#endif
