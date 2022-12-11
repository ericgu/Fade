class StringChunk
{

  char* _pStringsBuffer;
  int _stringChunkSize;
  //int _stringsInUse;

public:
  static const int StringMaxLength = 128;

  static const int StringChunkSize = 256;

  StringChunk(int stringsChunkSize = StringChunkSize)
  {
    _stringChunkSize = stringsChunkSize;
    _pStringsBuffer = new char[stringsChunkSize];
    *_pStringsBuffer = '\r';                // end of data placeholder.
  }

  ~StringChunk()
  {
    delete _pStringsBuffer;
    _pStringsBuffer = 0;
  }

  const char* LookupOrAdd(const char* pString)
  {
    char* pCurrent = _pStringsBuffer;

    while (*pCurrent != '\r')
    {
      if (strcmp(pString, pCurrent) == 0)
      {
        return pCurrent;
      }

      pCurrent++;
    }

    // Not found. Will it fit here? 

    int spaceUsed = pCurrent - _pStringsBuffer;

    if (spaceUsed + strlen(pString) + 2 > _stringChunkSize)
    {
      return 0;
    }

    char* pInternedLocation = pCurrent;

    strcpy(pCurrent, pString);

    while (*pCurrent != '\0')
    {
      pCurrent++;
    }
    pCurrent++;
    *pCurrent = '\r';

    return pInternedLocation;
  }

};

class InternedStrings
{
  static const int MaxStringChunks = 16;
  static const int StringsInChunk = 64;

  StringChunk* _pStringChunks[MaxStringChunks];
  int _stringChunkCount;
  const char* _pSpace = "";

public:
  InternedStrings()
  {
    _stringChunkCount = 0;
  }

  const char* LookupOrAdd(const char* pString)
  {
    if (*pString == 0)
    {
      return _pSpace;
    }

    bool logging = false;

    if (logging)
    {
      Serial.print(">");
      Serial.print(pString);
      Serial.println("<");
    }

    if (strlen(pString) > StringChunk::StringMaxLength - 1)
    {
      return "StringToLong";
    }

    for (int i = 0; i < _stringChunkCount; i++)
    {
      const char* pLookup = _pStringChunks[i]->LookupOrAdd(pString);
      if (pLookup != 0)
      {
        return pLookup;   // found or added...
      }
    }

    if (logging)
    {
      Serial.println("Extend");
      Serial.flush();
    }

    // no space; add new chunk...
    if (_stringChunkCount == MaxStringChunks)
    {
      Serial.println("Too many interned strings!");
      return pString;
    }

    StringChunk* pNewStringChunk = new StringChunk();
    _pStringChunks[_stringChunkCount] = pNewStringChunk;
    _stringChunkCount++;

    return pNewStringChunk->LookupOrAdd(pString);
  }
};



#if fred
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

  const char* LookupOrAdd(const char* pString)
  {
    char *pCurrent = _pStringsBuffer;

    for (int i = 0; i < _stringsInUse; i++)
    {
      if (strcmp(pString, pCurrent) == 0)
      {
        return pCurrent;
      }

      if (strcmp(pString, pCurrent + 1) == 0) // variables are first stored as $name so this is an optimization to reuse the string. 
      {
        return pCurrent + 1;
      }


      pCurrent += StringMaxLength;
    }

    if (_stringsInUse == _stringsInChunk)
    {
      return 0;
    }

//    Serial.print(">");
//    Serial.print(pString);
//    Serial.println("<");

    strcpy(pCurrent, pString);
    _stringsInUse++;

    return pCurrent;
  }

};

class InternedStrings
{
  static const int MaxStringChunks = 16;
  static const int StringsInChunk = 64;

  StringChunk* _pStringChunks[MaxStringChunks];
  int _stringChunkCount;
  const char* _pSpace = "";

public:
  InternedStrings()
  {
    _stringChunkCount = 0;
  }

  const char* LookupOrAdd(const char* pString)
  {
    if (*pString == 0)
    {
      return _pSpace;
    }

    bool logging = false;

    if (logging)
    {
      Serial.print(">");
      Serial.print(pString);
      Serial.println("<");
    }

    if (strlen(pString) > StringChunk::StringMaxLength - 1)
    {
      return "StringToLong";
    }

    for (int i = 0; i < _stringChunkCount; i++)
    {
      const char* pLookup = _pStringChunks[i]->LookupOrAdd(pString);
      if (pLookup != 0)
      {
        return pLookup;   // found or added...
      }
    }

    if (logging)
    {
      Serial.println("Extend");
      Serial.flush();
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

#endif