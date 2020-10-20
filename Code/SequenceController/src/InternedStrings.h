class StringChunk
{

  char* _pStringsBuffer;
  int _stringsInChunk;
  int _stringsInUse;

public:
  static const int StringMaxLength = 128;

  StringChunk(int stringsInChunk)
  {
    _stringsInChunk = stringsInChunk;
    _stringsInUse = 0;
    _pStringsBuffer = new char[stringsInChunk * StringMaxLength];
  }

  ~StringChunk()
  {
    delete[] _pStringsBuffer;
    _pStringsBuffer = 0;
  }

  char* LookupOrAdd(char* pString)
  {
    char *pCurrent = _pStringsBuffer;

    for (int i = 0; i < _stringsInUse; i++)
    {
      if (strcmp(pString, pCurrent) == 0)
      {
        return pCurrent;
      }

      pCurrent += StringMaxLength;
    }

    if (_stringsInUse == _stringsInChunk)
    {
      return 0;
    }

    strcpy(pCurrent, pString);
    _stringsInUse++;

    return pCurrent;
  }

};

class InternedStrings
{
  static const int MaxStringChunks = 64;
  static const int StringsInChunk = 64;

  StringChunk* _pStringChunks[MaxStringChunks];
  int _stringChunkCount;

public:
  InternedStrings()
  {
    _stringChunkCount = 0;
  }

  char* LookupOrAdd(char* pString)
  {
    if (strlen(pString) > StringChunk::StringMaxLength - 1)
    {
      return "StringToLong";
    }

    for (int i = 0; i < _stringChunkCount; i++)
    {
      char* pLookup = _pStringChunks[i]->LookupOrAdd(pString);
      if (pLookup != 0)
      {
        return pLookup;   // found or added...
      }
    }

    // no space; add new chunk...
    if (_stringChunkCount == MaxStringChunks)
    {
      Serial.println("Too many interned strings!");
      return pString;
    }

    StringChunk* pNewStringChunk = new StringChunk(StringsInChunk);
    _pStringChunks[_stringChunkCount] = pNewStringChunk;
    _stringChunkCount++;

    return pNewStringChunk->LookupOrAdd(pString);
  }
};