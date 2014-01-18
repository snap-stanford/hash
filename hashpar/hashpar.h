#ifndef hashpar_h
#define hashpar_h

#include "../../snap/snap-core/Snap.h"

/////////////////////////////////////////////////
// HashPar-Table-Key-Data
#pragma pack(push, 1) // pack class size
template <class TKey, class TDat>
class THashParKeyDat {
public:
  TInt Status;
  TInt HashCd;
  TKey Key;
  TDat Dat;
public:
  THashParKeyDat():
    Status(0), HashCd(-1), Key(), Dat(){}
  THashParKeyDat(const int& _Status, const int& _HashCd, const TKey& _Key):
    Status(_Status), HashCd(_HashCd), Key(_Key), Dat(){}
  explicit THashParKeyDat(TSIn& SIn):
    Status(SIn), HashCd(SIn), Key(SIn), Dat(SIn){}
  void Save(TSOut& SOut) const {
    Status.Save(SOut); HashCd.Save(SOut); Key.Save(SOut); Dat.Save(SOut);}

  bool operator==(const THashParKeyDat& HashKeyDat) const {
    if (this==&HashKeyDat || (Status==HashKeyDat.Status && HashCd==HashKeyDat.HashCd
      && Key==HashKeyDat.Key && Dat==HashKeyDat.Dat)){return true;}
    return false;}
  THashParKeyDat& operator=(const THashParKeyDat& HashKeyDat){
    if (this!=&HashKeyDat){
      Status=HashKeyDat.Status; HashCd=HashKeyDat.HashCd;
      Key=HashKeyDat.Key; Dat=HashKeyDat.Dat;}
    return *this;}
};
#pragma pack(pop)  

/////////////////////////////////////////////////
// HashPar-Table-Key-Data-Iterator
template<class TKey, class TDat>
class THashParKeyDatI{
public:
  typedef THashParKeyDat<TKey, TDat> THPKeyDat;
private:
  THPKeyDat* KeyDatI;
  THPKeyDat* EndI;
public:
  THashParKeyDatI(): KeyDatI(NULL), EndI(NULL){}
  THashParKeyDatI(const THashParKeyDatI& _HashKeyDatI):
    KeyDatI(_HashKeyDatI.KeyDatI), EndI(_HashKeyDatI.EndI){}
  THashParKeyDatI(const THPKeyDat* _KeyDatI, const THPKeyDat* _EndI):
    KeyDatI((THPKeyDat*)_KeyDatI), EndI((THPKeyDat*)_EndI){}

  THashParKeyDatI& operator=(const THashParKeyDatI& HashKeyDatI){
    KeyDatI=HashKeyDatI.KeyDatI; EndI=HashKeyDatI.EndI; return *this;}
  bool operator==(const THashParKeyDatI& HashKeyDatI) const {
    return KeyDatI==HashKeyDatI.KeyDatI;}
  bool operator<(const THashParKeyDatI& HashKeyDatI) const {
    return KeyDatI<HashKeyDatI.KeyDatI;}
  THashParKeyDatI& operator++(int){ KeyDatI++; while (KeyDatI < EndI && KeyDatI->Status==0) { KeyDatI++; } return *this; }
  THashParKeyDatI& operator--(int){ do { KeyDatI--; } while (KeyDatI->Status==0); return *this;}
  THPKeyDat& operator*() const { return *KeyDatI; }
  THPKeyDat& operator()() const { return *KeyDatI; }
  THPKeyDat* operator->() const { return KeyDatI; }
  THashParKeyDatI& Next(){ operator++(1); return *this; }

  /// Tests whether the iterator has been initialized.
  bool IsEmpty() const { return KeyDatI == NULL; }
  /// Tests whether the iterator is pointing to the past-end element.
  bool IsEnd() const { return EndI == KeyDatI; }
  
