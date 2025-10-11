#include "CollisionModel.h"
#include <cmath>

void CollisionModel::elasticCollision(IParticleModel& p1, IParticleModel& p2)
{
    // 3d-ellastische Kollision für Impuls- und Energieerhaltung
    double m1 = p1.getMass().value;
    double m2 = p2.getMass().value;
    auto r1 = p1.getPosition();
    auto r2 = p2.getPosition();
    auto v1 = p1.getVelocity();
    auto v2 = p2.getVelocity();
    auto n = Vector3d<Velocity>::fromOther((r2 - r1).normalized());
    if (n.norm() == 0.0) n = Vector3d<Velocity>(Velocity(1), Velocity(0), Velocity(0)); // Fallback
    double v1n = v1.dot(n).value;
    double v2n = v2.dot(n).value;
    
    double v1n_new = (v1n*(m1-m2) + 2*m2*v2n)/(m1+m2);
    double v2n_new = (v2n*(m2-m1) + 2*m1*v1n)/(m1+m2);
    auto v1_new = v1 + (v1n_new - v1n)*n;
    auto v2_new = v2 + (v2n_new - v2n)*n;
    p1.setVelocity(v1_new);
    p2.setVelocity(v2_new);
}

void CollisionModel::inelasticCollision(IParticleModel& p1, IParticleModel& p2, double energyLoss)
{
    double m1 = p1.getMass().value;
    double m2 = p2.getMass().value;
    auto v1 = p1.getVelocity();
    auto v2 = p2.getVelocity();
    auto v_cm = (m1 * v1 + m2 * v2) / (m1 + m2); // gesch. schwerpunkt
    auto v1_rel = v1 - v_cm;
    auto v2_rel = v2 - v_cm;
    double total_kinetic_energy = 0.5 * m1 * v1_rel.squaredNorm().value + 0.5 * m2 * v2_rel.squaredNorm().value;
    if (total_kinetic_energy <= energyLoss)
    {
        p1.setVelocity(v_cm);
        p2.setVelocity(v_cm);
        return;
    }

    double reduction_factor = sqrt((total_kinetic_energy - energyLoss) / total_kinetic_energy);
    Vector3d v1_new = v_cm + reduction_factor * v1_rel;
    Vector3d v2_new = v_cm + reduction_factor * v2_rel;
    p1.setVelocity(v1_new);
    p2.setVelocity(v2_new);
}