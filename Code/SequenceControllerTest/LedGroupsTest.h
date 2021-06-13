#pragma once

class LedDeviceSimulator : public ILedDevice
{
    int _updateCount;
    int _maxUpdates;
    LedState* _pUpdatedStates;
    bool _showCalled = false;

    int* _pChannels;
    Variable* _pBrightnesses;



public:
    LedDeviceSimulator(int maxUpdates)
    {
        _updateCount = 0;
        _maxUpdates = maxUpdates;
        _pUpdatedStates = new LedState[_maxUpdates];
//        _pChannels = new int[_maxUpdates];
//        _pBrightnesses = new Variable[_maxUpdates];

    }

    ~LedDeviceSimulator()
    {
        delete[] _pUpdatedStates;
        _pUpdatedStates = 0;
    }

    virtual void UpdateLed(int channel, Variable *pBrightness)
    {
        LedState ledState;
        ledState.SetChannel(channel);
        ledState.SetBrightness(*pBrightness);

        UpdateLed(ledState);
    }


    void UpdateLed(LedState ledState)
    {
        if (_updateCount >= _maxUpdates)
        {
            Serial.println("max updates exceeded");
            return;
        }

        *(_pUpdatedStates + _updateCount) = ledState;

        _updateCount++;
    }

    void Show()
    {
        _showCalled = true; 
    }

    bool GetShowCalled()
    {
        return _showCalled;
    }

    int GetUpdateCount() { return _updateCount; }

    LedState GetUpdatedState(int index)
    {
        return *(_pUpdatedStates + index);
    }
};

#include "LedGroups.h"

class LedGroupsTest
{
    static void TestTwoGroups()
    {
        LedGroups ledGroups;

        Assert::AreEqual(0, ledGroups.GetGroupCount());
        LedDeviceSimulator led1(10);
        LedDeviceSimulator led2(10);
        ledGroups.AddGroup(0, &led1, 5);
        ledGroups.AddGroup(1, &led2, 10);

        Assert::AreEqual(2, ledGroups.GetGroupCount());

        Assert::AreEqual(0, ledGroups.GetGroup(0)->_ledGroupNumber);
        Assert::AreEqual(5, ledGroups.GetGroup(0)->_ledCount);

        Assert::AreEqual(1, ledGroups.GetGroup(1)->_ledGroupNumber);
        Assert::AreEqual(10, ledGroups.GetGroup(1)->_ledCount);

        Assert::AreEqual(15, ledGroups.GetLedTotal());
    }

    static void TestUpdate()
    {
        LedGroups ledGroups;

        Assert::AreEqual(0, ledGroups.GetGroupCount());
        LedDeviceSimulator led1(10);
        LedDeviceSimulator led2(10);
        ledGroups.AddGroup(0, &led1, 5);
        ledGroups.AddGroup(1, &led2, 10);

        LedState ledState(0, 1.0, 15);
        ledGroups.UpdateLed(ledState);

        Assert::AreEqual(0, led1.GetUpdatedState(0).GetChannel());

        LedState ledState2(3, 0.8F, 11);
        ledGroups.UpdateLed(ledState2);

        Assert::AreEqual(3, led1.GetUpdatedState(1).GetChannel());

        // second group...

        LedState ledState3(7, 0.8F, 22);
        ledGroups.UpdateLed(ledState3);
        Assert::AreEqual(2, led2.GetUpdatedState(0).GetChannel());

        LedState ledState4(12, 0.8F, 23);
        ledGroups.UpdateLed(ledState4);
        Assert::AreEqual(7, led2.GetUpdatedState(1).GetChannel());

        LedState ledState5(14, 0.8F, 28);
        ledGroups.UpdateLed(ledState5);
        Assert::AreEqual(9, led2.GetUpdatedState(2).GetChannel());

            // out of range...
        LedState ledState6(15, 0.8F, 22);
        Assert::AreEqual(0, ledGroups.UpdateLed(ledState6));

    }

    static void TestShow()
    {
        LedGroups ledGroups;

        Assert::AreEqual(0, ledGroups.GetGroupCount());
        LedDeviceSimulator led1(10);
        LedDeviceSimulator led2(10);
        ledGroups.AddGroup(0, &led1, 5);
        ledGroups.AddGroup(1, &led2, 10);

        ledGroups.Show();
        Assert::AreEqual(1, led1.GetShowCalled());
        Assert::AreEqual(1, led2.GetShowCalled());
    }

public:

    static int Run()
    {
        TestTwoGroups();
        TestUpdate();
        TestShow();


        return 0;
    }

};