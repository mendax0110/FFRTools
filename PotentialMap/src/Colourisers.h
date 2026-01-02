#if !defined(COLOURISERS_H)
#define COLOURISERS_H

#include <math.h>
#include <stdint.h>
#include <algorithm>

namespace Colourisers
{
    class ColouriserCreator
    {
    public:
        static void colourise(double magnitude, double limit, int8_t &red, int8_t &green, int8_t &blue);
        static void RgbToHls(double r, double g,  double b, double &h, double &l, double s);
        static void HlsToRgb(double h, double l, double s, int8_t &red, int8_t &green, int8_t &blue);
        static double QqhToRgb(double q1, double q2, double hue);

        static int8_t toRed(double magnitude, double limit);

        static int8_t toGreen(double magnitude, double limit);

        static int8_t toBlue(double magnitude, double limit);
    };
}

#endif // COLOURISERS_H
