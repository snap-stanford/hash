#ifndef phash_h
#define phash_h

#include "../../snap/snap-core/Snap.h"

/////////////////////////////////////////////////
// PHash-Table-Key-Data
#pragma pack(push, 1) // pack class size
template <class TKey, class TDat>
class TPHashKeyDat {
public:
  TInt Status;
  TInt HashCd;
  TKey Key;
  TDat Dat;
public:
  TPHashKeyDat():
    Status(0), HashCd(-1), Key(), Dat(){}
  TPHashKeyDat(const int& _Status, const int& _HashCd, const TKey& _Key):
    Status(_Status), HashCd(_HashCd), Key(_Key), Dat(){}
  explicit TPHashKeyDat(TSIn& SIn):
    Status(SIn), HashCd(SIn), Key(SIn), Dat(SIn){}
  void Save(TSOut& SOut) const {
    Status.Save(SOut); HashCd.Save(SOut); Key.Save(SOut); Dat.Save(SOut);}

  bool operator==(const TPHashKeyDat& HashKeyDat) const {
    if (this==&HashKeyDat || (Status==HashKeyDat.Status && HashCd==HashKeyDat.HashCd
      && Key==HashKeyDat.Key && Dat==HashKeyDat.Dat)){return true;}
    return false;}
  TPHashKeyDat& operator=(const TPHashKeyDat& HashKeyDat){
    if (this!=&HashKeyDat){
      Status=HashKeyDat.Status; HashCd=HashKeyDat.HashCd;
      Key=HashKeyDat.Key; Dat=HashKeyDat.Dat;}
    return *this;}
};
#pragma pack(pop)  

/////////////////////////////////////////////////
// PHash-Table-Key-Data-Iterator
template<class TKey, class TDat>
class TPHashKeyDatI{
public:
  typedef TPHashKeyDat<TKey, TDat> TPHKeyDat;
private:
  TPHKeyDat* KeyDatI;
  TPHKeyDat* EndI;
public:
  TPHashKeyDatI(): KeyDatI(NULL), EndI(NULL){}
  TPHashKeyDatI(const TPHashKeyDatI& _HashKeyDatI):
    KeyDatI(_HashKeyDatI.KeyDatI), EndI(_HashKeyDatI.EndI){}
  TPHashKeyDatI(const TPHKeyDat* _KeyDatI, const TPHKeyDat* _EndI):
    KeyDatI((TPHKeyDat*)_KeyDatI), EndI((TPHKeyDat*)_EndI){}

  TPHashKeyDatI& operator=(const TPHashKeyDatI& HashKeyDatI){
    KeyDatI=HashKeyDatI.KeyDatI; EndI=HashKeyDatI.EndI; return *this;}
  bool operator==(const TPHashKeyDatI& HashKeyDatI) const {
    return KeyDatI==HashKeyDatI.KeyDatI;}
  bool operator<(const TPHashKeyDatI& HashKeyDatI) const {
    return KeyDatI<HashKeyDatI.KeyDatI;}
  TPHashKeyDatI& operator++(int){ KeyDatI++; while (KeyDatI < EndI && KeyDatI->Status==0) { KeyDatI++; } return *this; }
  TPHashKeyDatI& operator--(int){ do { KeyDatI--; } while (KeyDatI->Status==0); return *this;}
  TPHKeyDat& operator*() const { return *KeyDatI; }
  TPHKeyDat& operator()() const { return *KeyDatI; }
  TPHKeyDat* operator->() const { return KeyDatI; }
  TPHashKeyDatI& Next(){ operator++(1); return *this; }

  /// Tests whether the iterator has been initialized.
  bool IsEmpty() const { return KeyDatI == NULL; }
  /// Tests whether the iterator is pointing to the past-end element.
  bool IsEnd() const { return EndI == KeyDatI; }
  
  const TKey& GetKey() const {Assert((KeyDatI!=NULL)&&(KeyDatI->Status!=0)); return KeyDatI->Key;}
  const TDat& GetDat() const {Assert((KeyDatI!=NULL)&&(KeyDatI->Status!=0)); return KeyDatI->Dat;}
  TDat& GetDat() {Assert((KeyDatI!=NULL)&&(KeyDatI->Status!=0)); return KeyDatI->Dat;}
};

