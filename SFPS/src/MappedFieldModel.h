#ifndef MAPPEDFIELDMODEL_H_INCLUDED
#define MAPPEDFIELDMODEL_H_INCLUDED

#include <cstddef>
#include <cassert>

/**
 * @brief MappedFieldModel provides a way to map a field over a 3D grid for efficient retrieval.
 * @tparam Field The type of field being mapped.
 * @tparam Point The type representing a point in 3D space.
 */
template<
    typename Field,
    typename Point >
class MappedFieldModel
{
public:
    typedef Field field_t;
    typedef typename Field::scalar_t magnitude_t;
    typedef Point point_t;

    /**
     * @brief Constructor for MappedFieldModel.
     * @param size The size of the 3D grid.
     * @param scale The scaling factor for the grid resolution.
     */
    explicit MappedFieldModel(const point_t& size, const unsigned int& scale = 1)
        : m_scale(scale)
          , m_size(
              size.x.value / m_scale,
              size.y.value / m_scale,
              size.z.value / m_scale)
          , m_x_max(static_cast<size_t>(m_size.x.value))
          , m_y_max(static_cast<size_t>(m_size.y.value))
          , m_z_max(static_cast<size_t>(m_size.z.value)), m_force_map(nullptr)
    {
    }

    /**
     * @brief Destructor for MappedFieldModel.
     */
    ~MappedFieldModel()
    {
        delete[] m_force_map;
    }

    /**
     * @brief Create the field map from a given field model.
     * @tparam Map The type of the field model.
     * @param m The field model to map.
     */
    template< typename Map >
    void create_field_map(Map& m)
    {
        const size_t map_size = m_x_max * m_y_max * m_z_max;

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

    /**
     * @brief Overloaded function call operator to get field at a point.
     * @param point The point in space.
     * @return The field at the given point.
     */
    field_t operator()(const point_t& point)
    {
        return m_force_map[get_map_position(point)];
    }

private:
    /**
     * @brief Default constructor.
     */
    MappedFieldModel();

    /**
     * @brief Copy constructor.
     * @param MappedFieldModel The MappedFieldModel to copy from.
     */
    MappedFieldModel(const MappedFieldModel&);

    const unsigned int m_scale;
    const point_t m_size;

    const size_t m_x_max;
    const size_t m_y_max;
    const size_t m_z_max;

    field_t* m_force_map;

    /**
     * @brief Get the map position for a given point.
     * @param p The point in space.
     * @return The index in the field map corresponding to the point.
     */
    size_t get_map_position(const point_t& p)
    {
        point_t mapped_point = p / typename point_t::scalar_t(static_cast< double >(m_scale));
        assert(mapped_point.x.value < m_x_max);
        assert(mapped_point.y.value < m_y_max);
        assert(mapped_point.z.value < m_z_max);

        const size_t pos = get_map_position(
            static_cast< size_t >(p.x.value),
            static_cast< size_t >(p.y.value),
            static_cast< size_t >(p.z.value));

        return pos;
    }

    /**
     * @brief Get the map position for given x, y, z coordinates.
     * @param x The x-coordinate.
     * @param y The y-coordinate.
     * @param z The z-coordinate.
     * @return The index in the field map corresponding to the coordinates.
     */
    size_t get_map_position(const size_t x, const size_t y, const size_t z) const
    {
        const size_t px = x * m_y_max * m_z_max;
        const size_t py = y * m_z_max;

        return px+py+z;
    }
};

#endif // MAPPEDFIELDMODEL_H_INCLUDED
