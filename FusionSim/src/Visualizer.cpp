#include "Visualizer.h"
#include "IParticleModel.h"
#include "Vector3d.h"
#include <fstream>
#include <iostream>

void Visualizer::plot(const std::vector<std::unique_ptr<IParticleModel>>& particles, const std::string& filename)
{
    // export cvs
    std::ofstream out("fusion_particles.csv");
    out << "x,y,z,vx,vy,vz,mass,charge" << std::endl;
    for (const auto& p : particles)
    {
        auto pos = p->getPosition();
        auto vel = p->getVelocity();
        double mass = p->getMass().value;
        double charge = p->getCharge().value;
        out << pos.x << "," << pos.y << "," << pos.z << ","
            << vel.x << "," << vel.y << "," << vel.z << ","
            << mass << "," << charge << std::endl;
    }
    out.close();
    std::cout << "Daten als fusion_particles.csv gespeichert. Python-Skript kann daraus Bild erzeugen." << std::endl;
}