  const TKey& GetKey() const {Assert((KeyDatI!=NULL)&&(KeyDatI->Status!=0)); return KeyDatI->Key;}
  const TDat& GetDat() const {Assert((KeyDatI!=NULL)&&(KeyDatI->Status!=0)); return KeyDatI->Dat;}
  TDat& GetDat() {Assert((KeyDatI!=NULL)&&(KeyDatI->Status!=0)); return KeyDatI->Dat;}
};

/////////////////////////////////////////////////
// HashPar-Table
template<class TKey, class TDat, class TSizeTy = int, class THashFunc = TDefaultHashFunc<TKey> >
class THashPar{
public:
  enum {HashPrimes=32};
  static const unsigned int HashPrimeT[HashPrimes]; // Prime numbers used for resize
public:
  typedef THashParKeyDatI<TKey, TDat> TIter;
private:
  // The different states of each element in the table
  static const int FREE = 0;
  static const int WRITING_KEY = 1;
  static const int OCCUPIED = 2;
  static const int WRITING_DAT = 3;
private:
  typedef THashParKeyDat<TKey, TDat> THPKeyDat;
  typedef TPair<TKey, TDat> TKeyDatP;
  TVec<THPKeyDat, TSizeTy> Table; // The table in which the elements are stored directly into
  TInt NumVals;                   // The number of keys in the table
private:
  class THashParKeyDatCmp {
  public:
    const THashPar<TKey, TDat, TSizeTy, THashFunc>& Hash;
    bool CmpKey, Asc;
    THashParKeyDatCmp(THashPar<TKey, TDat, TSizeTy, THashFunc>& _Hash, const bool& _CmpKey, const bool& _Asc) :
      Hash(_Hash), CmpKey(_CmpKey), Asc(_Asc) { }
    bool operator () (const int& KeyId1, const int& KeyId2) const {
      if (CmpKey) {
        if (Asc) { return Hash.GetKey(KeyId1) < Hash.GetKey(KeyId2); }
        else { return Hash.GetKey(KeyId2) < Hash.GetKey(KeyId1); } }
      else {
        if (Asc) { return Hash[KeyId1] < Hash[KeyId2]; }
        else { return Hash[KeyId2] < Hash[KeyId1]; } } }
  };
private:
  THPKeyDat& GetHashParKeyDat(const int& KeyId){
    THPKeyDat& KeyDat=Table[KeyId];
    Assert(KeyDat.Status!=0); return KeyDat;}
  const THPKeyDat& GetHashParKeyDat(const int& KeyId) const {
    const THPKeyDat& KeyDat=Table[KeyId];
    Assert(KeyDat.Status!=0); return KeyDat;}
  uint GetNextPrime(const uint& Val) const;
//  void Resize();
public:
  THashPar():
    Table(), NumVals(0){}
  THashPar(const THashPar& HashPar):
    Table(HashPar.Table), NumVals(HashPar.NumVals){}
  explicit THashPar(const int& ExpectVals) {
    Gen(ExpectVals);}
  explicit THashPar(TSIn& SIn):
    Table(SIn), NumVals(SIn){
    SIn.LoadCs();}
  void Load(TSIn& SIn){
    Table.Load(SIn); NumVals.Load(SIn);
    SIn.LoadCs();}
  void Save(TSOut& SOut) const {
    Table.Save(SOut); NumVals.Save(SOut);
    SOut.SaveCs();}
  void LoadXml(const PXmlTok& XmlTok, const TStr& Nm=""){}
  void SaveXml(TSOut& SOut, const TStr& Nm){}

