#pragma once

#include <functional>
#include <string>

namespace Engine::Low
{
    struct NativeWindow;
}

namespace Engine
{
    struct WindowCreateInfo
    {
        int height{};
        int width{};
        std::string title{};
    };

    enum class Key
    {
        Space = 32,

        _0 = 48,
        _1 = 49,
        _2 = 50,
        _3 = 51,
        _4 = 52,
        _5 = 53,
        _6 = 54,
        _7 = 55,
        _8 = 56,
        _9 = 57,

        A = 65,
        B = 66,
        C = 67,
        D = 68,
        E = 69,
        F = 70,
        G = 71,
        H = 72,
        I = 73,
        J = 74,
        K = 75,
        L = 76,
        M = 77,
        N = 78,
        O = 79,
        P = 80,
        Q = 81,
        R = 82,
        S = 83,
        T = 84,
        U = 85,
        V = 86,
        W = 87,
        X = 88,
        Y = 89,
        Z = 90,

        Escape = 256,

        Right = 262,
        Left = 263,
        Down = 264,
        Up = 265
    };
}
