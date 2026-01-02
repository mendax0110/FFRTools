#include "Colourisers.h"
#include <cmath>
#include <limits>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace Colourisers;


#if false

void ColouriserCreator::colourise(const double magnitude, const double limit, int8_t &red, int8_t &green, int8_t &blue)
{
	double normalised = magnitude / limit;
	
	// double r = sin(normalised) - (M_PI / 2);
	// double g = sin(normalised);
	// double b = sin(normalised) + (M_PI / 2);
	
	red = toRed(magnitude, limit);
	green = toGreen(magnitude, limit);
	blue = toBlue(magnitude, limit);
}

#else

void ColouriserCreator::colourise(const double magnitude, const double limit, int8_t &red, int8_t &green, int8_t &blue)
{
	const double normalised = magnitude / limit;
	
	constexpr double sensitivity = 0.1;
	
	if(normalised > sensitivity)
	{
		HlsToRgb(normalised, 0.5, 0.5, red, green, blue);
	}
	else
	{
		red = 0;
		green = 0;
		blue = 0;
	}
}

#endif

int8_t ColouriserCreator::toBlue(const double magnitude, const double limit)
{
	double normalised = magnitude / limit;
	
	normalised = sin(normalised) + (M_PI / 2);
	
	//normalised = magnitude / limit;
	normalised = std::max(normalised, 0.0);
	normalised = std::min(normalised, 1.0);
	
	const int8_t value = static_cast< int8_t >(normalised * std::numeric_limits< int8_t >::max());
	return value;
}

int8_t ColouriserCreator::toGreen(const double magnitude, const double limit)
{
	double normalised = magnitude / limit;

	normalised = sin(normalised);
	
	//normalised = magnitude / limit;
	normalised = std::max(normalised, 0.0);
	normalised = std::min(normalised, 1.0);
	
	const int8_t value = static_cast< int8_t >(normalised * std::numeric_limits< int8_t >::max());
	return value;
}

int8_t ColouriserCreator::toRed(const double magnitude, const double limit)
{
	double normalised = magnitude / limit;

	normalised = sin(normalised) - (M_PI / 2);
	
	//normalised = magnitude / limit;
	normalised = std::max(normalised, 0.0);
	normalised = std::min(normalised, 1.0);

	const int8_t value = static_cast< int8_t >(normalised * std::numeric_limits< int8_t >::max());
	return value;
}

void ColouriserCreator::RgbToHls(const double r, const double g,  const double b,double &h, double &l, double s)
{
	// Get the maximum and minimum RGB components.
    double maxVal = r;
    if(maxVal < g) maxVal = g;
    if(maxVal < b) maxVal = b;

    double minVal = r;
    if(minVal > g) minVal = g;
    if(minVal > b) minVal = b;

    const double diff = maxVal - minVal;
    l = (maxVal + minVal) / 2;
	
	const double aDiff = (diff < 0.0) ? -diff : diff;
    if(aDiff < 0.00001)
	{
        s = 0;
        h = 0;
	}
	else
	{
		if(l <= 0.5)
		{
            s = diff / (maxVal + minVal);
		}
        else
        {
			s = diff / (2 - maxVal - minVal);
		}

        const double r_dist = (maxVal - r) / diff;
        const double g_dist = (maxVal - g) / diff;
        const double b_dist = (maxVal - b) / diff;

        if(r == maxVal)
		{
            h = b_dist - g_dist;
		}
        else if(g == maxVal)
		{
            h = 2 + r_dist - b_dist;
		}
		else
		{
            h = 4 + g_dist - r_dist;
		}

        h = h * 60;
        if(h < 0)
		{
			h = h + 360;
		}
	}
}

void ColouriserCreator::HlsToRgb(const double h, const double l, const double s, int8_t &red, int8_t &green, int8_t &blue)
{
	double p2;
	double r, g, b;

    if(l <= 0.5)
	{
        p2 = l * (1 + s);
    }
	else
	{
        p2 = l + s - l * s;
	}
	
    const double p1 = 2 * l - p2;
    if(s == 0)
	{
        r = l;
        g = l;
        b = l;
	}
	else
	{
        r = QqhToRgb(p1, p2, h + 120);
        g = QqhToRgb(p1, p2, h);
        b = QqhToRgb(p1, p2, h - 120);
    }
	
	red = static_cast< int8_t >(r * std::numeric_limits< int8_t >::max());
	green = static_cast< int8_t >(g * std::numeric_limits< int8_t >::max());
	blue = static_cast< int8_t >(b * std::numeric_limits< int8_t >::max());
}

double ColouriserCreator::QqhToRgb(const double q1, const double q2, double hue)
{
    if(hue > 360)
	{
        hue = hue - 360;
    }
	else if(hue < 0)
	{
        hue = hue + 360;
    }
	
    if(hue < 60)
	{
        return q1 + (q2 - q1) * hue / 60;
	}
    else if(hue < 180)
	{
        return q2;
	}
    else if(hue < 240)
	{
        return q1 + (q2 - q1) * (240 - hue) / 60;
    }
	else
	{
        return q1;
    }
}