  THashPar& operator=(const THashPar& Hash){
    if (this!=&Hash){
      Table=Hash.Table; NumVals=Hash.NumVals;}
    return *this;}
  bool operator==(const THashPar& Hash) const; //J: zdaj tak kot je treba
  bool operator < (const THashPar& Hash) const { Fail; return true; }
  const TDat& operator[](const int& KeyId) const {return GetHashParKeyDat(KeyId).Dat;}
  TDat& operator[](const int& KeyId){return GetHashParKeyDat(KeyId).Dat;}
  TDat& operator()(const TKey& Key){return AddDat(Key);}
  ::TSize GetMemUsed() const {
    // return PortV.GetMemUsed()+KeyDatV.GetMemUsed()+sizeof(bool)+2*sizeof(int);}
      int64 MemUsed = sizeof(int);
      for (int TableN = 0; TableN < Table.Len(); TableN++) {
          MemUsed += int64(sizeof(TInt));
          MemUsed += int64(Table[TableN].Key.GetMemUsed());
          MemUsed += int64(Table[TableN].Dat.GetMemUsed());
      }
      return ::TSize(MemUsed);
  }

  TIter BegI() const {
    if (Len() == 0){return TIter(Table.EndI(), Table.EndI());}
    return TIter(Table.BegI(), Table.EndI());}
  TIter EndI() const {return TIter(Table.EndI(), Table.EndI());}
  //TIter GetI(const int& KeyId) const {return TIter(&KeyDatV[KeyId], KeyDatV.EndI());}
  TIter GetI(const TKey& Key) const {return TIter(&Table[GetKeyId(Key)], Table.EndI());}

  void Gen(const int& ExpectVals){
    Table.Gen(GetNextPrime(ExpectVals*2));
    NumVals = 0;}
  void Clr(const bool& DoDel=true);
  bool Empty() const {return Len()==0;}
  int Len() const {return NumVals;}
  int GetMxKeyIds() const {return Table.Len();}
//int GetReservedKeyIds() const {return KeyDatV.Reserved();}
//bool IsKeyIdEqKeyN() const {return FreeKeys==0;}

  /// Return and set NumVals to the number of keys in the table
  int ComputeLen();
  /// Thread-safe add key to table. UpdateCount specifies if NumVals should be updated. If not,
  /// NumVals can be set to the correct value by calling ComputeLen() after insertion is done.
  int ThreadSafeAddKey(const TKey& Key, bool UpdateCount=true);
//TDat& AddDatId(const TKey& Key){
//  int KeyId=AddKey(Key); return Table[KeyId].Dat=KeyId;}
//TDat& AddDat(const TKey& Key){return Table[AddKey(Key)].Dat;}
  // Thread-safe add dat to table. UpdateCount specifies if NumVals should be updated. If not,
  // NumVals can be set to the correct value by calling ComputeLen() after insertion is done.
  void ThreadSafeAddDat(const TKey& Key, const TDat& Dat, bool UpdateCount=true){
    // Find the index of table that the dat should be written
    int KeyId=ThreadSafeAddKey(Key, UpdateCount);
    // Acquire lock to write dat
    while (Table[KeyId].Status != OCCUPIED ||
	   (!__sync_bool_compare_and_swap(&Table[KeyId].Status.Val, OCCUPIED, WRITING_DAT)));
    Table[KeyId].Dat = Dat;
    // Unlock
    IAssert(__sync_bool_compare_and_swap(&Table[KeyId].Status.Val, WRITING_DAT, OCCUPIED));}
  // Thread-safe bulk add dat to table. UpdateCount specifies if NumVals should be updated. If not,
  // NumVals can be set to the correct value by calling ComputeLen() after insertion is done.
  void ThreadSafeAddNDat(const TVec<TKey>& Keys, const TVec<TDat>& Dats, bool UpdateCount=true){
    IAssert(Keys.Len() == Dats.Len());
    for (int i=0; i<Keys.Len(); i++){ThreadSafeAddDat(Keys[i], Dats[i], UpdateCount);}}

  void DelKey(const TKey& Key){DelKeyId(GetKeyId(Key));}
  bool DelIfKey(const TKey& Key){
    int KeyId; if (IsKey(Key, KeyId)){DelKeyId(KeyId); return true;} return false;}
  void DelKeyId(const int& KeyId){
    IAssert(Table[KeyId].Status==OCCUPIED);
    NumVals--;
    Table[KeyId].Status = 0;
    Table[KeyId].Key = TKey();
    Table[KeyId].Dat = TDat();}
  void DelKeyIdV(const TIntV& KeyIdV){
    for (int KeyIdN=0; KeyIdN<KeyIdV.Len(); KeyIdN++){DelKeyId(KeyIdV[KeyIdN]);}}