/////////////////////////////////////////////////
// PHash-Table
template<class TKey, class TDat, class TSizeTy = int, class THashFunc = TDefaultHashFunc<TKey> >
class TPHash{
public:
  enum {HashPrimes=32};
  static const unsigned int HashPrimeT[HashPrimes];
public:
  typedef TPHashKeyDatI<TKey, TDat> TIter;
private:
  typedef TPHashKeyDat<TKey, TDat> TPHKeyDat;
  typedef TPair<TKey, TDat> TKeyDatP;
TVec<TPHKeyDat, TSizeTy> Table;
  TInt NumVals;
private:
  class TPHashKeyDatCmp {
  public:
    const TPHash<TKey, TDat, TSizeTy, THashFunc>& Hash;
    bool CmpKey, Asc;
    TPHashKeyDatCmp(TPHash<TKey, TDat, TSizeTy, THashFunc>& _Hash, const bool& _CmpKey, const bool& _Asc) :
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
  TPHKeyDat& GetPHashKeyDat(const int& KeyId){
    TPHKeyDat& KeyDat=Table[KeyId];
    Assert(KeyDat.Status!=0); return KeyDat;}
  const TPHKeyDat& GetPHashKeyDat(const int& KeyId) const {
    const TPHKeyDat& KeyDat=Table[KeyId];
    Assert(KeyDat.Status!=0); return KeyDat;}
  uint GetNextPrime(const uint& Val) const;
//  void Resize();
public:
  TPHash():
    Table(), NumVals(0){}
  TPHash(const TPHash& PHash):
    Table(PHash.Table), NumVals(PHash.NumVals){}
  explicit TPHash(const int& ExpectVals) {
    Gen(ExpectVals);}
  explicit TPHash(TSIn& SIn):
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

  TPHash& operator=(const TPHash& Hash){
    if (this!=&Hash){
      Table=Hash.Table; NumVals=Hash.NumVals;}
    return *this;}
  bool operator==(const TPHash& Hash) const; //J: zdaj tak kot je treba
  bool operator < (const TPHash& Hash) const { Fail; return true; }
  const TDat& operator[](const int& KeyId) const {return GetPHashKeyDat(KeyId).Dat;}
  TDat& operator[](const int& KeyId){return GetPHashKeyDat(KeyId).Dat;}
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
    //TODO
    NumVals = ExpectVals;}
  void Clr(const bool& DoDel=true);
  bool Empty() const {return Len()==0;}
  int Len() const {return NumVals;}
  int GetMxKeyIds() const {return Table.Len();}
//int GetReservedKeyIds() const {return KeyDatV.Reserved();}
//bool IsKeyIdEqKeyN() const {return FreeKeys==0;}

  int SafeAddKey(const TKey& Key);
  TDat& AddDatId(const TKey& Key){
    int KeyId=AddKey(Key); return Table[KeyId].Dat=KeyId;}
  // TODO: Make Dat updatable
  TDat& AddDat(const TKey& Key){return Table[AddKey(Key)].Dat;}
  void SafeAddDat(const TKey& Key, const TDat& Dat, bool UpdateCount=true){
    int KeyId=SafeAddKey(Key);
    while (Table[KeyId].Status != 2 ||
	   (!__sync_bool_compare_and_swap(&Table[KeyId].Status.Val, 2, 3)));
    Table[KeyId].Dat = Dat;
    __sync_fetch_and_add(&Table[KeyId].Status.Val, -1);}
/*  void SafeAddNDat(const TVec<TKey>& Keys, const TVec<TDat>& Dats){
    IAssert(Keys.Len() == Dats.Len());
    for (int i=0; i<Keys.Len(); i++){SafeAddDat(Keys[i], Dats[i], false);}*/

  void DelKey(const TKey& Key){DelKeyId(GetKeyId(Key));}
  bool DelIfKey(const TKey& Key){
    int KeyId; if (IsKey(Key, KeyId)){DelKeyId(KeyId); return true;} return false;}
  void DelKeyId(const int& KeyId){
    IAssert(Table[KeyId].Status==2);
    Table[KeyId].Status = 0;
    Table[KeyId].Key = TKey();
    Table[KeyId].Dat = TDat();}
  void DelKeyIdV(const TIntV& KeyIdV){
    for (int KeyIdN=0; KeyIdN<KeyIdV.Len(); KeyIdN++){DelKeyId(KeyIdV[KeyIdN]);}}

  void MarkDelKey(const TKey& Key){MarkDelKeyId(GetKeyId(Key));}
  void MarkDelKeyId(const int& KeyId){
    IAssert(Table[KeyId].Status==2);
    Table[KeyId].Status = 0;}

