#ifndef MICROMETREPOINT_H_INCLUDED
#define MICROMETREPOINT_H_INCLUDED

struct MicroMetrePoint
{
    typedef long long position_t;

    position_t x;
    position_t y;
    position_t z;

    MicroMetrePoint()
        :    x(0)
        ,    y(0)
        ,    z(0)
    {
    }


    MicroMetrePoint(const MicroMeterPoint& rhs)
        :    x(rhs.x)
        ,    y(rhs.y)
        ,    z(rhs.z)
    {
    }

    MicroMetrePoint(position_t px, position_t py, position_t pz)
        :    x(px)
        ,    y(py)
        ,    z(pz)
    {
    }
};

#endif // MICROMETREPOINT_H_INCLUDED
