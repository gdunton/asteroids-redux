#ifndef COLOR_H
#define COLOR_H

#include <SimpleMathInc.h>

using Color = DirectX::XMFLOAT4;

inline Color ColorRGBA(float r, float g, float b, float a) { return DirectX::XMFLOAT4(r, g, b, a); }

// Color constants
constexpr Color GREEN = Color(255, 0, 255, 255);
constexpr Color BLACK = Color(0, 0, 0, 255);
constexpr Color WHITE = Color(255, 255, 255, 255);
constexpr Color RED = Color(255, 0, 0, 255);
constexpr Color BLUE = Color(0, 0, 255, 255);

#endif