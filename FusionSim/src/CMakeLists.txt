cmake_minimum_required(VERSION 3.17)
project(FusionSim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SIM_SRC
    main.cpp
    CLI.cpp
    CLI.h
    Reactor.cpp
    Reactor.h
    FusionReaction.cpp
    FusionReaction.h
    Visualizer.cpp
    Visualizer.h
    ParticleModelSFPS.h
    FieldModelPotentialMap.h
    MagneticFieldUniform.h
    CollisionModel.cpp
    CollisionModel.h
    SimulationManager.cpp
    SimulationManager.h
    ReactionModelDD.h
    ReactionModelDT.h
    IFieldModel.h
    IMagneticFieldModel.h
    IParticleModel.h
    IReactionModel.h
)

add_executable(FusionSim ${SIM_SRC})

target_include_directories(FusionSim
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/../PotentialMap/src
        ${CMAKE_SOURCE_DIR}/../SFPS/src
)

target_link_libraries(FusionSim
    PRIVATE
        PotentialMapLib
        SFPSLib
)

if(TARGET Boost::program_options)
    target_link_libraries(FusionSim PRIVATE Boost::program_options)
endif()
