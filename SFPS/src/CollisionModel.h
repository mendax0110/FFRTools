#ifndef COLLISIONMODEL_H_INCLUDED
#define COLLISIONMODEL_H_INCLUDED

/**
 * @brief Collision model with bounds and a centre that is 'solid'.
 */
template< typename Point >
class CollisionModel
{
public:
    typedef Point point_t;

    /**
     * @brief Constructor for CollisionModel.
     * @param area The size of the area.
     * @param centre The size of the central solid area.
     */
    CollisionModel(const point_t& area, const point_t& centre)
        :   m_area(area)
        ,   m_centre(centre)
    {
    }

    /**
     * @brief Check if a point collides with the solid area.
     * @param p The point to check.
     * @return True if the point collides, false otherwise.
     */
    bool operator()(const point_t& p)
    {
        if((p.x.value >= m_area.x.value)
            || (p.y.value >= m_area.y.value)
            || (p.z.value >= m_area.z.value))
        {
            return true;
        }

        if((p.x.value <= m_centre.x.value)
            || (p.y.value <= m_centre.y.value)
            || (p.z.value <= m_centre.z.value))
        {
            return true;
        }

        return false;
    }

private:
    const point_t m_area;
    const point_t m_centre;
};

#endif // COLLISIONMODEL_H_INCLUDED
