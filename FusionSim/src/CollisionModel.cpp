#include "CollisionModel.h"
#include <cmath>

using namespace fusion;

void CollisionModel::elasticCollision(IParticleModel& p1, IParticleModel& p2)
{
    const double m1 = p1.getMass();
    const double m2 = p2.getMass();
    const Vector3d r1 = p1.getPosition();
    const Vector3d r2 = p2.getPosition();
    const Vector3d v1 = p1.getVelocity();
    const Vector3d v2 = p2.getVelocity();
    Vector3d n = (r2 - r1).normalized();
    if (n.norm() == 0)
    {
        n = Vector3d(1, 0, 0);
    }
    const double v1n = v1.dot(n);
    const double v2n = v2.dot(n);

    const double v1n_new = (v1n * (m1 - m2) + 2 * m2 * v2n) / (m1 + m2);
    const double v2n_new = (v2n * (m2 - m1) + 2 * m1 * v1n) / (m1 + m2);
    const Vector3d v1_new = v1 + (v1n_new - v1n) * n;
    const Vector3d v2_new = v2 + (v2n_new - v2n) * n;
    p1.setVelocity(v1_new);
    p2.setVelocity(v2_new);
}

void CollisionModel::inelasticCollision(IParticleModel& p1, IParticleModel& p2, const double energyLoss)
{
    const double m1 = p1.getMass();
    const double m2 = p2.getMass();
    const Vector3d v1 = p1.getVelocity();
    const Vector3d v2 = p2.getVelocity();
    const Vector3d v_cm = (m1 * v1 + m2 * v2) / (m1 + m2);
    const Vector3d v1_rel = v1 - v_cm;
    const Vector3d v2_rel = v2 - v_cm;
    const double total_kinetic_energy = 0.5 * m1 * v1_rel.squaredNorm() + 0.5 * m2 * v2_rel.squaredNorm();
    if (total_kinetic_energy <= energyLoss)
    {
        p1.setVelocity(v_cm);
        p2.setVelocity(v_cm);
        return;
    }

    const double reduction_factor = std::sqrt((total_kinetic_energy - energyLoss) / total_kinetic_energy);
    const Vector3d v1_new = v_cm + reduction_factor * v1_rel;
    const Vector3d v2_new = v_cm + reduction_factor * v2_rel;
    p1.setVelocity(v1_new);
    p2.setVelocity(v2_new);
}