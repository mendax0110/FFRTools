#include "Visualizer.h"
#include <fstream>
#include <iostream>
#include <memory>
#include "IParticleModel.h"

using namespace fusion;

void Visualizer::plot(const std::vector<std::unique_ptr<IParticleModel>>& particles, const std::string& filename)
{
    std::ofstream out("fusion_particles.csv");
    out << "x,y,z,vx,vy,vz,mass,charge" << std::endl;
    for (const auto& p : particles)
    {
        const Vector3d pos = p->getPosition();
        const Vector3d vel = p->getVelocity();
        const double mass = p->getMass();
        const double charge = p->getCharge();
        out << pos.x << "," << pos.y << "," << pos.z << ","
            << vel.x << "," << vel.y << "," << vel.z << ","
            << mass << "," << charge << "\n";
    }
    out.close();
    std::cout << "Daten als fusion_particles.csv gespeichert. " << "Python-Skript kann daraus Bild erzeugen." << std::endl;
}
