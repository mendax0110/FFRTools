import matplotlib.pyplot as plt
import numpy as np


def vacuum_potential_profile(r, Rc, Ra, Phi_c):
    return Phi_c * np.log((Rc**2 + r**2) / (Ra**2 + r**2))

Phi_c = -60.0  # Cathode potential
anode_radii = [0.05, 0.10, 0.15]  # Anode radii

r_values = np.linspace(0.01, max(anode_radii), 100)

plt.figure(figsize=(8, 6))
for Rc in anode_radii:
    potential_values = vacuum_potential_profile(r_values, Rc, 0.0, Phi_c)
    plt.plot(r_values, potential_values, label=f'Rc = {Rc}')

plt.xlabel('Distance to Cathode (r)')
plt.ylabel('Vacuum Potential (Φ)')
plt.title('Vacuum Potential Profile in Fusor (Cylindrical Shape)')
plt.ylim([-60, 0])
plt.legend()
plt.grid(True)
plt.show()
