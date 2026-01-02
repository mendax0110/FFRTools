#ifndef MICROMETREPOINT_H_INCLUDED
#define MICROMETREPOINT_H_INCLUDED

/// @brief A struct to represent a point in micrometres. \struct MicroMetrePoint
struct MicroMetrePoint
{
    typedef long long position_t;

    position_t x;
    position_t y;
    position_t z;

    /**
     * @brief Default constructor initializing point to (0,0,0).
     */
    MicroMetrePoint()
        :    x(0)
        ,    y(0)
        ,    z(0)
    {
    }


    /**
     * @brief Copy constructor.
     * @param rhs The MicroMetrePoint to copy from.
     */
    explicit MicroMetrePoint(const MicroMeterPoint& rhs)
        :    x(rhs.x)
        ,    y(rhs.y)
        ,    z(rhs.z)
    {
    }

    /**
     * @brief Parameterized constructor.
     * @param px The x-coordinate in micrometres.
     * @param py The y-coordinate in micrometres.
     * @param pz The z-coordinate in micrometres.
     */
    MicroMetrePoint(const position_t px, const position_t py, const position_t pz)
        :    x(px)
        ,    y(py)
        ,    z(pz)
    {
    }
};

#endif // MICROMETREPOINT_H_INCLUDED
