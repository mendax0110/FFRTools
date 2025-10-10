data = np.loadtxt('fusion_results.csv')

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import argparse
import sys

def main():
	parser = argparse.ArgumentParser(description='FusionSim Datenanalyse und Visualisierung')
	parser.add_argument('--input', type=str, default='fusion_particles.csv', help='CSV-Datei mit Teilchendaten')
	parser.add_argument('--output', type=str, default='fusion_particles.png', help='Name der Ausgabedatei (Bild)')
	parser.add_argument('--plot', type=str, choices=['energy', 'velocity', 'position'], default='energy', help='Was soll geplottet werden?')
	parser.add_argument('--export', type=str, help='Exportiere Statistik als TXT')
	args = parser.parse_args()

	try:
		df = pd.read_csv(args.input)
	except Exception as e:
		print(f'Fehler beim Laden der Datei: {e}')
		sys.exit(1)

	# Energie berechnen (E = 0.5*m*v^2)
	df['speed'] = np.sqrt(df['vx']**2 + df['vy']**2 + df['vz']**2)
	df['energy_J'] = 0.5 * df['mass'] * df['speed']**2
	df['energy_MeV'] = df['energy_J'] / 1.60218e-13

	stats = {
		'Anzahl Teilchen': len(df),
		'Mittlere Energie (MeV)': df['energy_MeV'].mean(),
		'Maximale Energie (MeV)': df['energy_MeV'].max(),
		'Minimale Energie (MeV)': df['energy_MeV'].min(),
		'Mittlere Geschwindigkeit (m/s)': df['speed'].mean(),
	}

	if args.export:
		with open(args.export, 'w') as f:
			for k, v in stats.items():
				f.write(f'{k}: {v}\n')
		print(f'Statistik exportiert nach {args.export}')

	plt.figure(figsize=(8,6))
	if args.plot == 'energy':
		plt.hist(df['energy_MeV'], bins=50, color='blue', alpha=0.7)
		plt.xlabel('Energie [MeV]')
		plt.ylabel('Anzahl Teilchen')
		plt.title('Energieverteilung der Fusionsprodukte')
	elif args.plot == 'velocity':
		plt.hist(df['speed'], bins=50, color='green', alpha=0.7)
		plt.xlabel('Geschwindigkeit [m/s]')
		plt.ylabel('Anzahl Teilchen')
		plt.title('Geschwindigkeitsverteilung der Fusionsprodukte')
	elif args.plot == 'position':
		plt.scatter(df['x'], df['z'], c=df['energy_MeV'], cmap='plasma', s=10)
		plt.xlabel('x [m]')
		plt.ylabel('z [m]')
		plt.title('Positionen der Fusionsprodukte (Farbe: Energie)')
		plt.colorbar(label='Energie [MeV]')
	plt.tight_layout()
	plt.savefig(args.output)
	print(f'Bild gespeichert: {args.output}')
	print('Statistik:')
	for k, v in stats.items():
		print(f'{k}: {v}')

if __name__ == '__main__':
	main()
