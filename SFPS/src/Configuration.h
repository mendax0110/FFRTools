#ifndef CONFIGURATION_H_INCLUDED
#define CONFIGURATION_H_INCLUDED

#include "Quantities.h"
#include "Vector3d.h"
#include <string>

namespace Configuration
{
    struct Data
    {
        std::string out_file;
        VPoint map_size;
        unsigned int max_length;
        unsigned int particles;
        Potential voltage;
    };

    Data get_configuration(int argc, char* argv[]);
}

#endif // CONFIGURATION_H_INCLUDED
