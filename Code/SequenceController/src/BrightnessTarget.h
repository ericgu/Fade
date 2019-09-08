class BrightnessTarget
{
    LedState _targets[16];
    int _targetCount = 0;

    public:
        void AddTarget(LedState ledState)
        {
            _targets[_targetCount] = ledState;
            _targetCount++;
        }

        LedState GetTarget(int index)
        {
            return _targets[index];
        }

        int GetCount() { return _targetCount;}

		void Clear() 
		{
			_targetCount = 0;
		}
};