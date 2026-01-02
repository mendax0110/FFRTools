#ifndef WHILE_MOVING_H_INCLUDED
#define WHILE_MOVING_H_INCLUDED

/**
 * @brief Simulate particle movement while checking for collisions.
 *
 * @tparam ParticleModel The model representing the particle's movement.
 * @tparam CollisionModel The model representing collision detection.
 * @tparam FieldModel The model representing the field affecting the particle.
 * @param start The starting position of the particle.
 * @param particle The particle model instance.
 * @param collision The collision model instance.
 * @param field The field model instance.
 * @param max_length The maximum number of steps to simulate.
 */
template<
    typename ParticleModel,
    typename CollisionModel,
    typename FieldModel >
void while_moving(
    typename ParticleModel::point_t start,
    ParticleModel& particle,
    CollisionModel& collision,
    FieldModel& field,
    const unsigned int max_length)
{
    typename ParticleModel::point_t cur_pos = start;

    for(unsigned int num = 0; (num < max_length) && !collision(cur_pos); num++)
    {
        typename FieldModel::field_t v = field(cur_pos);

        cur_pos = particle(v);
    }
}

#endif // WHILE_MOVING_H_INCLUDED
