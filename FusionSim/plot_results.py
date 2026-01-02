#!/usr/bin/env python3

import matplotlib.pyplot as plt
from matplotlib.widgets import Slider, Button, RadioButtons, CheckButtons
import numpy as np
import pandas as pd
import argparse
import sys
from pathlib import Path

plt.style.use('seaborn-v0_8-whitegrid')
plt.rcParams.update({
	'font.size': 11,
	'axes.labelsize': 12,
	'axes.titlesize': 14,
	'legend.fontsize': 10,
	'figure.titlesize': 16,
	'figure.dpi': 100,
})

PHYSICAL_CONSTANTS = {
	'eV_to_J': 1.602176634e-19,
	'keV_to_J': 1.602176634e-16,
	'MeV_to_J': 1.602176634e-13,
	'mass_deuterium': 3.343583719e-27,
	'mass_neutron': 1.67492749804e-27,
	'mass_proton': 1.67262192369e-27,
	'mass_he3': 5.008234e-27,
	'mass_he4': 6.646476e-27,
	'mass_tritium': 5.00827e-27,
}

PARTICLE_TYPES = {
	'deuterium': {'mass_range': (3.3e-27, 3.4e-27), 'charge': 1.6e-19, 'label': 'Deuterium', 'color': '#1f77b4'},
	'tritium': {'mass_range': (5.0e-27, 5.1e-27), 'charge': 1.6e-19, 'label': 'Tritium', 'color': '#2ca02c'},
	'neutron': {'mass_range': (1.67e-27, 1.68e-27), 'charge': 0.0, 'label': 'Neutron', 'color': '#7f7f7f'},
	'proton': {'mass_range': (1.67e-27, 1.68e-27), 'charge': 1.6e-19, 'label': 'Proton', 'color': '#d62728'},
	'he3': {'mass_range': (5.0e-27, 5.1e-27), 'charge': 3.2e-19, 'label': 'He-3', 'color': '#ff7f0e'},
	'he4': {'mass_range': (6.6e-27, 6.7e-27), 'charge': 3.2e-19, 'label': 'He-4', 'color': '#9467bd'},
}


def classify_particle(mass, charge):
	"""
	Classify particle type based on mass and charge.
	:param mass: The mass of the particle.
	:param charge: The charge of the particle.
	:return:
	"""
	for ptype, props in PARTICLE_TYPES.items():
		mass_min, mass_max = props['mass_range']
		if mass_min <= mass <= mass_max:
			if abs(charge - props['charge']) < 1e-20:
				return ptype
	return 'unknown'


def load_data(filepath):
	"""
	Load and preprocess simulation data.
	:param filepath: The path to the CSV file.
	:return: A pandas DataFrame with computed properties.
	"""
	df = pd.read_csv(filepath)

	df['speed'] = np.sqrt(df['vx']**2 + df['vy']**2 + df['vz']**2)
	df['radius'] = np.sqrt(df['x']**2 + df['y']**2 + df['z']**2)
	df['energy_J'] = 0.5 * df['mass'] * df['speed']**2
	df['energy_keV'] = df['energy_J'] / PHYSICAL_CONSTANTS['keV_to_J']
	df['energy_MeV'] = df['energy_J'] / PHYSICAL_CONSTANTS['MeV_to_J']

	df['vr'] = (df['x'] * df['vx'] + df['y'] * df['vy'] + df['z'] * df['vz']) / (df['radius'] + 1e-20)

	df['particle_type'] = df.apply(lambda row: classify_particle(row['mass'], row['charge']), axis=1)

	return df


def compute_statistics(df):
	"""
	Compute comprehensive statistics.
	:param df: The dataframe with particle data.
	:return: A dictionary with statistics.
	"""
	stats = {
		'Total particles': len(df),
		'Mean energy (keV)': df['energy_keV'].mean(),
		'Max energy (keV)': df['energy_keV'].max(),
		'Min energy (keV)': df['energy_keV'].min(),
		'Std energy (keV)': df['energy_keV'].std(),
		'Mean speed (km/s)': df['speed'].mean() / 1000,
		'Max speed (km/s)': df['speed'].max() / 1000,
		'Mean radius (cm)': df['radius'].mean() * 100,
		'Max radius (cm)': df['radius'].max() * 100,
	}

	for ptype in df['particle_type'].unique():
		mask = df['particle_type'] == ptype
		count = mask.sum()
		if count > 0:
			stats[f'{ptype.capitalize()} count'] = count
			stats[f'{ptype.capitalize()} mean energy (keV)'] = df.loc[mask, 'energy_keV'].mean()

	return stats


