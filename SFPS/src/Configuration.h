#ifndef CONFIGURATION_H_INCLUDED
#define CONFIGURATION_H_INCLUDED

#include "Quantities.h"
#include <string>

/// @brief Configuration Utilities. \namespace Configuration
namespace Configuration
{
    /// @brief Configuration Data Structure. \struct Data
    struct Data
    {
        std::string out_file;
        VPoint map_size;
        unsigned int max_length;
        unsigned int particles;
        Potential voltage;
    };

    /**
     * @brief Get configuration data from command line arguments.
     * @param argc The number of command line arguments.
     * @param argv The array of command line argument strings.
     * @return A Data struct containing the configuration parameters.
     */
    Data get_configuration(int argc, char* argv[]);
}

#endif // CONFIGURATION_H_INCLUDED
