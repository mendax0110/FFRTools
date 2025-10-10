# FusionSim

**FusionSim** ist eine modulare Simulationssoftware für Fusionsreaktoren, die auf physikalisch korrekten Modellen und modernen Software-Design-Patterns basiert. Sie integriert Komponenten aus den Projekten `PotentialMap` (Feldberechnung) und `SFPS` (Teilchenphysik) und unterstützt die Simulation von Deuterium-Deuterium- und Deuterium-Tritium-Fusion.

## Features
- **Modularer Aufbau:** Erweiterbare Architektur für Reaktorphysik, Teilchen, Felder, Reaktionen und Visualisierung
- **Physikalisch korrekt:** Nutzung realer Parameter und Modelle aus der Fusionsforschung
- **Integration:** Einbindung von PotentialMap (elektrische Felder) und SFPS (Teilchenbewegung)
- **CLI:** Komfortables Command-Line-Interface zur Steuerung und Konfiguration
- **Visualisierung:** Export der Simulationsergebnisse als CSV und automatische Bildgenerierung mit Python
- **Erweiterbar:** Neue Reaktionstypen, Feldmodelle und Visualisierungen können einfach ergänzt werden

## Installation
1. **Abhängigkeiten:**
   - C++17-kompatibler Compiler
   - CMake >= 3.10
   - Python3 mit matplotlib und numpy (für die Visualisierung)
2. **Build:**
   ```bash
   cd FusionSim
   mkdir build && cd build
   cmake ..
   make
   ```
3. **Visualisierungsskript installieren:**
   ```bash
   pip install matplotlib numpy
   ```


## Nutzung
```bash
./FusionSim --dd --tmax 1.0 --dt 0.01 --particles 100
./FusionSim --dt --tmax 0.5 --dt 0.005 --particles 50
```
- `--dd` : Deuterium-Deuterium-Fusion
- `--dt` : Deuterium-Tritium-Fusion
- `--tmax <t>` : Simulationszeit in Sekunden
- `--dt <dt>` : Zeitschritt in Sekunden
- `--particles <n>` : Anzahl der simulierten Teilchen

Nach der Simulation werden die Ergebnisse als `fusion_particles.csv` gespeichert. Mit dem Python-Skript `plot_results.py` kannst du die Daten flexibel auswerten und visualisieren:

### Beispiele für die Auswertung
```bash
# Energieverteilung plotten und Statistik exportieren
python3 plot_results.py --plot energy --export statistik.txt

# Geschwindigkeitsverteilung plotten
python3 plot_results.py --plot velocity --output velocity.png

# Teilchenpositionen als Scatterplot (Farbe = Energie)
python3 plot_results.py --plot position --output pos.png
```
Das Skript bietet:
- Flexible Plots (Energie, Geschwindigkeit, Position)
- Export von Statistiken als TXT
- Anpassbare Eingabe- und Ausgabedateien


## Erweiterung
- Neue Reaktionsmodelle können durch Implementierung von `IReactionModel` ergänzt werden.
- Neue Feldmodelle können durch Implementierung von `IFieldModel` integriert werden.
- Die Teilchenphysik ist über `IParticleModel` modular erweiterbar.

## Lizenz
Dieses Projekt steht unter MIT-Lizenz.

## Kontakt
Für Fragen und Beiträge: [GitHub Issues](https://github.com/mendax0110/FFRTools)
