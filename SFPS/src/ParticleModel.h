#ifndef PARTICLEMODEL_H_INCLUDED
#define PARTICLEMODEL_H_INCLUDED

#include "Quantities.h"

#include <vector>

/**
 * @brief ParticleModel simulates the movement of a particle in an electric field.
 * @tparam Particle The type of particle being simulated.
 */
template< typename Particle >
class ParticleModel
{
public:
    typedef Particle particle_t;
    typedef VPoint point_t;
    typedef VElectricField field_t;

    /**
     * @brief Constructor for ParticleModel.
     * @param start The starting position of the particle.
     */
    explicit ParticleModel(const point_t& start)
        :    m_last_pos(start)
        ,    m_particle()
    {
        m_track.push_back(start);
    }

    /**
     * @brief Move the particle based on the electric field.
     * @param v The electric field at the current position.
     * @return The new position of the particle after movement.
     */
    point_t operator()(field_t v)
    {
        const point_t delta = m_particle.move(m_last_pos, v);
        m_last_pos = (m_last_pos + delta);
        m_track.push_back(m_last_pos);

        return m_last_pos;
    }

    typedef std::vector< point_t > point_list_t;

    /**
     * @brief Getter for the particle's track.
     * @return A list of points representing the particle's track.
     */
    const point_list_t& get_track() { return m_track; }

private:
    point_list_t m_track;
    point_t m_last_pos;
    particle_t m_particle;
};

#endif // PARTICLEMODEL_H_INCLUDED
