import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import argparse
import sys
import os

def plot_data(df, mode, output=None):
    plt.figure(figsize=(8,6))
    if mode == 'energy':
        plt.hist(df['energy_MeV'].dropna(), bins=50, color='blue', alpha=0.7)
        plt.xlabel('Energie [MeV]')
        plt.ylabel('Anzahl Teilchen')
        plt.title('Energieverteilung')
    elif mode == 'velocity':
        plt.hist(df['speed'].dropna(), bins=50, color='green', alpha=0.7)
        plt.xlabel('Geschwindigkeit [m/s]')
        plt.ylabel('Anzahl Teilchen')
        plt.title('Geschwindigkeitsverteilung')
    elif mode == 'position':
        plt.scatter(df['x'], df['z'], c=df['energy_MeV'], cmap='plasma', s=10)
        plt.xlabel('x [m]')
        plt.ylabel('z [m]')
        plt.title('Positionen (Farbe = Energie)')
        plt.colorbar(label='Energie [MeV]')
    plt.tight_layout()
    if output:
        plt.savefig(output)
        print(f"Plot gespeichert unter: {output}")
    else:
        plt.show()

def load_data(input_file):
    if not os.path.exists(input_file):
        print(f"Datei nicht gefunden: {input_file}")
        sys.exit(1)
    df = pd.read_csv(input_file)
    df['speed'] = np.sqrt(df['vx']**2 + df['vy']**2 + df['vz']**2)
    df['energy_J'] = 0.5 * df['mass'] * df['speed']**2
    df['energy_MeV'] = df['energy_J'] / 1.60218e-13
    return df

def interactive_mode(df):
    print("\nInteraktiver Analysemodus")
    print("Befehle:")
    print("  plot energy        – Energieverteilung anzeigen")
    print("  plot velocity      – Geschwindigkeitsverteilung anzeigen")
    print("  plot position      – Positionsplot anzeigen")
    print("  stats              – Statistik ausgeben")
    print("  filter energy <min> <max> – Datenbereich begrenzen")
    print("  exit               – Programm beenden\n")

    while True:
        cmd = input("Befehl > ").strip().split()
        if not cmd:
            continue
        if cmd[0] == "exit":
            print("Auf Wiedersehen!")
            break
        elif cmd[0] == "plot" and len(cmd) > 1:
            plot_data(df, cmd[1])
        elif cmd[0] == "stats":
            print(df[['energy_MeV', 'speed']].describe())
        elif cmd[0] == "filter" and len(cmd) == 4 and cmd[1] == "energy":
            try:
                e_min, e_max = float(cmd[2]), float(cmd[3])
                df = df[(df['energy_MeV'] >= e_min) & (df['energy_MeV'] <= e_max)]
                print(f"Filter angewendet: {e_min}–{e_max} MeV, neue Größe: {len(df)}")
            except ValueError:
                print("Ungültige Zahlenwerte")
        else:
            print("Unbekannter Befehl")

def main():
    parser = argparse.ArgumentParser(description='FusionSim Datenanalyse und Visualisierung')
    parser.add_argument('--input', type=str, default='fusion_particles.csv', help='CSV-Datei')
    parser.add_argument('--interactive', action='store_true', help='Interaktiven Modus starten')
    args = parser.parse_args()

    df = load_data(args.input)

    if args.interactive:
        interactive_mode(df)
    else:
        plot_data(df, 'energy', 'fusion_particles.png')

if __name__ == '__main__':
    main()
