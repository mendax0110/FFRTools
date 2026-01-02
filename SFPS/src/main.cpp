
#include "MappedFieldModel.h"
#include "ParticleModel.h"
#include "Proton.h"
#include "CollisionModel.h"
#include "PointMap.h"
#include "while_moving.h"
#include "Configuration.h"

#include <boost/random.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <fstream>
#include <ctime>

/// @brief Factory to generate random starting positions within a defined size. \class RandomPositionFactory
class RandomPositionFactory
{
public:
    /**
     * @brief Constructor for RandomPositionFactory.
     * @param s The size defining the bounds for random position generation.
     */
    explicit RandomPositionFactory(const VPoint& s)
        :   size(s)
        ,   seed( std::time(nullptr) )
        ,   gen_x(0, size.x.value)
        ,   gen_y(0, size.y.value)
        ,   gen_z(0, size.z.value)
        ,   v_x(seed, gen_x)
        ,   v_y(seed, gen_y)
        ,   v_z(seed, gen_z)
    {
    }

    /**
     * @brief Generate a random position within the defined size.
     * @return A VPoint representing the random position.
     */
    VPoint operator()()
    {
        return VPoint(v_x(), v_y(), v_z());
    }

    const VPoint size;

private:

    boost::mt19937 seed;
    boost::uniform_real<> gen_x;
    boost::uniform_real<> gen_y;
    boost::uniform_real<> gen_z;
    boost::variate_generator<boost::mt19937&, boost::uniform_real<> > v_x;
    boost::variate_generator<boost::mt19937&, boost::uniform_real<> > v_y;
    boost::variate_generator<boost::mt19937&, boost::uniform_real<> > v_z;
};

int main(int argc, char* argv[])
{
    Configuration::Data config = Configuration::get_configuration(argc, argv);

    MappedFieldModel< VElectricField, VPoint > field(config.map_size);

    VPoint centre_size(2.0, 2.0, 2.0);
    CollisionModel< VPoint > collision(config.map_size, centre_size);

    PointMap map(config.voltage);
    field.create_field_map(map);


    RandomPositionFactory start_positions(config.map_size);

    typedef ParticleModel< Proton > proton_model_t;

    for(unsigned int i = 0; i < config.particles; i++)
    {
        VPoint start(start_positions());
        proton_model_t particle(start);
        while_moving(start, particle, collision, field, config.max_length);

        for(auto pos = particle.get_track().begin(); pos != particle.get_track().end(); ++pos)
        {
            std::cout << *pos << ",";
        }

        std::cout << "(end)\n";

        for(auto pos = particle.get_track().begin(); pos != particle.get_track().end(); ++pos)
        {
            boost::filesystem::path p("positions.txt");
            std::ofstream ofs(p.string().c_str(), std::ios_base::app);
            ofs << *pos << "\n";
        }
    }

    return 0;
}
