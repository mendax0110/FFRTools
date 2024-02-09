import argparse
import numpy as np
import matplotlib.pyplot as plt


def paschen_curve_cylinder(p, d, A):
    return 0.21 * A * (p * d / 2)**2


def main():
    parser = argparse.ArgumentParser(description='Plot Paschen Curve for Deuterium in a cylinder.')
    parser.add_argument('--A', type=float, default=173, help='Coefficient A')
    parser.add_argument('--d', type=float, default=0.15, help='Electrode distance in meters')
    parser.add_argument('--num_points', type=int, default=100, help='Number of data points for pressure values')
    args = parser.parse_args()

    p_values = np.logspace(-1, 4, args.num_points)

    Vb_values = paschen_curve_cylinder(p_values, args.d, args.A)

    plt.figure(figsize=(10, 6))
    plt.semilogx(p_values, Vb_values, label=f'Paschen Curve (A={args.A}) - Cylinder')
    plt.title(f'Paschen Curve for Deuterium in a Cylinder (d={args.d}m, {args.num_points} points)')
    plt.xlabel('Pressure (Pa)')
    plt.ylabel('Breakdown Voltage (V)')
    plt.grid(True)
    plt.legend()
    plt.show()


if __name__ == "__main__":
    main()