  void MarkDelKey(const TKey& Key){MarkDelKeyId(GetKeyId(Key));}
  void MarkDelKeyId(const int& KeyId){
    IAssert(Table[KeyId].Status==2);
    Table[KeyId].Status = 0;}

  const TKey& GetKey(const int& KeyId) const { return GetHashParKeyDat(KeyId).Key;}
  int GetKeyId(const TKey& Key) const;
  /// Get an index of a random element. If the hash table has many deleted keys, this may take a long time.
  //int GetRndKeyId(TRnd& Rnd) const;
  /// Get an index of a random element. If the hash table has many deleted keys, defrag the hash table first (that's why the function is non-const).
  //int GetRndKeyId(TRnd& Rnd, const double& EmptyFrac);
  bool IsKey(const TKey& Key) const {return GetKeyId(Key)!=-1;}
  bool IsKey(const TKey& Key, int& KeyId) const { KeyId=GetKeyId(Key); return KeyId!=-1;}
  bool IsKeyId(const int& KeyId) const {
    return (0<=KeyId)&&(KeyId<Table.Len())&&(Table[KeyId].Status!=0);}
  const TDat& GetDat(const TKey& Key) const {return Table[GetKeyId(Key)].Dat;}
  TDat& GetDat(const TKey& Key){return Table[GetKeyId(Key)].Dat;}
//  TKeyDatP GetKeyDat(const int& KeyId) const {
//    THPKeyDat& KeyDat=GetHashParKeyDat(KeyId);
//    return TKeyDatP(KeyDat.Key, KeyDat.Dat);}
  void GetKeyDat(const int& KeyId, TKey& Key, TDat& Dat) const {
    const THPKeyDat& KeyDat=GetHashParKeyDat(KeyId);
    Key=KeyDat.Key; Dat=KeyDat.Dat;}
  bool IsKeyGetDat(const TKey& Key, TDat& Dat) const {int KeyId;
    if (IsKey(Key, KeyId)){Dat=GetHashParKeyDat(KeyId).Dat; return true;}
    else {return false;}}

  int FFirstKeyId() const {return 0-1;}
  bool FNextKeyId(int& KeyId) const;
  void GetKeyV(TVec<TKey>& KeyV) const;
  void GetDatV(TVec<TDat>& DatV) const;
  void GetKeyDatPrV(TVec<TPair<TKey, TDat> >& KeyDatPrV) const;
  void GetDatKeyPrV(TVec<TPair<TDat, TKey> >& DatKeyPrV) const;
  void GetKeyDatKdV(TVec<TKeyDat<TKey, TDat> >& KeyDatKdV) const;
  void GetDatKeyKdV(TVec<TKeyDat<TDat, TKey> >& DatKeyKdV) const;

  void Swap(THashPar& Hash);
//void Defrag();
  void Pack(){Table.Pack();}
/*void Sort(const bool& CmpKey, const bool& Asc);
  void SortByKey(const bool& Asc=true) { Sort(true, Asc); }
  void SortByDat(const bool& Asc=true) { Sort(false, Asc); }*/
};

template<class TKey, class TDat, class TSizeTy, class THashFunc>
const unsigned int THashPar<TKey, TDat, TSizeTy, THashFunc>::HashPrimeT[HashPrimes]={
  3ul, 5ul, 11ul, 23ul,
  53ul,         97ul,         193ul,       389ul,       769ul,
  1543ul,       3079ul,       6151ul,      12289ul,     24593ul,
  49157ul,      98317ul,      196613ul,    393241ul,    786433ul,
  1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
  50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul,
  1610612741ul, 3221225473ul, 4294967291ul
};

