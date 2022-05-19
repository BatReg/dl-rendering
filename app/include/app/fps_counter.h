#pragma once

namespace App
{
    class FpsCounter
    {
    public:
        FpsCounter(float updateInterval = 0.5f) noexcept;

        void Update(float deltaTime);

        float FPS() const;
        float DeltaTime() const;
        
    private:
        float _updateInterval = 0.5f;

        int _frames{};
        float _accum{};

        float _avgDelta{};
        float _avgFPS{};

        float _timeLeft{};
    };
}
