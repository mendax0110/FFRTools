#ifndef WHILE_MOVING_H_INCLUDED
#define WHILE_MOVING_H_INCLUDED

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
