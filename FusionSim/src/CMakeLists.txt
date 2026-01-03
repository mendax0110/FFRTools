cmake_minimum_required(VERSION 3.10)
project(FusionSim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    message(STATUS "OpenMP found - enabling parallel execution")
else()
    message(WARNING "OpenMP not found - simulation will run single-threaded")
endif()

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
        FarnsworthFusorFieldModel.h
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
        PhysicalConstants.h
        Vector3dSimple.h
        ThermalDynamicsModel.h
)

add_executable(FusionSim ${SIM_SRC})

target_include_directories(FusionSim
        PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/../PotentialMap/src
        ${CMAKE_SOURCE_DIR}/../SFPS/src
)

find_package(Boost REQUIRED)
if(Boost_FOUND)
    target_include_directories(FusionSim PRIVATE ${Boost_INCLUDE_DIRS})
    target_link_libraries(FusionSim PRIVATE ${Boost_LIBRARIES})
endif()

target_link_libraries(FusionSim
        PRIVATE
        PotentialMap
        SFPS
)

if(OpenMP_CXX_FOUND)
    target_link_libraries(FusionSim PRIVATE OpenMP::OpenMP_CXX)
    target_compile_definitions(FusionSim PRIVATE USE_OPENMP)
endif()