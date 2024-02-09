import matplotlib.pyplot as plt
import numpy as np


def calculate_cross_sections(ion_energy):
    excitation_cross_section = 0.5 * np.exp(-ion_energy / 20.0)
    cx_cross_section = 0.3 * np.exp(-ion_energy / 15.0)
    iii_cross_section = 0.4 * np.exp(-ion_energy / 18.0)
    fusion_cross_section = 0.6 * np.exp(-ion_energy / 25.0)
    return excitation_cross_section, cx_cross_section, iii_cross_section, fusion_cross_section

ion_energies = np.linspace(0, 50, 100)

excitation_cross_sections, cx_cross_sections, iii_cross_sections, fusion_cross_sections = zip(
    *[calculate_cross_sections(energy) for energy in ion_energies]
)


plt.figure(figsize=(10, 6))
plt.plot(ion_energies, excitation_cross_sections, label='Excitation')
plt.plot(ion_energies, cx_cross_sections, label='Charge Exchange (CX)')
plt.plot(ion_energies, iii_cross_sections, label='Ion-Impact Ionization (III)')
plt.plot(ion_energies, fusion_cross_sections, label='Fusion')
plt.xlabel('Ion Energy')
plt.ylabel('Cross-section')
plt.title('Collision Processes Cross-sections')
plt.legend()
plt.grid(True)
plt.show()