def plot_comprehensive(df):
	"""
	Generate comprehensive multi-panel analysis plot. Returns figure for interactive display.
	:param df: The dataframe with particle data.
	:return: A matplotlib figure.
	"""
	fig = plt.figure(figsize=(16, 12))
	fig.suptitle('Farnsworth Fusor Simulation Analysis', fontsize=16, fontweight='bold')

	ax1 = fig.add_subplot(2, 3, 1)
	for ptype in df['particle_type'].unique():
		mask = df['particle_type'] == ptype
		if mask.sum() > 0:
			color = PARTICLE_TYPES.get(ptype, {'color': '#333333'})['color']
			label = PARTICLE_TYPES.get(ptype, {'label': ptype.capitalize()})['label']
			ax1.hist(df.loc[mask, 'energy_keV'], bins=50, alpha=0.7,
					 label=f'{label} (n={mask.sum()})', color=color, edgecolor='black', linewidth=0.5)
	ax1.set_xlabel('Energy [keV]')
	ax1.set_ylabel('Count')
	ax1.set_title('Energy Distribution by Particle Type')
	ax1.legend(loc='upper right', fontsize=8)
	ax1.set_yscale('log', nonpositive='clip')

	ax2 = fig.add_subplot(2, 3, 2)
	for ptype in df['particle_type'].unique():
		mask = df['particle_type'] == ptype
		if mask.sum() > 0:
			color = PARTICLE_TYPES.get(ptype, {'color': '#333333'})['color']
			label = PARTICLE_TYPES.get(ptype, {'label': ptype.capitalize()})['label']
			ax2.scatter(df.loc[mask, 'radius'] * 100, df.loc[mask, 'energy_keV'],
						alpha=0.5, s=10, c=color, label=label)
	ax2.set_xlabel('Radial Position [cm]')
	ax2.set_ylabel('Energy [keV]')
	ax2.set_title('Energy vs Radial Position')
	ax2.axvline(x=1.6, color='red', linestyle='--', linewidth=1.5, label='Inner Grid (1.6 cm)')
	ax2.axvline(x=8.0, color='blue', linestyle='--', linewidth=1.5, label='Outer Grid (8.0 cm)')
	ax2.legend(loc='upper right', fontsize=8)

	ax3 = fig.add_subplot(2, 3, 3, projection='polar')
	theta = np.arctan2(df['y'], df['x'])
	r = df['radius'] * 100
	colors = df['energy_keV']
	scatter = ax3.scatter(theta, r, c=colors, cmap='plasma', s=5, alpha=0.7)
	ax3.set_title('Particle Positions (Polar View)')
	plt.colorbar(scatter, ax=ax3, label='Energy [keV]', shrink=0.8)

	ax4 = fig.add_subplot(2, 3, 4)
	ax4.hist(df['vr'] / 1000, bins=50, color='steelblue', alpha=0.8, edgecolor='black', linewidth=0.5)
	ax4.axvline(x=0, color='red', linestyle='-', linewidth=2)
	ax4.set_xlabel('Radial Velocity [km/s]')
	ax4.set_ylabel('Count')
	ax4.set_title('Radial Velocity Distribution')
	inward = (df['vr'] < 0).sum()
	outward = (df['vr'] >= 0).sum()
	ax4.text(0.02, 0.98, f'Inward: {inward}\nOutward: {outward}',
			 transform=ax4.transAxes, verticalalignment='top', fontsize=9,
			 bbox=dict(boxstyle='round', facecolor='wheat', alpha=0.5))

	ax5 = fig.add_subplot(2, 3, 5)
	r_bins = np.linspace(0, df['radius'].max() * 100, 30)
	r_centers = 0.5 * (r_bins[:-1] + r_bins[1:])
	mean_energy = []
	for i in range(len(r_bins) - 1):
		mask = (df['radius'] * 100 >= r_bins[i]) & (df['radius'] * 100 < r_bins[i+1])
		if mask.sum() > 0:
			mean_energy.append(df.loc[mask, 'energy_keV'].mean())
		else:
			mean_energy.append(np.nan)
	ax5.plot(r_centers, mean_energy, 'o-', color='darkgreen', markersize=4)
	ax5.set_xlabel('Radial Position [cm]')
	ax5.set_ylabel('Mean Energy [keV]')
	ax5.set_title('Mean Energy vs Radius')
	ax5.axvline(x=1.6, color='red', linestyle='--', linewidth=1.5, alpha=0.7)
	ax5.axvline(x=8.0, color='blue', linestyle='--', linewidth=1.5, alpha=0.7)

	ax6 = fig.add_subplot(2, 3, 6)
	particle_counts = df['particle_type'].value_counts()
	colors = [PARTICLE_TYPES.get(p, {'color': '#333333'})['color'] for p in particle_counts.index]
	labels = [PARTICLE_TYPES.get(p, {'label': p.capitalize()})['label'] for p in particle_counts.index]
	_, _, _ = ax6.pie(particle_counts.values, labels=labels, colors=colors,
									   autopct='%1.1f%%', startangle=90)
	ax6.set_title('Particle Type Distribution')

	plt.tight_layout(rect=[0, 0.03, 1, 0.95])
	return fig


