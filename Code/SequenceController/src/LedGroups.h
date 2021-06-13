class LedGroup
{
public:
    int _ledGroupNumber;
    int _ledCount;
    ILedDevice *_pLedDevice;
};

class LedGroups
{
    static const int MaxLedGroups = 16;

public:
    int _groupCount = 0;
    LedGroup _ledGroups[MaxLedGroups];

    LedGroups()
    {
        for (int i = 0; i < MaxLedGroups; i++)
        {
            _ledGroups[i]._ledGroupNumber = -1;
            _ledGroups[i]._ledCount = -1;
            _ledGroups[i]._pLedDevice = 0;
        }
    }

    int GetGroupCount()
    {
        return _groupCount;
    }

    bool AddGroup(int groupNumber, ILedDevice *pLedDevice, int ledCount)
    {
        if (GetGroup(groupNumber) != 0)
        {
            return false;
        }

        if (groupNumber != _groupCount)
        {
            return false;
        }

        _ledGroups[_groupCount]._ledCount = ledCount;
        _ledGroups[_groupCount]._ledGroupNumber = groupNumber;
        _ledGroups[_groupCount]._pLedDevice = pLedDevice;
        _groupCount++;

#if fred
        Serial.println("LedGroups::AddGroup");
        Serial.print(ledCount);
        Serial.print(" ");
        Serial.print(groupNumber);
        Serial.print(" ");
        Serial.println((int)pLedDevice);
        Serial.println(_groupCount);
#endif

        return true;
    }

    LedGroup *GetGroup(int groupNumber)
    {
        for (int i = 0; i < MaxLedGroups; i++)
        {
            if (_ledGroups[i]._ledGroupNumber == groupNumber)
            {
                return &_ledGroups[i];
            }
        }
        return 0;
    }

    int GetLedTotal()
    {
        int ledTotal = 0;

        for (int i = 0; i < _groupCount; i++)
        {
            ledTotal += _ledGroups[i]._ledCount;
        }

        return ledTotal;
    }

    bool UpdateLed(LedState ledState)
    {
        return UpdateLed(ledState.GetChannel(), ledState.GetBrightness());
    }

    bool UpdateLed(int channel, Variable* pBrightness)
    {
        //Serial.println("LedGroups::UpdateLed");
    //Serial.println(ledState.GetBrightness()->GetValueFloat(0));
    //Serial.println(ledState.GetChannel());
    //Serial.println(ledState.GetCycleCount());

        // find the group that this belongs to...
        for (int i = 0; i < _groupCount; i++)
        {
            if (channel < _ledGroups[i]._ledCount)
            {
                _ledGroups[i]._pLedDevice->UpdateLed(channel, pBrightness);
                return true;
            }
            else
            {
                channel -= _ledGroups[i]._ledCount;
            }
        }

        return false;
    }

#if fred
    bool UpdateLed(LedState* pLedState)
    {
        //Serial.println("LedGroups::UpdateLed");
        //Serial.println(ledState.GetBrightness()->GetValueFloat(0));
        //Serial.println(ledState.GetChannel());
        //Serial.println(ledState.GetCycleCount());

        int channel = pLedState->GetChannel();

        // find the group that this belongs to...
        for (int i = 0; i < _groupCount; i++)
        {
            //Serial.println(i);
            //Serial.println(ledState.GetChannel());
            if (pLedState->GetChannel() < _ledGroups[i]._ledCount)
            {
                //Serial.println(">Found");
                //Serial.println((int)_ledGroups[i]._pLedDevice);
                _ledGroups[i]._pLedDevice->UpdateLed(pLedState->GetChannel(), pLedState->GetBrightness());
                //Serial.println("<Found");
                return true;
            }
            else
            {
                pLedState->SetChannel(pLedState->GetChannel() - _ledGroups[i]._ledCount);
            }
        }

        return false;
    }
#endif

    void Show()
    {
        for (int i = 0; i < _groupCount; i++)
        {
            _ledGroups[i]._pLedDevice->Show();
        }
    }
};