import argparse
import math as m
import matplotlib.pyplot as plt


def calculate_npr(cps, delta_cps, sensitivity, delta_sens, distance, delta_distance):
    npr = (cps / sensitivity) * (4 * m.pi * distance**2)

    error_npr = (
        abs(delta_sens / sensitivity) +
        2 * abs(delta_distance / distance) +
        abs(delta_cps / cps)
    ) * npr

    return npr, error_npr


def plot_npr_with_error(npr, error_npr):
    plt.errorbar(1, npr, yerr=error_npr, fmt='o', label='NPR with error')
    plt.title('Neutron Production Rate (NPR) with Error')
    plt.xlabel('Sample')
    plt.ylabel('NPR (neutrons/s · cm2)')
    plt.legend()
    plt.show()


def main():
    parser = argparse.ArgumentParser(description="Calculate Neutron Production Rate (NPR)")
    parser.add_argument("--cps", type=float, help="Counts per second (cps)", required=True)
    parser.add_argument("--delta_cps", type=float, help="Error in counts per second (∆cps)", required=True)
    parser.add_argument("--sensitivity", type=float, help="Sensitivity of the detector (cps per neutron/s · cm2)", required=True)
    parser.add_argument("--delta_sens", type=float, help="Error in sensitivity (∆sens)", required=True)
    parser.add_argument("--distance", type=float, help="Distance between the center of the fusor and the neutron detector (cm)", required=True)
    parser.add_argument("--delta_distance", type=float, help="Error in distance (∆r)", required=True)
    parser.add_argument("--plot", action="store_true", help="Plot NPR with error bars")

    args = parser.parse_args()

    npr, error_npr = calculate_npr(args.cps, args.delta_cps, args.sensitivity, args.delta_sens, args.distance, args.delta_distance)

    print(f"Neutron Production Rate (NPR): {npr} neutrons/s · cm2")
    print(f"Error in NPR: {error_npr} neutrons/s · cm2")

    if args.plot:
        plot_npr_with_error(npr, error_npr)


if __name__ == "__main__":
    main()