  const TKey& GetKey(const int& KeyId) const { return GetPHashKeyDat(KeyId).Key;}
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
//    TPHKeyDat& KeyDat=GetPHashKeyDat(KeyId);
//    return TKeyDatP(KeyDat.Key, KeyDat.Dat);}
  void GetKeyDat(const int& KeyId, TKey& Key, TDat& Dat) const {
    const TPHKeyDat& KeyDat=GetPHashKeyDat(KeyId);
    Key=KeyDat.Key; Dat=KeyDat.Dat;}
  bool IsKeyGetDat(const TKey& Key, TDat& Dat) const {int KeyId;
    if (IsKey(Key, KeyId)){Dat=GetPHashKeyDat(KeyId).Dat; return true;}
    else {return false;}}

  int FFirstKeyId() const {return 0-1;}
  bool FNextKeyId(int& KeyId) const;
  void GetKeyV(TVec<TKey>& KeyV) const;
  void GetDatV(TVec<TDat>& DatV) const;
  void GetKeyDatPrV(TVec<TPair<TKey, TDat> >& KeyDatPrV) const;
  void GetDatKeyPrV(TVec<TPair<TDat, TKey> >& DatKeyPrV) const;
  void GetKeyDatKdV(TVec<TKeyDat<TKey, TDat> >& KeyDatKdV) const;
  void GetDatKeyKdV(TVec<TKeyDat<TDat, TKey> >& DatKeyKdV) const;

  void Swap(TPHash& Hash);
//void Defrag();
  void Pack(){Table.Pack();}
/*void Sort(const bool& CmpKey, const bool& Asc);
  void SortByKey(const bool& Asc=true) { Sort(true, Asc); }
  void SortByDat(const bool& Asc=true) { Sort(false, Asc); }*/
};

template<class TKey, class TDat, class TSizeTy, class THashFunc>
const unsigned int TPHash<TKey, TDat, TSizeTy, THashFunc>::HashPrimeT[HashPrimes]={
  3ul, 5ul, 11ul, 23ul,
  53ul,         97ul,         193ul,       389ul,       769ul,
  1543ul,       3079ul,       6151ul,      12289ul,     24593ul,
  49157ul,      98317ul,      196613ul,    393241ul,    786433ul,
  1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
  50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul,
  1610612741ul, 3221225473ul, 4294967291ul
};