def plot_phase_space(df):
	"""
	Generate phase space plots. Returns figure for interactive display.
	:param df: The dataframe with particle data.
	:return: A matplotlib figure.
	"""
	fig, axes = plt.subplots(1, 3, figsize=(15, 5))
	fig.suptitle('Phase Space Analysis', fontsize=14, fontweight='bold')

	for ax, (pos, vel) in zip(axes, [('x', 'vx'), ('y', 'vy'), ('z', 'vz')]):
		scatter = ax.scatter(df[pos] * 100, df[vel] / 1000,
							 c=df['energy_keV'], cmap='viridis', s=5, alpha=0.6)
		ax.set_xlabel(f'{pos} [cm]')
		ax.set_ylabel(f'v{pos[-1]} [km/s]')
		ax.set_title(f'{pos.upper()}-V{pos[-1].upper()} Phase Space')
		plt.colorbar(scatter, ax=ax, label='Energy [keV]')

	plt.tight_layout()
	return fig


def plot_3d_positions(df):
	"""
	Generate 3D position scatter plot. Returns figure for interactive display.
	:param df: The dataframe with particle data.
	:return: A matplotlib figure.
	"""
	fig = plt.figure(figsize=(10, 8))
	ax = fig.add_subplot(111, projection='3d')

	for ptype in df['particle_type'].unique():
		mask = df['particle_type'] == ptype
		if mask.sum() > 0:
			color = PARTICLE_TYPES.get(ptype, {'color': '#333333'})['color']
			label = PARTICLE_TYPES.get(ptype, {'label': ptype.capitalize()})['label']
			ax.scatter(df.loc[mask, 'x'] * 100,
					   df.loc[mask, 'y'] * 100,
					   df.loc[mask, 'z'] * 100,
					   c=color, s=10, alpha=0.6, label=label)

	u = np.linspace(0, 2 * np.pi, 30)
	v = np.linspace(0, np.pi, 20)
	for radius, color, alpha in [(1.6, 'red', 0.1), (8.0, 'blue', 0.05)]:
		x = radius * np.outer(np.cos(u), np.sin(v))
		y = radius * np.outer(np.sin(u), np.sin(v))
		z = radius * np.outer(np.ones(np.size(u)), np.cos(v))
		ax.plot_surface(x, y, z, color=color, alpha=alpha)

	ax.set_xlabel('X [cm]')
	ax.set_ylabel('Y [cm]')
	ax.set_zlabel('Z [cm]')
	ax.set_title('3D Particle Distribution')
	ax.legend(loc='upper left')

	plt.tight_layout()
	return fig


def plot_energy_filter(df, min_keV=0.0, max_keV=None):
	"""
	Interactive energy-filtered position plot.
	:param df: The dataframe with particle data.
	:param min_keV: The minimum energy in keV.
	:param max_keV: The maximum energy in keV.
	:return: A matplotlib figure.
	"""
	if max_keV is None:
		max_keV = df['energy_keV'].max()

	filtered = df[(df['energy_keV'] >= min_keV) & (df['energy_keV'] <= max_keV)]

	fig, ax = plt.subplots(figsize=(10, 8))

	r = np.sqrt(filtered['x']**2 + filtered['y']**2 + filtered['z']**2) * 100
	scatter = ax.scatter(r, filtered['energy_keV'], c=filtered['speed'] / 1000, cmap='plasma', s=20, alpha=0.7)

	ax.set_xlabel('Radius [cm]')
	ax.set_ylabel('Energy [keV]')
	ax.set_title(f'Energy vs Radius ({len(filtered)} particles in range)')
	ax.axvline(1.6, color='red', linestyle='--', label='Inner grid (1.6 cm)', alpha=0.7)
	ax.axvline(8.0, color='blue', linestyle='--', label='Outer grid (8 cm)', alpha=0.7)
	plt.colorbar(scatter, label='Speed [km/s]')
	ax.legend()
	plt.tight_layout()
	return fig


