#ifndef MAPPEDFIELDMODEL_H_INCLUDED
#define MAPPEDFIELDMODEL_H_INCLUDED

#include <cstddef>
#include <cassert>

template<
    typename Field,
    typename Point >
class MappedFieldModel
{
public:
    typedef Field field_t;
    typedef typename Field::scalar_t magnitude_t;
    typedef Point point_t;

    MappedFieldModel(const point_t& size, const unsigned int& scale = 1)
        :    m_scale(scale)
        ,    m_size(
                size.x.value / m_scale,
                size.y.value / m_scale,
                size.z.value / m_scale)
        ,    m_x_max(static_cast< size_t >(m_size.x.value))
        ,    m_y_max(static_cast< size_t >(m_size.y.value))
        ,    m_z_max(static_cast< size_t >(m_size.z.value))
    {
    }

    ~MappedFieldModel()
    {
        delete[] m_force_map;
    }

    template< typename Map >
    void create_field_map(Map& m)
    {
        size_t map_size = m_x_max * m_y_max * m_z_max;

        // Make sure that we're not just going to run out of
        // addressable space / try to keep it below 250MB.
        assert(map_size < ((250*1024*1024) / sizeof(field_t)));

        m_force_map = new field_t[map_size]; // Allocate in 1 go.

        for(size_t  x = 0; x < m_x_max; x++)
        {
            for(size_t y = 0; y < m_y_max; y++)
            {
                for(size_t z = 0; z < m_z_max; z++)
                {
                    point_t p(x, y, z);
                    field_t f = m(p);
                    m_force_map[get_map_position(x, y, z)] = f;

                }
            }
        }
    }

    field_t operator()(const point_t& point)
    {
        return m_force_map[get_map_position(point)];
    }

private:
    MappedFieldModel();
    MappedFieldModel(const MappedFieldModel&);

    const unsigned int m_scale;
    const point_t m_size;

    const size_t m_x_max;
    const size_t m_y_max;
    const size_t m_z_max;

    field_t* m_force_map;

    size_t get_map_position(const point_t& p)
    {
        point_t mapped_point = p / typename point_t::scalar_t(static_cast< double >(m_scale));
        assert(mapped_point.x.value < m_x_max);
        assert(mapped_point.y.value < m_y_max);
        assert(mapped_point.z.value < m_z_max);

        size_t pos = get_map_position(
            static_cast< size_t >(p.x.value),
            static_cast< size_t >(p.y.value),
            static_cast< size_t >(p.z.value));

        return pos;
    }

    size_t get_map_position(size_t x, size_t y, size_t z)
    {
        size_t px = x * m_y_max * m_z_max;
        size_t py = y * m_z_max;

        return px+py+z;
    }
};

#endif // MAPPEDFIELDMODEL_H_INCLUDED