template<class TKey, class TDat, class TSizeTy, class THashFunc>
uint THashPar<TKey, TDat, TSizeTy, THashFunc>::GetNextPrime(const uint& Val) const {
  const uint* f=(const uint*)HashPrimeT, *m, *l=(const uint*)HashPrimeT + (int)HashPrimes;
  int h, len = (int)HashPrimes;
  while (len > 0) {
    h = len >> 1;  m = f + h;
    if (*m < Val) { f = m;  f++;  len = len - h - 1; }
    else len = h;
  }
  return f == l ? *(l - 1) : *f;
}

template<class TKey, class TDat, class TSizeTy, class THashFunc>
bool THashPar<TKey, TDat, TSizeTy, THashFunc>::operator==(const THashPar& Hash) const {
  if (Len() != Hash.Len()) { return false; }
  for (int i = FFirstKeyId(); FNextKeyId(i); ) {
    const TKey& Key = GetKey(i);
    if (! Hash.IsKey(Key)) { return false; }
    if (GetDat(Key) != Hash.GetDat(Key)) { return false; }
  }
  return true;
}

template<class TKey, class TDat, class TSizeTy, class THashFunc>
int THashPar<TKey, TDat, TSizeTy, THashFunc>::ComputeLen() {
  int len = 0;
  int KeyId=FFirstKeyId();
  while (FNextKeyId(KeyId)) { len++; }
  return NumVals=len;
}

template<class TKey, class TDat, class TSizeTy, class THashFunc>
  int THashPar<TKey, TDat, TSizeTy, THashFunc>::ThreadSafeAddKey(const TKey& Key, bool UpdateCount) {
  const int BegTableN=abs(Key.GetPrimHashCd()%Table.Len());
  const int HashCd=abs(Key.GetSecHashCd());

  int TableN = BegTableN;
  while (1) {
    if (Table[TableN].Status == FREE && 
	__sync_bool_compare_and_swap(&Table[TableN].Status.Val, FREE, WRITING_KEY)) {
      // Acquire lock on a free space is successful. Proceed to write the key
      if (UpdateCount) {
	int CurVals = __sync_fetch_and_add(&NumVals.Val, 1);
	IAssertR(CurVals < Table.Len(), "Table must not be full");
      }
      // Write key and secondary hash value
      Table[TableN].HashCd = HashCd;
      Table[TableN].Key = Key;
      // Release the lock
      IAssert(__sync_bool_compare_and_swap(&Table[TableN].Status.Val, WRITING_KEY, OCCUPIED));
      break;
    } else if (Table[TableN].Status == WRITING_KEY) {
      // Some other thread is writing the key. The current thread must wait until the write is finish.
      continue;
    } else if (Table[TableN].HashCd == HashCd &&
	       Table[TableN].Key == Key) {
      // Key is already in the table
      break;
    } else {
      // Otherwise, proceed to the next space
      TableN = (TableN + 1) % Table.Len();
      IAssertR(TableN != BegTableN, "Table must not be full");
    }
  }
  return TableN;
}

template<class TKey, class TDat, class TSizeTy, class THashFunc>
int THashPar<TKey, TDat, TSizeTy, THashFunc>::GetKeyId(const TKey& Key) const {
  const int BegTableN=abs(Key.GetPrimHashCd()%Table.Len());
  const int HashCd=abs(Key.GetSecHashCd());
 
  int TableN = BegTableN;
  while (Table[TableN].HashCd != -1) {
    if (Table[TableN].Status == OCCUPIED && Table[TableN].Key == Key) { return TableN; }
    TableN = (TableN + 1) % Table.Len();
    if (TableN == BegTableN) { return -1; }
  }

  return -1;
}

template<class TKey, class TDat, class TSizeTy, class THashFunc>
void THashPar<TKey, TDat, TSizeTy, THashFunc>::Clr(const bool& DoDel){
  if (DoDel){
    Table.Clr();
  } else {
    Table.PutAll(THPKeyDat());
  }
  NumVals = TInt(0);
}

