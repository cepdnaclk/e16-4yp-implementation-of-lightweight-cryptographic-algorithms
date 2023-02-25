#include <cstdint>


inline float s0_prob(uint8_t x)
{
    return x == 0 ? 1.f : 0.f;
}

inline float s47_prob(uint8_t x)
{
    return (x == 0x80 || x == 0) ? 0.5f : 0;
}

inline float s70_prob(uint8_t x)
{
    return (x == 0xC0 || x == 0x80 || x == 0x40 || x == 0x00) ? 0.25f : 0.f;
}

inline float s85_prob(uint8_t x)
{
    return (x == 0xE0 || x == 0xC0 || x == 0xA0 || x == 0x80 || x == 0x60 || x == 0x40 || x == 0x20 || x == 0x00) ? 1.f / 8.f : 0.f;
}

inline float s91_prob(uint8_t x)
{
    return x <= 7 ? 1.f / 8.f : 0.f;
}