template<class TKey, class TDat, class TSizeTy, class THashFunc>
uint TPHash<TKey, TDat, TSizeTy, THashFunc>::GetNextPrime(const uint& Val) const {
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
bool TPHash<TKey, TDat, TSizeTy, THashFunc>::operator==(const TPHash& Hash) const {
  if (Len() != Hash.Len()) { return false; }
  for (int i = FFirstKeyId(); FNextKeyId(i); ) {
    const TKey& Key = GetKey(i);
    if (! Hash.IsKey(Key)) { return false; }
    if (GetDat(Key) != Hash.GetDat(Key)) { return false; }
  }
  return true;
}

template<class TKey, class TDat, class TSizeTy, class THashFunc>
int TPHash<TKey, TDat, TSizeTy, THashFunc>::SafeAddKey(const TKey& Key) {
  //int CurVals = __sync_fetch_and_add(&NumVals.Val, 1);
  //IAssertR(CurVals < Table.Len(), "Table must not be full");

  const int BegTableN=abs(Key.GetPrimHashCd()%Table.Len());
  const int HashCd=abs(Key.GetSecHashCd());

  int TableN = BegTableN;
  while (1) {
    if (Table[TableN].Status == 0 && 
	__sync_bool_compare_and_swap(&Table[TableN].Status.Val, 0, 1)) {
      Table[TableN].HashCd = HashCd;
      Table[TableN].Key = Key;
      __sync_fetch_and_add(&Table[TableN].Status.Val, 1);
      break;
    } else if (Table[TableN].Status == 1) {
      continue;
    } else if (Table[TableN].HashCd == HashCd &&
	       Table[TableN].Key == Key) {
      break;
    } else {
      TableN = (TableN + 1) % Table.Len();
    }
  }
  return TableN;
}

template<class TKey, class TDat, class TSizeTy, class THashFunc>
int TPHash<TKey, TDat, TSizeTy, THashFunc>::GetKeyId(const TKey& Key) const {
  const int BegTableN=abs(Key.GetPrimHashCd()%Table.Len());
  const int HashCd=abs(Key.GetSecHashCd());
 
  int TableN = BegTableN;
  while (Table[TableN].HashCd != -1) {
    if (Table[TableN].Status == 2 && Table[TableN].Key == Key) { return TableN; }
    TableN = (TableN + 1) % Table.Len();
    if (TableN == BegTableN) { return -1; }
  }

  return -1;
}

template<class TKey, class TDat, class TSizeTy, class THashFunc>
void TPHash<TKey, TDat, TSizeTy, THashFunc>::Clr(const bool& DoDel){
  if (DoDel){
    Table.Clr();
  } else {
    Table.PutAll(TPHKeyDat());
  }
  NumVals = TInt(0);
}

template<class TKey, class TDat, class TSizeTy, class THashFunc>
bool TPHash<TKey, TDat, TSizeTy, THashFunc>::FNextKeyId(int& KeyId) const {
  do {KeyId++;} while ((KeyId<Table.Len())&&(Table[KeyId].Status==0));
  return KeyId<Table.Len();
}

template<class TKey, class TDat, class TSizeTy, class THashFunc>
void TPHash<TKey, TDat, TSizeTy, THashFunc>::GetKeyV(TVec<TKey>& KeyV) const {
  KeyV.Gen(Len(), 0);
  int KeyId=FFirstKeyId();
  while (FNextKeyId(KeyId)){
    KeyV.Add(GetKey(KeyId));}
}

template<class TKey, class TDat, class TSizeTy, class THashFunc>
void TPHash<TKey, TDat, TSizeTy, THashFunc>::GetDatV(TVec<TDat>& DatV) const {
  DatV.Gen(Len(), 0);
  int KeyId=FFirstKeyId();
  while (FNextKeyId(KeyId)){
    DatV.Add(GetPHashKeyDat(KeyId).Dat);}
}

template<class TKey, class TDat, class TSizeTy, class THashFunc>
void TPHash<TKey, TDat, TSizeTy, THashFunc>::GetKeyDatPrV(TVec<TPair<TKey, TDat> >& KeyDatPrV) const {
  KeyDatPrV.Gen(Len(), 0);
  TKey Key; TDat Dat;
  int KeyId=FFirstKeyId();
  while (FNextKeyId(KeyId)){
    GetKeyDat(KeyId, Key, Dat);
    KeyDatPrV.Add(TPair<TKey, TDat>(Key, Dat));
  }
}

template<class TKey, class TDat, class TSizeTy, class THashFunc>
void TPHash<TKey, TDat, TSizeTy, THashFunc>::GetDatKeyPrV(TVec<TPair<TDat, TKey> >& DatKeyPrV) const {
  DatKeyPrV.Gen(Len(), 0);
  TKey Key; TDat Dat;
  int KeyId=FFirstKeyId();
  while (FNextKeyId(KeyId)){
    GetKeyDat(KeyId, Key, Dat);
    DatKeyPrV.Add(TPair<TDat, TKey>(Dat, Key));
  }
}

template<class TKey, class TDat, class TSizeTy, class THashFunc>
void TPHash<TKey, TDat, TSizeTy, THashFunc>::GetKeyDatKdV(TVec<TKeyDat<TKey, TDat> >& KeyDatKdV) const {
  KeyDatKdV.Gen(Len(), 0);
  TKey Key; TDat Dat;
  int KeyId=FFirstKeyId();
  while (FNextKeyId(KeyId)){
    GetKeyDat(KeyId, Key, Dat);
    KeyDatKdV.Add(TKeyDat<TKey, TDat>(Key, Dat));
  }
}

template<class TKey, class TDat, class TSizeTy, class THashFunc>
void TPHash<TKey, TDat, TSizeTy, THashFunc>::GetDatKeyKdV(TVec<TKeyDat<TDat, TKey> >& DatKeyKdV) const {
  DatKeyKdV.Gen(Len(), 0);
  TKey Key; TDat Dat;
  int KeyId=FFirstKeyId();
  while (FNextKeyId(KeyId)){
    GetKeyDat(KeyId, Key, Dat);
    DatKeyKdV.Add(TKeyDat<TDat, TKey>(Dat, Key));
  }
}

template<class TKey, class TDat, class TSizeTy, class THashFunc>
void TPHash<TKey, TDat, TSizeTy, THashFunc>::Swap(TPHash& Hash) {
  if (this!=&Hash){
    Table.Swap(Hash.Table);
    ::Swap(NumVals, Hash.NumVals);
  }
}

#endif
