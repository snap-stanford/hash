#include "phash.h"

const unsigned int TPHash::HashPrimeT[HashPrimes]={
  3ul, 5ul, 11ul, 23ul,
  53ul,         97ul,         193ul,       389ul,       769ul,
  1543ul,       3079ul,       6151ul,      12289ul,     24593ul,
  49157ul,      98317ul,      196613ul,    393241ul,    786433ul,
  1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
  50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul,
  1610612741ul, 3221225473ul, 4294967291ul
};

uint TPHash::GetNextPrime(const uint& Val) const {
  const uint* f=(const uint*)HashPrimeT, *m, *l=(const uint*)HashPrimeT + (int)HashPrimes;
  int h, len = (int)HashPrimes;
  while (len > 0) {
    h = len >> 1;  m = f + h;
    if (*m < Val) { f = m;  f++;  len = len - h - 1; }
    else len = h;
  }
  return f == l ? *(l - 1) : *f;
}

void TPHash::Gen(const int &ExpectVals) {
  Table.Gen(GetNextPrime(ExpectVals*2));
}

void TPHash::Resize() {
  //if (Table.Len() == 0) {
  //Table.Gen(2038064921);
    //}
}

TInt& TPHash::AddDat(const TInt& Key, const TInt& Dat) {
  int TableN=abs(Key.GetPrimHashCd()%Table.Len());
  const int HashCd=abs(Key.GetSecHashCd());

  while (Table[TableN].HashCd != -1 || 
    (!__sync_bool_compare_and_swap(&Table[TableN].HashCd.Val, -1, HashCd))) {
    TableN = (TableN + 1) % Table.Len();
  }

  Table[TableN].Key = Key;
  Table[TableN].Dat = Dat;
  return Table[TableN].Dat;
}

TInt& TPHash::GetDat(const TInt& Key) {
  int TableN=abs(Key.GetPrimHashCd()%Table.Len());
  const int HashCd=abs(Key.GetSecHashCd());

  while (Table[TableN].HashCd != -1) {
    if (Table[TableN].HashCd == HashCd) {
      return Table[TableN].Dat;
    }
    TableN = (TableN + 1) % Table.Len();
  }

  return Table[TableN].Dat;
}
