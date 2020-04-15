class SafeString
{
    public:

    static int StringLength(const char *pString)
    {
        int count = 0;
        while (*pString != 0)
        {
            count++;
			pString++;
        }

        return count;
    }

    static void StringCopy(char* pDest, const char* pSource, int bufferSize)
    {
        const char* pCurrent = pSource;
        while (*pCurrent != 0 && (pCurrent - pSource) < bufferSize - 1)
        {
            *pDest = *pCurrent;
            pDest++;
            pCurrent++;
        }
        *pDest = '\0';
    }

    static void StringCat(char* pDest, const char* pSource, int bufferSize)
    {
        int currentLength = StringLength(pDest);
        bufferSize -= currentLength;  // these characters are already used.
        pDest += currentLength;

        const char* pCurrent = pSource;

        while (*pCurrent != 0 && (pCurrent - pSource) < bufferSize - 1)
        {
            *pDest = *pCurrent;
            pDest++;
            pCurrent++;
        }
        *pDest = '\0';
    }
};