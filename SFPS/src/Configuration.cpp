#include "Configuration.h"

#include <boost/program_options.hpp>

#include <iostream>
#include <exception>

Configuration::Data Configuration::get_configuration(int argc, char* argv[])
{
    Data config;

    try
    {
        boost::program_options::options_description desc("Allowed options");

        desc.add_options()
            ("help", "produce help message")
            ("file", boost::program_options::value< std::string >(), "output file location")
            ("particles", boost::program_options::value< unsigned int >(), "number of particles to simulate")
            ("length", boost::program_options::value<unsigned int >(), "maximum length of a particle simulation")
            ("x", boost::program_options::value< unsigned int >(), "size of map X axis")
            ("y", boost::program_options::value< unsigned int >(), "size of map Y axis")
            ("z", boost::program_options::value< unsigned int >(), "size of map Z axis")
            ("potential", boost::program_options::value< double >(), "grid potential");

        boost::program_options::variables_map m;
        boost::program_options::store(
            boost::program_options::parse_command_line(argc, argv, desc), m);
        boost::program_options::notify(m);

        if(m.count("help")
            || (m.count("particles") != 1)
            || (m.count("length") != 1)
            || (m.count("x") != 1)
            || (m.count("y") != 1)
            || (m.count("z") != 1))
        {
            std::cout << desc << std::endl;
            exit(0);
        }

        if(m.count("file") == 1)
        {
            config.out_file = m["file"].as< std::string >();
        }

        config.max_length = m["length"].as< unsigned int >();
        config.particles = m["particles"].as< unsigned int >();
        config.voltage = m["potential"].as< double >();
        config.map_size.x.value = m["x"].as< unsigned int >();
        config.map_size.y.value = m["y"].as< unsigned int >();
        config.map_size.z.value = m["z"].as< unsigned int >();
    }
    catch(std::exception& e)
    {
        std::cerr << "error with configuration: " << e.what() << std::endl;
        exit(1);
    }

    return config;
}