template<class TKey, class TDat, class TSizeTy, class THashFunc>
bool THashPar<TKey, TDat, TSizeTy, THashFunc>::FNextKeyId(int& KeyId) const {
  do {KeyId++;} while ((KeyId<Table.Len())&&(Table[KeyId].Status==0));
  return KeyId<Table.Len();
}

template<class TKey, class TDat, class TSizeTy, class THashFunc>
void THashPar<TKey, TDat, TSizeTy, THashFunc>::GetKeyV(TVec<TKey>& KeyV) const {
  KeyV.Gen(Len(), 0);
  int KeyId=FFirstKeyId();
  while (FNextKeyId(KeyId)){
    KeyV.Add(GetKey(KeyId));}
}

template<class TKey, class TDat, class TSizeTy, class THashFunc>
void THashPar<TKey, TDat, TSizeTy, THashFunc>::GetDatV(TVec<TDat>& DatV) const {
  DatV.Gen(Len(), 0);
  int KeyId=FFirstKeyId();
  while (FNextKeyId(KeyId)){
    DatV.Add(GetHashParKeyDat(KeyId).Dat);}
}

template<class TKey, class TDat, class TSizeTy, class THashFunc>
void THashPar<TKey, TDat, TSizeTy, THashFunc>::GetKeyDatPrV(TVec<TPair<TKey, TDat> >& KeyDatPrV) const {
  KeyDatPrV.Gen(Len(), 0);
  TKey Key; TDat Dat;
  int KeyId=FFirstKeyId();
  while (FNextKeyId(KeyId)){
    GetKeyDat(KeyId, Key, Dat);
    KeyDatPrV.Add(TPair<TKey, TDat>(Key, Dat));
  }
}

template<class TKey, class TDat, class TSizeTy, class THashFunc>
void THashPar<TKey, TDat, TSizeTy, THashFunc>::GetDatKeyPrV(TVec<TPair<TDat, TKey> >& DatKeyPrV) const {
  DatKeyPrV.Gen(Len(), 0);
  TKey Key; TDat Dat;
  int KeyId=FFirstKeyId();
  while (FNextKeyId(KeyId)){
    GetKeyDat(KeyId, Key, Dat);
    DatKeyPrV.Add(TPair<TDat, TKey>(Dat, Key));
  }
}

template<class TKey, class TDat, class TSizeTy, class THashFunc>
void THashPar<TKey, TDat, TSizeTy, THashFunc>::GetKeyDatKdV(TVec<TKeyDat<TKey, TDat> >& KeyDatKdV) const {
  KeyDatKdV.Gen(Len(), 0);
  TKey Key; TDat Dat;
  int KeyId=FFirstKeyId();
  while (FNextKeyId(KeyId)){
    GetKeyDat(KeyId, Key, Dat);
    KeyDatKdV.Add(TKeyDat<TKey, TDat>(Key, Dat));
  }
}

template<class TKey, class TDat, class TSizeTy, class THashFunc>
void THashPar<TKey, TDat, TSizeTy, THashFunc>::GetDatKeyKdV(TVec<TKeyDat<TDat, TKey> >& DatKeyKdV) const {
  DatKeyKdV.Gen(Len(), 0);
  TKey Key; TDat Dat;
  int KeyId=FFirstKeyId();
  while (FNextKeyId(KeyId)){
    GetKeyDat(KeyId, Key, Dat);
    DatKeyKdV.Add(TKeyDat<TDat, TKey>(Dat, Key));
  }
}

template<class TKey, class TDat, class TSizeTy, class THashFunc>
void THashPar<TKey, TDat, TSizeTy, THashFunc>::Swap(THashPar& Hash) {
  if (this!=&Hash){
    Table.Swap(Hash.Table);
    ::Swap(NumVals, Hash.NumVals);
  }
}

#endif
