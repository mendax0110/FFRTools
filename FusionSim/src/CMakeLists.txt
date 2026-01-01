cmake_minimum_required(VERSION 3.5)
project(FusionSim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
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
        FieldModelPotentialMap.cpp
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

# Find boost
find_package(Boost REQUIRED)
if(Boost_FOUND)
    target_include_directories(FusionSim PRIVATE ${Boost_INCLUDE_DIRS})
    target_link_libraries(FusionSim PRIVATE ${Boost_LIBRARIES})
endif()

# Link libraries (linking to the library targets, not the executables)
target_link_libraries(FusionSim
        PRIVATE
        PotentialMap  # This is now the library target
        SFPS          # This is now the library target
)