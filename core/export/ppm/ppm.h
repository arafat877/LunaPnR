#pragma once
#include <cstdint>
#include <vector>

namespace LunaCore::PPM
{

    /** color tripple stored in a 4-byte structure */
    struct RGB
    {
        uint8_t r,g,b, dummy{0};

        [[nodiscard]] constexpr bool isBlack() const noexcept
        {
            return (r==0) && (g==0) && (b==0);
        }        

        [[nodiscard]] constexpr bool operator==(const RGB &rhs) const noexcept
        {
            return (r==rhs.r) && (g==rhs.g) && (b==rhs.b);
        }

    } __attribute__((packed));

    /** interpolate two colors. frac = 0.0 .. 1.0 */
    constexpr RGB interpolate(const RGB &col1, const RGB &col2, float frac)
    {
        if (frac > 1.0f) frac = 1.0f;
        uint8_t r = static_cast<uint8_t>(col1.r + (col2.r-col1.r)*frac);
        uint8_t g = static_cast<uint8_t>(col1.g + (col2.g-col1.g)*frac);
        uint8_t b = static_cast<uint8_t>(col1.b + (col2.b-col1.b)*frac);
        return RGB{r,g,b,0};
    }     

    struct Bitmap
    {
        std::size_t m_width{0};
        std::size_t m_height{0};
        std::vector<RGB> m_data;
    };

};