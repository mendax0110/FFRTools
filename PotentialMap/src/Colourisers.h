#if !defined(COLOURISERS_H)
#define COLOURISERS_H

#include <cmath>
#include <stdint.h>
#include <algorithm>

/// @brief Color Mapping Utilities. \namespace Colourisers
namespace Colourisers
{
    /// @brief Color Mapper for Potential Visualization. \class ColouriserCreator
    class ColouriserCreator
    {
    public:
        /**
         * @brief Convert magnitude to RGB color values.
         * @param magnitude The magnitude value to convert.
         * @param limit The maximum limit for normalization.
         * @param red Output red component (0-127).
         * @param green Output green component (0-127).
         * @param blue Output blue component (0-127).
         */
        static void colourise(double magnitude, double limit, int8_t &red, int8_t &green, int8_t &blue);

        /**
         * @brief Convert RGB to HLS color space.
         * @param r Red component (0.0-1.0).
         * @param g Green component (0.0-1.0).
         * @param b Blue component (0.0-1.0).
         * @param h Output hue (0-360 degrees).
         * @param l Output lightness (0.0-1.0).
         * @param s Output saturation (0.0-1.0).
         */
        static void RgbToHls(double r, double g, double b, double &h, double &l, double &s);

        /**
         * @brief Convert HLS to RGB color space.
         * @param h Hue (0-360 degrees).
         * @param l Lightness (0.0-1.0).
         * @param s Saturation (0.0-1.0).
         * @param red Output red component (0-127).
         * @param green Output green component (0-127).
         * @param blue Output blue component (0-127).
         */
        static void HlsToRgb(double h, double l, double s, int8_t &red, int8_t &green, int8_t &blue);

        /**
         * @brief Helper function for HLS to RGB conversion.
         * @param q1 First intermediate value.
         * @param q2 Second intermediate value.
         * @param hue Hue value.
         * @return RGB component value (0.0-1.0).
         */
        static double QqhToRgb(double q1, double q2, double hue);

        /**
         * @brief Convert magnitude to red component.
         * @param magnitude The magnitude value.
         * @param limit The maximum limit for normalization.
         * @return Red component value (0-127).
         */
        static int8_t toRed(double magnitude, double limit);

        /**
         * @brief Convert magnitude to green component.
         * @param magnitude The magnitude value.
         * @param limit The maximum limit for normalization.
         * @return Green component value (0-127).
         */
        static int8_t toGreen(double magnitude, double limit);

        /**
         * @brief Convert magnitude to blue component.
         * @param magnitude The magnitude value.
         * @param limit The maximum limit for normalization.
         * @return Blue component value (0-127).
         */
        static int8_t toBlue(double magnitude, double limit);
    };
}

#endif // COLOURISERS_H
