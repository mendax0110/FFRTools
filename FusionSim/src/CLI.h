#pragma once
#include <string>

/// @brief FusionSim - a simulator for FFR \namespace  fusion
namespace fusion
{
    /// @brief Command Line Interface for FusionSim. \class CLI
    class CLI
    {
    public:
        /**
         * @brief Run the command line interface.
         * @param argc Argument count.
         * @param argv Argument vector.
         * @return Exit code.
         */
        static int run(int argc, char* argv[]);
    };
}
