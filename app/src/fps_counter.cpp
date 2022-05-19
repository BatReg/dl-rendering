#include <app/fps_counter.h>

namespace App
{
    FpsCounter::FpsCounter(float updateInterval) noexcept
    {
        _updateInterval = updateInterval;
        _timeLeft = _updateInterval;
    }

    void FpsCounter::Update(float deltaTime)
    {
        _timeLeft -= deltaTime;
        _accum += deltaTime;
        ++_frames;

        if(_timeLeft <= 0)
        {
            _avgDelta = _accum / _frames;
            _avgFPS = (_frames / _accum);
            _timeLeft = _updateInterval;
            _accum = 0.0f;
            _frames = 0;
        }
    }

    float FpsCounter::FPS() const
    {
        return _avgFPS;
    }

    float FpsCounter::DeltaTime() const
    {
        return _avgDelta;
    }
}