def export_statistics(stats, filepath):
	"""
	Export statistics to file.
	:param stats: The statistics dictionary.
	:param filepath: The output file path.
	"""
	with open(filepath, 'w') as f:
		f.write('=' * 60 + '\n')
		f.write('FUSIONSIM SIMULATION STATISTICS\n')
		f.write('=' * 60 + '\n\n')

		f.write('GENERAL STATISTICS\n')
		f.write('-' * 40 + '\n')
		for key, value in stats.items():
			if isinstance(value, float):
				f.write(f'{key}: {value:.4e}\n')
			else:
				f.write(f'{key}: {value}\n')

		f.write('\n' + '=' * 60 + '\n')
	print(f'Statistics exported: {filepath}')


class InteractiveExplorer:
	"""
	Interactive data exploration with matplotlib widgets.
	"""

	def __init__(self, df, stats):
		self.df = df
		self.stats = stats
		self.current_plot = 'comprehensive'
		self.fig = None
		self.energy_min = 0.0
		self.energy_max = df['energy_keV'].max()

	def show_menu(self):
		"""
		Display interactive menu in terminal.
		"""
		print('\n' + '=' * 60)
		print('  FUSIONSIM INTERACTIVE EXPLORER')
		print('=' * 60)
		print(f'  Loaded: {len(self.df)} particles')
		print(f'  Energy range: {self.df["energy_keV"].min():.2f} - {self.df["energy_keV"].max():.2f} keV')
		print('=' * 60)
		print('\nAvailable plots:')
		print('  [1] Comprehensive Analysis (4 subplots)')
		print('  [2] Phase Space (x-vx, y-vy, z-vz)')
		print('  [3] 3D Particle Distribution')
		print('  [4] Energy vs Radius (interactive filter)')
		print('  [5] Show Statistics')
		print('  [6] Energy Filter Settings')
		print('  [s] Save current plot to file')
		print('  [q] Quit')
		print('-' * 60)

	def run(self):
		"""
		Main interactive loop.
		"""
		plt.ion()  # interactive mode...

		while True:
			self.show_menu()
			choice = input('\nSelect option: ').strip().lower()

			if choice == '1':
				self.show_comprehensive()
			elif choice == '2':
				self.show_phase_space()
			elif choice == '3':
				self.show_3d()
			elif choice == '4':
				self.show_energy_filter()
			elif choice == '5':
				self.show_statistics()
			elif choice == '6':
				self.energy_filter_settings()
			elif choice == 's':
				self.save_current()
			elif choice == 'q':
				print('\nGoodbye!')
				plt.close('all')
				break
			else:
				print(f'Unknown option: {choice}')

	def show_comprehensive(self):
		"""
		Display comprehensive analysis.
		"""
		print('\nGenerating comprehensive analysis...')
		if self.fig:
			plt.close(self.fig)
		self.fig = plot_comprehensive(self.df)
		self.current_plot = 'comprehensive'
		plt.show(block=False)
		plt.pause(0.1)
		print('Plot displayed. Use zoom/pan tools to explore.')
		input('Press Enter to continue...')

	def show_phase_space(self):
		"""
		Display phase space analysis.
		"""
		print('\nGenerating phase space analysis...')
		if self.fig:
			plt.close(self.fig)
		self.fig = plot_phase_space(self.df)
		self.current_plot = 'phase_space'
		plt.show(block=False)
		plt.pause(0.1)
		print('Plot displayed. Use zoom/pan tools to explore.')
		input('Press Enter to continue...')

	def show_3d(self):
		"""
		Display 3D visualization.
		"""
		print('\nGenerating 3D visualization...')
		print('(Use mouse to rotate and explore the 3D plot)')
		if self.fig:
			plt.close(self.fig)
		self.fig = plot_3d_positions(self.df)
		self.current_plot = '3d'
		plt.show(block=False)
		plt.pause(0.1)
		print('Plot displayed. Click and drag to rotate.')
		input('Press Enter to continue...')

	def show_energy_filter(self):
		"""
		Display energy-filtered plot.
		"""
		print(f'\nGenerating energy-filtered plot ({self.energy_min:.2f} - {self.energy_max:.2f} keV)...')
		if self.fig:
			plt.close(self.fig)
		self.fig = plot_energy_filter(self.df, self.energy_min, self.energy_max)
		self.current_plot = 'energy_filter'
		plt.show(block=False)
		plt.pause(0.1)
		print('Plot displayed. Use zoom/pan tools to explore.')
		input('Press Enter to continue...')

	def show_statistics(self):
		"""
		Display statistics in terminal.
		"""
		print('\n' + '=' * 50)
		print('SIMULATION STATISTICS')
		print('=' * 50)
		for key, value in self.stats.items():
			if isinstance(value, float):
				print(f'  {key}: {value:.4e}')
			else:
				print(f'  {key}: {value}')
		print('=' * 50)
		input('\nPress Enter to continue...')

	def energy_filter_settings(self):
		"""
		Configure energy filter.
		"""
		max_energy = self.df['energy_keV'].max()
		print(f'\nCurrent range: {self.energy_min:.2f} - {self.energy_max:.2f} keV')
		print(f'Data range: 0 - {max_energy:.2f} keV')

		try:
			min_input = input(f'Min energy (keV) [{self.energy_min:.2f}]: ').strip()
			if min_input:
				self.energy_min = float(min_input)

			max_input = input(f'Max energy (keV) [{self.energy_max:.2f}]: ').strip()
			if max_input:
				self.energy_max = float(max_input)

			# Clamp values
			self.energy_min = max(0, min(self.energy_min, max_energy))
			self.energy_max = max(self.energy_min, min(self.energy_max, max_energy))

			print(f'New range: {self.energy_min:.2f} - {self.energy_max:.2f} keV')
		except ValueError:
			print('Invalid input, keeping current values.')

	def save_current(self):
		"""
		Save current plot to file.
		"""
		if self.fig is None:
			print('No plot to save. Generate a plot first.')
			return

		filename = input('Enter filename (without extension): ').strip()
		if not filename:
			filename = f'fusion_{self.current_plot}'

		output_path = f'{filename}.png'
		self.fig.savefig(output_path, dpi=150, bbox_inches='tight')
		print(f'Saved: {output_path}')


def main():
	"""
	Main loop, select interactive or not...
	"""
	parser = argparse.ArgumentParser(
		description='FusionSim Data Analysis and Visualization',
		formatter_class=argparse.RawDescriptionHelpFormatter,
		epilog='''
		Examples:
		  python plot_results.py --input fusion_particles.csv
		  python plot_results.py --input fusion_particles.csv --save
		  python plot_results.py --input fusion_particles.csv --save --plot 3d
        ''')
	parser.add_argument('--input', '-i', type=str, default='fusion_particles.csv',
						help='Input CSV file with particle data')
	parser.add_argument('--output', '-o', type=str, default='fusion_analysis',
						help='Output base filename (without extension)')
	parser.add_argument('--plot', '-p', type=str, nargs='+',
						choices=['comprehensive', 'phase', '3d', 'energy', 'all'],
						default=['comprehensive'],
						help='Plot types to generate (for --save mode)')
	parser.add_argument('--save', '-s', action='store_true',
						help='Save plots to files instead of interactive mode')
	parser.add_argument('--export', '-e', type=str,
						help='Export statistics to text file')
	parser.add_argument('--all', '-a', action='store_true',
						help='Generate all plot types (for --save mode)')
	args = parser.parse_args()

	input_path = Path(args.input)
	if not input_path.exists():
		print(f'Error: File not found: {args.input}')
		print('Usage: python plot_results.py --input <csv_file>')
		sys.exit(1)

	print(f'Loading data from: {args.input}')
	df = load_data(args.input)
	print(f'Loaded {len(df)} particles')

	stats = compute_statistics(df)

	if args.save:
		# Batch mode - save static images
		print('\n' + '=' * 50)
		print('SIMULATION STATISTICS')
		print('=' * 50)
		for key, value in stats.items():
			if isinstance(value, float):
				print(f'{key}: {value:.4e}')
			else:
				print(f'{key}: {value}')
		print('=' * 50 + '\n')

		plot_types = args.plot
		if args.all or 'all' in plot_types:
			plot_types = ['comprehensive', 'phase', '3d', 'energy']

		for plot_type in plot_types:
			if plot_type == 'comprehensive':
				fig = plot_comprehensive(df)
				output_file = f'{args.output}_comprehensive.png'
			elif plot_type == 'phase':
				fig = plot_phase_space(df)
				output_file = f'{args.output}_phase_space.png'
			elif plot_type == '3d':
				fig = plot_3d_positions(df)
				output_file = f'{args.output}_3d.png'
			elif plot_type == 'energy':
				fig = plot_energy_filter(df)
				output_file = f'{args.output}_energy.png'
			else:
				continue

			fig.savefig(output_file, dpi=150, bbox_inches='tight')
			print(f'Saved: {output_file}')
			plt.close(fig)

		if args.export:
			export_statistics(stats, args.export)

		print('\nBatch export complete.')
	else:
		# Interactive mode
		explorer = InteractiveExplorer(df, stats)
		explorer.run()


if __name__ == '__main__':
	main()
